#pragma once
#include <concepts>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// Message class is immutable

class Message{
public:
    explicit Message(std::size_t size);

    template<typename T>
    requires (std::integral<T>)
    void add(T data){
        std::memcpy(curr,&data,sizeof(data));
        curr+=sizeof(data);
    }

    ~Message();

    //no copy's
    Message(const Message&) = delete;
    Message& operator=(const Message&) = delete;

    // only moves ie. transfer of ownership
    Message(Message&& other) noexcept;
    Message& operator=(Message&& other) noexcept;

private:
    void* buffer;
    void* curr;
    boost::uuids::uuid id;
};