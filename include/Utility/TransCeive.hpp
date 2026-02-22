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

inline std::size_t receiveAll(int socket, std::span<std::byte> buffer, int sz){
    LOGGING(LogLevel::DEBUG, "buffer size is: {}", buffer.size());
    LOGGING(LogLevel::DEBUG, "sz is: {}",sz);
    std::size_t received = 0;
    while(received < sz){
        auto remainingView = buffer.subspan(received);
        ssize_t data = recv(socket,reinterpret_cast<char*>(remainingView.data()),remainingView.size(),0);
        if(data == -1){
            LOGGING(LogLevel::ERROR, "{}",strerror(errno));
            std::abort();
        }

        if(data == 0){
            LOGGING(LogLevel::ERROR, "Client connection closed");
            return 0;
            //std::abort();
        }
        received+=data;
    }

    return received;
}

inline void sendAll(int socket, std::span<std::byte> buffer){
    LOGGING(LogLevel::DEBUG, "buffer size is: ", buffer.size());
    std::size_t sent = 0;
    while(sent < buffer.size()){
        auto remainingView = buffer.subspan(sent);
        ssize_t data = send(socket,reinterpret_cast<char*>(remainingView.data()),remainingView.size(),0);
        if(data == -1){
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }

        if(data == 0){
            LOGGING(LogLevel::ERROR, "{}",strerror(errno));
            std::abort();
        }
        sent+=data;
    }
    LOGGING(LogLevel::DEBUG, "sent: {}", sent);
}