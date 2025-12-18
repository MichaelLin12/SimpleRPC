#include "Client/Client.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

Client::Client():serializer{},id{}{
    boost::uuids::random_generator gen;
    this->id = gen();
}