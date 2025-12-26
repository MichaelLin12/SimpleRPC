#include "Msg/Message.hpp"
#include <bit>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// need to pass in client uuid and message uuid - and add size to the buffer
// Its total size, client uuid, message uuid, function name size, function name, Args....
Message::Message(boost::uuids::uuid clientID, std::size_t size)
        :buffer{new char[size + 2*sizeof(boost::uuids::uuid) + sizeof(size_t)]},
        id{},
        curr{buffer},
        size{size+2*sizeof(boost::uuids::uuid) + sizeof(size_t)}
{
    boost::uuids::random_generator gen;
    this->id = gen();

    
    std::size_t v = this->size;
    if constexpr(std::endian::native == std::endian::little){
        v = std::byteswap(v);
    }

    std::memcpy(curr,&v,sizeof(v));
    curr+=sizeof(v);

    boost::uuids::uuid cliID = clientID;
    std::memcpy(curr,&cliID,sizeof(cliID));
    curr+=sizeof(cliID);

    boost::uuids::uuid msgID = this->id;
    std::memcpy(curr,&msgID,sizeof(msgID));
    curr+=sizeof(msgID);
}

Message::~Message(){
    delete[] buffer;
}

Message& Message::operator=(Message&& other) noexcept
{
    if (this != &other) [[likely]] {
        delete[] buffer;

        buffer = other.buffer;
        curr = other.curr;
        size = other.size;
        id = std::move(other.id);

        other.buffer = nullptr;
        other.curr = nullptr;
        other.size = 0;
    }
    return *this;
}


Message::Message(Message&& other) noexcept
     :buffer(other.buffer),
      curr(other.curr),
      size(other.size),
      id(std::move(other.id))
{
    other.buffer = nullptr;
    other.curr = nullptr;
    other.size = 0;
}

char* Message::getBuffer(){
    return this->buffer;
}

char* Message::getCurr(){
    return this->curr;
}

size_t Message::getSize(){
    return this->size;
}

char* Message::getBuffer() const{
    return this->buffer;
}

char* Message::getCurr() const{
    return this->curr;
}

size_t Message::getSize() const{
    return this->size;
}

boost::uuids::uuid Message::getID(){
    return this->id;
}