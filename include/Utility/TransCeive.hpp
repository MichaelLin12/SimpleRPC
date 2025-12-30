#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <span>
#include <bit>
#include "Utility/Logger.hpp"
#include <cstddef>

inline void receiveAll(int socket, std::span<std::byte> buffer, int sz){
#ifdef LOGGING
    log_debug(std::format("buffer size is: {}",buffer.size()));
    log_debug(std::format("sz is: {}",sz));
#endif
    std::size_t received = 0;
    while(received < sz){
        auto remainingView = buffer.subspan(received);
        ssize_t data = recv(socket,reinterpret_cast<char*>(remainingView.data()),remainingView.size(),0);
        if(data == -1){
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            std::abort();
        }

        if(data == 0){
#ifdef LOGGING
            log_error("Client connection closed");
#endif
            std::abort();
        }
        received+=data;
    }
}

inline void sendAll(int socket, std::span<std::byte> buffer){
#ifdef LOGGING
    log_debug(std::format("buffer size is: ", buffer.size()));
#endif
    std::size_t sent = 0;
    while(sent < buffer.size()){
        auto remainingView = buffer.subspan(sent);
        ssize_t data = send(socket,reinterpret_cast<char*>(remainingView.data()),remainingView.size(),0);
        if(data == -1){
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            std::abort();
        }

        if(data == 0){
#ifdef LOGGING
            log_error("Server connection closed");
#endif
            std::abort();
        }
        sent+=data;
    }
#ifdef LOGGING
    log_debug(std::format("sent: {}",sent));
#endif
}