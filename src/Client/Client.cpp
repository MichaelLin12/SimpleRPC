#include "Client/Client.hpp"
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Utility/Constants.hpp"
#include "Utility/Logger.hpp"
#include "Utility/TransCeive.hpp"
#include <array>
#include <cstddef>
#include <bit>
#include <Codec/Encoder.hpp>
#include <Msg/Message.hpp>


void Client::create(){
    int numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("localhost", PORT, &hints, &servinfo)) != 0) {
#ifdef LOGGING
        log_error(gai_strerror(rv));
#endif
        return;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            continue;
        }

#ifdef LOGGING
        log_info("client: attempting to connect");
#endif

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
#ifdef LOGGING
            log_error(strerror(errno));
#endif
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
#ifdef LOGGING
        log_error("Client failed to connect");
#endif
        return;
    }

    freeaddrinfo(servinfo);
}


Client::~Client(){
    if(sockfd != -1){
        close(sockfd);
    }
}