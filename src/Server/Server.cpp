#include <Server/Server.hpp>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "Utility/Constants.hpp"
#include <array>
#include <bit>
#include "Utility/Logger.hpp"
#include "Utility/TransCeive.hpp"
#include "Codec/Decoder.hpp"
#include "Msg/Message.hpp"
#include <format>


void Server::create(){
    struct addrinfo hints, *servinfo, *p;
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        std::abort();
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            log_error(strerror(errno));
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            log_error(strerror(errno));
            std::abort();
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            log_error(strerror(errno));
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        log_error("Server failed to bind");
        std::abort();
    }

    if (listen(sockfd, BACKLOG) == -1) {
        log_error(strerror(errno));
        std::abort();
    }

    log_info("waiting for connections");
}

void Server::run(){
    Decoder decoder{};
    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);
    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size);
    if (new_fd == -1) {
        log_error(strerror(errno));
        return;
    }

    size_t sz = receiveSize(new_fd);
    log_debug(std::format("size has been received... need to create message: {}", sz));
    Message m{sz};
    log_debug("created message");
    m.addData(sz);
    log_debug(std::format("received size: ",sz));
    log_debug(std::format("buffer size is: {}",m.getBuffer().size()));
    log_debug(std::format("real buffer size: {}",m.getSize()));
    log_debug(std::format("offset is: {}",m.getOffset()));
    receiveAll(new_fd,m.getData(),m.getSize() - m.getOffset());
    std::string name = decoder.decode<std::string>(m);
    std::span<std::byte> argBytes = m.getData();
    log_debug(std::format("argBytes size: {}",argBytes.size()));
    auto func = functions[name];
    func(new_fd,m);
    close(new_fd);
}

Server::~Server(){
    if(sockfd == -1)
        return;
    close(sockfd);
}

Server::Server():sockfd{-1},functions{}{}

std::size_t Server::receiveSize(int socket){
    log_debug("Server::receiveSize");
    std::size_t buf = 0;
    std::size_t received = 0;
    while(received < sizeof(std::size_t)){
        ssize_t bytes = recv(socket,&buf,sizeof(std::size_t),0);
        if(bytes == -1){
            log_error(strerror(errno));
            std::abort();
        }
        if(bytes == 0){
            log_error("client closed connection");
            std::abort();
        }
        received += bytes;
    }

    return std::byteswap(buf);
}