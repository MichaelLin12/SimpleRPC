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
        log_error(gai_strerror(rv));
        return;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            log_error(strerror(errno));
            continue;
        }

        log_info("client: attempting to connect");

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            log_error(strerror(errno));
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        log_error("Client failed to connect");
        return;
    }

    freeaddrinfo(servinfo);
}

/*
void Client::call(){
    Encoder encoder{};
    constexpr std::size_t sz = sizeof(std::size_t) + sizeof(std::size_t) + 3 + sizeof(int) + sizeof(int);
    std::cout << sz << std::endl;
    Message m{sz};
    std::string funcName = "add";
    int x = 5;
    int y = 3;
    encoder.encode(funcName,m);
    encoder.encode(x,m);
    encoder.encode(y,m);
    sendAll(sockfd,m.getBuffer());
}
*/

Client::~Client(){
    if(sockfd != -1){
        close(sockfd);
    }
}