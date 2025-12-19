#include "Client/Client.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

Client::Client():serializer{},id{}{
    boost::uuids::random_generator gen;
    this->id = gen();
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