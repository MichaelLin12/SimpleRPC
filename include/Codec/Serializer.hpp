#pragma once
#include <string>
#include "Msg/Message.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

/*
* https://insujang.github.io/2022-03-12/implementing-basic-rpc-with-c-20/
* https://stackoverflow.com/questions/47871671/difference-between-stddecay-and-stdremove-reference
* std::decay will remove any cv-qualifer, remove_reference won't. 
* It will just strip the "reference" part of the type.
* Therefore std::decay will perform way more type conversions than std::remove_reference.
*There are also further type modifiers for more nuanced applications that will only perform selected 
*parts of the set of possible transformations decay does, like remove_cv, remove_volatile or, 
*in C++20, remove_cvref.
*/

// NOTE: WE CANNOT SERIALIZE POINTERS
class Serializer{
public:
    template<typename ... Args>
    Message serialize(const boost::uuids::uuid& id, const std::string& functionName, Args&&... args){
        size_t size = functionName.size() + (sizeof(Args) + ...);
        Message data{id, size};
        (encode<Args>(data,args),...);
        return data;
    }

    Serializer();
    Serializer(const Serializer& other)=delete;
    Serializer(Serializer&& other) noexcept;
    Serializer& operator=(const Serializer& other)=delete;
    Serializer& operator=(Serializer&& other) noexcept;

};