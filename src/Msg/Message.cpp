#include "Msg/Message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

Message::Message(std::size_t size):buffer{new char[size]},id{},curr{buffer}{
    boost::uuids::random_generator gen;
    this->id = gen();
}

Message::~Message(){
    delete[] buffer;
}