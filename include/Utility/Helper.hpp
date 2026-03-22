#pragma once
#include <fcntl.h>
#include <sys/epoll.h>

inline void setNonBlocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

inline int addSocket(int epfd, int sockfd)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
}