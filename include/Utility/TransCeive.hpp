#pragma once
#include "Utility/Logger.hpp"
#include <arpa/inet.h>
#include <bit>
#include <cstddef>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <span>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

inline std::size_t receiveAll(int socket, std::span<std::byte> buffer,
                              std::size_t sz)
{
    LOGGING(LogLevel::DEBUG, "buffer size is: {}", buffer.size());
    LOGGING(LogLevel::DEBUG, "sz is: {}", sz);
    std::size_t received = 0;
    while (received < sz)
    {
        auto remainingView = buffer.subspan(received);
        ssize_t data =
            recv(socket, reinterpret_cast<char*>(remainingView.data()),
                 remainingView.size(), 0);
        if (data == -1)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }

        if (data == 0)
        {
            LOGGING(LogLevel::ERROR, "Client connection closed");
            return 0;
            // std::abort();
        }
        received += data;
    }

    return received;
}

inline void sendAll(int socket, std::span<std::byte> buffer)
{
    LOGGING(LogLevel::DEBUG, "buffer size is: ", buffer.size());
    std::size_t sent = 0;
    while (sent < buffer.size())
    {
        auto remainingView = buffer.subspan(sent);
        ssize_t data =
            send(socket, reinterpret_cast<char*>(remainingView.data()),
                 remainingView.size(), 0);
        if (data == -1)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }

        if (data == 0)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }
        sent += data;
    }
    LOGGING(LogLevel::DEBUG, "sent: {}", sent);
}