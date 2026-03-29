#pragma once
#include "Msg/MessageTypes.hpp"
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

inline std::size_t receiveSize(int socket)
{
    std::size_t buf = 0;
    std::size_t received = 0;
    while (received < sizeof(std::size_t))
    {
        ssize_t bytes = recv(socket, reinterpret_cast<char*>(&buf) + received,
                             sizeof(std::size_t) - received, 0);
        if (bytes == -1 && errno != EAGAIN)
        {
            LOGGING(LogLevel::INFO, "somthing wrong happened {}",
                    strerror(errno));
            std::abort();
        }
        if (bytes == 0)
        {
            LOGGING(LogLevel::INFO,
                    "received 0 bytes ... you should close the socket");
            break;
        }
        received += bytes;
    }

    return std::byteswap(buf);
}

inline Msg receiveMsgType(int socket)
{
    std::size_t received = 0;
    uint8_t data = 0;
    while (received < sizeof(data))
    {
        ssize_t bytes = recv(socket, reinterpret_cast<char*>(&data) + received,
                             sizeof(data) - received, 0);
        if (bytes == -1)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            std::abort();
        }

        if (bytes == 0)
        {
            LOGGING(LogLevel::ERROR, "Client connection closed");
            std::abort();
        }
        received += bytes;
    }

    return static_cast<Msg>(data);
}

inline std::size_t receiveAll(int socket, Message& msg)
{
    std::size_t received = 0;
    std::size_t sz = msg.getSize() - msg.getOffset();
    std::vector<std::byte> buffer{sz};
    while (received < sz)
    {
        ssize_t data = recv(
            socket, reinterpret_cast<unsigned char*>(buffer.data()) + received,
            sz - received, 0);
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

    msg.addRawData(buffer);
    return received;
}

inline void sendAll(int socket, const Message& m)
{
    auto buffer = m.getBuffer();
    LOGGING(LogLevel::DEBUG, "buffer size is: {}", buffer.size());
    std::size_t sent = 0;
    while (sent < buffer.size())
    {
        auto remainingView = buffer.subspan(sent);
        ssize_t data =
            send(socket, reinterpret_cast<const void*>(remainingView.data()),
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