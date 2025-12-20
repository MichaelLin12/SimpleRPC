#pragma once
#include <concepts>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// Message class is only movable
// need to pass down uuids to construction

class Message{
public:
    explicit Message(boost::uuids::uuid clientID, std::size_t size);

    template<typename T>
    requires (std::integral<T>)
    void add(T data){
        std::memcpy(curr,&data,sizeof(data));
        curr+=sizeof(data);
    }

    ~Message();

    Message(const Message&) = delete;
    Message& operator=(const Message&) = delete;
    Message(Message&& other) noexcept;
    Message& operator=(Message&& other) noexcept;

    char* getBuffer();
    char* getCurr();
    size_t getSize();
    boost::uuids::uuid getID();
    
private:
    char* buffer;
    char* curr;
    boost::uuids::uuid id;
    size_t size;
};