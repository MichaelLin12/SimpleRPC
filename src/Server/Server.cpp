#include "Codec/Decoder.hpp"
#include "Msg/Message.hpp"
#include "Utility/Constants.hpp"
#include "Utility/Helper.hpp"
#include "Utility/Logger.hpp"
#include "Utility/TransCeive.hpp"
#include <Server/Server.hpp>
#include <arpa/inet.h>
#include <array>
#include <bit>
#include <cerrno>
#include <cstring>
#include <future>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // close
#include <unistd.h>

void Server::create()
{
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        LOGGING(LogLevel::ERROR, "gai error: {}", gai_strerror(rv));
        std::abort(); // error
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1)
        {
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==
            -1)
        {
            std::abort();
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        std::abort();
    }

    setNonBlocking(sockfd);

    if (listen(sockfd, BACKLOG) == -1)
    {
        std::abort();
    }

    createWorkerThread();
}

void Server::run()
{
    int epfd = epoll_create1(0);
    if (epfd == -1)
    {
        LOGGING(LogLevel::ERROR, "cannot create epoll: {}", strerror(errno));
        throw std::runtime_error(strerror(errno)); // change
    }

    struct epoll_event events[MAX_EVENTS];
    int running = 1;
    int event_count = 0;
    int i = 0;

    if (addSocket(epfd, sockfd, EPOLLIN))
    {
        LOGGING(LogLevel::ERROR, "Failed to add file descriptor to epoll: {}",
                strerror(errno));
        close(epfd);
        return;
    }

    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);
    while (running)
    {
        LOGGING(LogLevel::INFO, "Polling for input...");
        event_count = epoll_wait(epfd, events, MAX_EVENTS, TIMEOUT);
        LOGGING(LogLevel::INFO, "{} ready events", event_count);
        for (i = 0; i < event_count; i++)
        {
            LOGGING(LogLevel::INFO, "Reading file descriptor: {}",
                    events[i].data.fd);
            if (events[i].data.fd == sockfd)
            {
                // we have a new connection
                int new_fd =
                    accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
                if (new_fd == -1)
                {
                    continue;
                }

                setNonBlocking(new_fd);
                if (addSocket(epfd, new_fd, EPOLLIN | EPOLLRDHUP))
                {
                    LOGGING(LogLevel::ERROR,
                            "Failed to add file descriptor to epoll: {}",
                            strerror(errno));
                    close(new_fd);
                    continue;
                }
            }
            else if (events[i].events & EPOLLRDHUP)
            {
                LOGGING(LogLevel::INFO, "client has closed socket");
                epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                close(events[i].data.fd);
            }
            else
            {
                buffer.offer(events[i].data.fd);
            }
        }
    }

    if (close(epfd))
    {
        LOGGING(LogLevel::ERROR, "Failed to close epoll file descriptor: {}",
                strerror(errno));
        return;
    }
}

void Server::createWorkerThread()
{
    auto fn = [this](int new_fd)
    {
        Decoder decoder{};
        size_t sz = receiveSize(new_fd);
        if (sz == 0)
        {
            return;
        }
        Message m{sz};
        m.addData(sz);
        std::size_t received =
            receiveAll(new_fd, m.getData(), m.getSize() - m.getOffset());
        if (!received)
            return;
        std::string name = decoder.decode<std::string>(m);
        std::span<std::byte> argBytes = m.getData();
        auto handler = functions[name]; // assume always true for now
        handler.call(handler.functionPointer, new_fd,
                     m); // assume works as intended for now -- as in no errors
    };

    auto worker = [this, fn](std::stop_token stoken)
    {
        int fd;
        while (!stoken.stop_requested())
        {
            if (buffer.poll(fd))
            {
                fn(fd);
            }
            else
            {
                std::this_thread::yield();
            }
        }
    };
    workerThread = std::jthread{worker};
}

Server::~Server()
{
    workerThread.request_stop();
    if (sockfd == -1)
        return;
    close(sockfd);
}

Server::Server() : sockfd{-1}, functions{}, buffer{BUFFERSIZE} {}

std::size_t Server::receiveSize(int socket)
{
    std::size_t buf = 0;
    std::size_t received = 0;
    while (received < sizeof(std::size_t))
    {
        ssize_t bytes = recv(socket, &buf, sizeof(std::size_t), 0);
        if (bytes == -1 && errno != EAGAIN)
        {
            LOGGING(LogLevel::INFO, "somthing wrong happened {}",
                    strerror(errno));
            std::abort();
        }
        if (bytes == 0)
        { // client side has closed -- need to better handle this error
            LOGGING(LogLevel::INFO,
                    "received 0 bytes ... you should close the socket");
            break;
        }
        received += bytes;
    }

    return std::byteswap(buf);
}