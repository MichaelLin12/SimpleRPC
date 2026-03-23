#include "Client/Client.hpp"
#include "Utility/Constants.hpp"
#include "Utility/Logger.hpp"
#include "Utility/TransCeive.hpp"
#include <Codec/Encoder.hpp>
#include <Msg/Message.hpp>
#include <arpa/inet.h>
#include <array>
#include <bit>
#include <cstddef>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void Client::create()
{
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("localhost", PORT, &hints, &servinfo)) != 0)
    {
        LOGGING(LogLevel::ERROR, "{}", gai_strerror(rv));
        return;
    }

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            continue;
        }
        LOGGING(LogLevel::INFO, "client: attempting to connect");
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            LOGGING(LogLevel::ERROR, "{}", strerror(errno));
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        LOGGING(LogLevel::ERROR, "Client failed to connect");
        return;
    }

    freeaddrinfo(servinfo);
}

Client::~Client()
{
    if (sockfd != -1)
    {
        close(sockfd);
    }
}