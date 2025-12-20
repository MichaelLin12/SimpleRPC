#pragma once
#include <string>
#include <type_traits>
#include "Msg/Message.hpp"
#include "Utility/Encoder.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

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
    Message serialize(const boost::uuids::uuid& id, std::string& functionName, Args&&... args){
        size_t size = sizeof(size_t) + functionName.size() + (sizeof(Args) + ...);
        Message data{id, size};
        encode<size_t>(data,functionName.size());
        encode<std::string>(data,functionName);
        (encode<std::decay_t<Args>>(data,args),...);
        return data;
    }

    Serializer()=default;
    Serializer(const Serializer& other)=delete;
    Serializer(Serializer&& other) noexcept = default;
    Serializer& operator=(const Serializer& other)=delete;
    Serializer& operator=(Serializer&& other) noexcept = default;

};