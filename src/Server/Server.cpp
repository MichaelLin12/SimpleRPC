#include "Server/Server.hpp" 
#include "Utility/Error.hpp"
#include "Utility/Constants.hpp"
#include <expected>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//https://johnfarrier.com/the-definitive-guide-to-std-expected-in-c/
//https://www.cppstories.com/2024/expected-cpp23/

std::expected<Server,ERROR> Server::create(){
    Server s{};
    int yes=1;
    struct addrinfo hints;
    struct addrinfo* res;
    struct addrinfo* p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPV4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;

    int rv = getaddrinfo(NULL, PORT, &hints, &res);
    if(rv != 0) [[unlikely]]{
        return std::unexpected(ERROR::CONN_FAILED);
    }
    // AI_PASSIVE + NULL binds to all local interfaces (INADDR_ANY)
    for(p = res; p != NULL; p = p->ai_next) {
        if ((s.acceptSoc = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) [[unlikely]] {
            continue;
        }

        // lose the pesky "Address already in use" error message
        if (setsockopt(s.acceptSoc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) [[unlikely]] {
            freeaddrinfo(res);
            return std::unexpected(ERROR::SETSOCKOPT_FAILURE);
        }

        // associate socket with port
        if (bind(s.acceptSoc, p->ai_addr, p->ai_addrlen) == -1) [[unlikely]] {
            close(s.acceptSoc);
            s.acceptSoc = -1;
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (p == NULL)  {
        return std::unexpected(ERROR::BIND_FAILURE);
    }

    if (listen(s.acceptSoc, BACKLOG) == -1) {
        return std::unexpected(ERROR::LISTEN_FAILURE);
    }

    return s;
}

Server::Server():deserializer{},acceptSoc{-1},functions{}{}

Server::~Server(){
    if(this->acceptSoc == -1) [[unlikely]]
        return;
    close(this->acceptSoc);
}

Server::Server(Server&& other):deserializer{std::move(other.deserializer)},acceptSoc{other.acceptSoc},functions{other.functions}{}

Server& Server::operator=(Server&& other){
    if(this!=&other) [[likely]] {
        this->deserializer = std::move(other.deserializer);
        this->acceptSoc = other.acceptSoc;
        this->functions = other.functions;
    }

    return *this;
}

void Server::run(){ // assume only one socket
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    int socket = accept(this->acceptSoc,(struct sockaddr *)&their_addr,&addr_size);
    if(socket == -1) [[unlikely]] {
        return;
    }
    // we need to continuously run a loop
    while(true){
        // receive response -- handle partial reads
        // deserialize to get the size and the function name
        // get the function from the register
        // bind the args
        //execute the function
        //send back the result 
    }
    close(socket);
}