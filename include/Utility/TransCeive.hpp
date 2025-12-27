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

void receiveAll(int socket, std::span<std::byte> buffer, int sz){
    std::size_t received = 0;
    while(received < sz){
        auto remainingView = buffer.subspan(received);
        ssize_t data = recv(socket,reinterpret_cast<char*>(remainingView.data()),remainingView.size(),0);
        if(data == -1){
            log_error(strerror(errno));
            std::abort();
        }

        if(data == 0){
            log_error("Client connection closed");
            std::abort();
        }
        received+=data;
    }
}