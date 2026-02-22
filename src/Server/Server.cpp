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
#ifdef LOGGING
        log_error(std::format("getaddrinfo: {}",gai_strerror(rv)));
#endif
        std::abort();
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            std::abort();
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL){
#ifdef LOGGING
        log_error("Server failed to bind");
#endif
        std::abort();
    }

    if (listen(sockfd, BACKLOG) == -1) {
#ifdef LOGGING
        log_error(strerror(errno));
#endif
        std::abort();
    }

#ifdef LOGGING
    log_info("waiting for connections");
#endif
}

void Server::run(){
    Decoder decoder{};
    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);
    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size);
    if (new_fd == -1) {
#ifdef LOGGING
        log_error(strerror(errno));
#endif
        return;
    }
    while(true){
        size_t sz = receiveSize(new_fd);
#ifdef LOGGING
        log_debug(std::format("size has been received... need to create message: {}", sz));
#endif
        Message m{sz};
#ifdef LOGGING
        log_debug("created message");
#endif
        m.addData(sz);
#ifdef LOGGING
        log_debug(std::format("received size: ",sz));
        log_debug(std::format("buffer size is: {}",m.getBuffer().size()));
        log_debug(std::format("real buffer size: {}",m.getSize()));
        log_debug(std::format("offset is: {}",m.getOffset()));
#endif
        receiveAll(new_fd,m.getData(),m.getSize() - m.getOffset());
        std::string name = decoder.decode<std::string>(m);
        std::span<std::byte> argBytes = m.getData();
#ifdef LOGGING
        log_debug(std::format("argBytes size: {}",argBytes.size()));
#endif
        auto func = functions[name];
        func(new_fd,m);
    }
    close(new_fd);
}

Server::~Server(){
    if(sockfd == -1)
        return;
    close(sockfd);
}

Server::Server():sockfd{-1},functions{}{}

std::size_t Server::receiveSize(int socket){
#ifdef LOGGING
    log_debug("Server::receiveSize");
#endif
    std::size_t buf = 0;
    std::size_t received = 0;
    while(received < sizeof(std::size_t)){
        ssize_t bytes = recv(socket,&buf,sizeof(std::size_t),0);
        if(bytes == -1){
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            std::abort();
        }
        if(bytes == 0){
#ifdef LOGGING
            log_error("client closed connection");
#endif
            std::abort();
        }
        received += bytes;
    }

    return std::byteswap(buf);
}