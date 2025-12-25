#include "Client/Client.hpp"
#include "Utility/Error.hpp"
#include "Utility/Constants.hpp"
#include <expected>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


std::expected<Client,ERROR> Client::create(const std::string& addr){
    Client c;
    boost::uuids::random_generator gen;
    c.id = gen();
    int status;
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(addr.c_str(), PORT, &hints, &res);
    if(status != 0) [[unlikely]] {
        return std::unexpected(ERROR::CONN_FAILED);
    }

    struct addrinfo* p;
    for(p = res; p != NULL; p = p->ai_next) {
        c.soc = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(c.soc == -1) [[unlikely]] {
            continue;
        }
        if (connect(c.soc, p->ai_addr, p->ai_addrlen) == -1) [[unlikely]] {
            close(c.soc);
            c.soc = -1;
            continue;
        }
        break;
    }

    if (p == NULL) [[unlikely]] {
        freeaddrinfo(res);
        return std::unexpected(ERROR::CONN_FAILED);
    }

    freeaddrinfo(res);
    return c;
}

Client::Client():serializer{},id{},soc{0},deserializer{}{}

Client::~Client(){
    if(this->soc == -1) [[unlikely]]
        return;
    close(this->soc); 
}

Client::Client(Client&& other)
    :id{std::move(other.id)},
    serializer{std::move(other.serializer)},
    deserializer{std::move(other.deserializer)}
{}

Client& Client::operator=(Client&& other){
    if(this!=&other) [[likely]] {
        this->id = std::move(other.id);
        this->serializer = std::move(other.serializer);
        this->deserializer = std::move(other.deserializer);
    }

    return *this;
}

std::expected<void,ERROR> Client::snd(const Message& m){

    ssize_t bytes = 0;
    size_t totalSent = 0;
    size_t size = m.getSize();
    char* buffer = m.getBuffer();
    while(totalSent < size){ // could use do while loop
        bytes = send(this->soc, buffer + totalSent, size - totalSent, 0);
        if(bytes == -1){
            return std::expected<void, ERROR>(std::unexpect, ERROR::SEND_FAILURE);
        }
        totalSent += static_cast<size_t>(bytes);
    }

    return std::expected<void,ERROR>{std::in_place};
}