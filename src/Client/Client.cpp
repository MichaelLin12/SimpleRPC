#include "Client/Client.hpp"
#include "Utility/Error.hpp"
#include <expected>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// at some point you should look into scatter and gather io and look at implementing that

// WE NEED TO SEND THE CLIENTS IP POTENTIALLY

std::expected<Client,ERROR> Client::create(const std::string& addr){
    Client c;
    std::expected<Client,ERROR> ret = std::unexpected(ERROR::SUCCESS);
    boost::uuids::random_generator gen;
    c.id = gen();
    int status;
    struct addrinfo hints;
    struct addrinfo* res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(addr.c_str(), "3490", &hints, &res);
    if(status != 0) [[unlikely]] {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }

    struct addrinfo* p;
    for(p = res; p != NULL; p = p->ai_next) {
        c.soc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if(c.soc == -1) [[unlikely]] {
            continue;
        }
        if (connect(c.soc, p->ai_addr, p->ai_addrlen) == -1) [[unlikely]] {
            close(c.soc);
            continue;
        }
        break;
    }

    if (p == NULL) [[unlikely]] {
        ret = std::unexpected(ERROR::CONN_FAILED);
    }

    freeaddrinfo(res);
    if(ret.error() == ERROR::SUCCESS) [[likely]] {
        ret.emplace(std::move(c));
    }
    return ret;
}

Client::Client():serializer{},id{},soc{0}{}

Client::~Client(){
    close(this->soc); 
}

Client::Client(Client&& other)
    :id{std::move(other.id)},
    serializer{std::move(other.serializer)}
{}

Client& Client::operator=(Client&& other){
    if(this!=&other) [[likely]] {
        this->id = std::move(other.id);
        this->serializer = std::move(other.serializer);
    }

    return *this;
}

std::expected<void,ERROR> Client::snd(const Message& m){

    ssize_t bytes = 0;
    size_t totalSent = 0;
    size_t size = m.getSize();
    char* buffer = m.getBuffer();
    while(totalSent < static_cast<ssize_t>(size)){ // could use do while loop
        bytes = send(this->soc, buffer + totalSent, size - totalSent, 0);
        if(bytes == -1){
            return std::expected<void,ERROR>{std::unexpect,ERROR::SEND_FAILURE};
        }
        totalSent += bytes;
    }

    return std::expected<void,ERROR>{};
}