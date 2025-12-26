#include "Server/Server.hpp" 
#include "Utility/Error.hpp"
#include "Utility/Encoder.hpp"
#include "Utility/Constants.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <expected>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bit>
#include <array>


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

void Server::receive(int socket, std::string& funcName, std::size_t sz){
    char* buff = new char[sz+1]; // we don't send over the null character
    std::size_t received = 0;
    char* ptr = buff;
    while(received < sz){
        ssize_t bytes = recv(socket,ptr+received,sz - received,0);
        if(bytes == 0 || bytes == -1) [[unlikely]]{ //assume this never happens
            close(socket);
            return;
        }

        received += static_cast<std::size_t>(bytes);
    }
    funcName = buff;
    delete[] buff;
}

void Server::run(){ // assume only one socket
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    int socket = accept(this->acceptSoc,(struct sockaddr *)&their_addr,&addr_size);
    if(socket == -1) [[unlikely]] {
        return;
    }
    while(true){
        std::size_t buff_size=0;
        receive<std::size_t>(socket,buff_size);
        if constexpr(std::endian::native == std::endian::little){
            buff_size = std::byteswap(buff_size);
        }
        boost::uuids::uuid clientID;
        receive<boost::uuids::uuid>(socket,clientID);
        boost::uuids::uuid messageID;
        receive<boost::uuids::uuid>(socket,messageID);
        std::size_t funcNameSize;
        receive<std::size_t>(socket,funcNameSize);
        if constexpr(std::endian::native == std::endian::little){
            funcNameSize = std::byteswap(funcNameSize);
        }
        std::string funcName;
        receive(socket,funcName,funcNameSize);
        std::function<void(char* buffer,int socket)> func = functions[funcName];
        char* buffer = new char[buff_size - 2*sizeof(boost::uuids::uuid)-sizeof(size_t)-funcNameSize];
        receive(socket,buffer);
        // callProxy will send the data over
        // this is not necessarily great design ... but it'll have to do for now
        func(buffer,socket);
        delete[] buffer;
    }
    close(socket);
}