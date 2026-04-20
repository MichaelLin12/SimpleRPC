#pragma once
#include "Msg/Message.hpp"
#include "Msg/MessageTypes.hpp"
#include "Utility/Constants.hpp"
#include "Utility/Helper.hpp"
#include "Utility/Logger.hpp"
#include "Utility/TransCeive.hpp"
#include <concepts>
#include <fcntl.h>
#include <string>
#include <sys/epoll.h>

template <typename T>
    requires(std::integral<T> || std::same_as<T, bool> || std::same_as<T, char>)
inline std::size_t getSize(T arg)
{
    (void)arg;
    return sizeof(T);
}

template <typename T>
    requires(std::same_as<T, std::string>)
inline std::size_t getSize(T arg)
{
    return sizeof(std::size_t) + arg.size();
}

inline void setNonBlocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

inline int addSocket(int epfd, int sockfd, uint32_t echecks)
{
    struct epoll_event event;
    event.events = echecks;
    event.data.fd = sockfd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
}

inline Message receiveMsg(int socket)
{
    LOGGING(LogLevel::INFO, "Message::receiveMsg");
    Msg type = receiveMsgType(socket);
    LOGGING(LogLevel::INFO, "Msg type is: {}", static_cast<uint8_t>(type));
    std::size_t sz = receiveSize(socket);
    if (type == Msg::Void)
    {
        return {type, MSGTYPESIZE};
    }
    Message rec{type, sz};
    LOGGING(LogLevel::DEBUG, "rec size: {}", rec.getSize());
    LOGGING(LogLevel::DEBUG, "rec offset: {}", rec.getOffset());
    receiveAll(socket, rec);
    return rec;
}