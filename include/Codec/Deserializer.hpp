#pragma once
#include <cstddef>
#include <cstring>
#include <concepts>
#include <bit>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

class Deserializer{
public:
    Deserializer()=default;
    Deserializer(const Deserializer& other)=delete;
    Deserializer(Deserializer&& other)=default;
    Deserializer& operator=(const Deserializer& other)=delete;
    Deserializer& operator=(Deserializer&& other)=default;

    template<typename Arg>
    requires (std::integral<Arg>)
    Arg deserialize(const char* buffer){
        Arg ret{};
        std::memcpy(&ret,buffer,sizeof(Arg));
        if constexpr(sizeof(std::decay_t<Arg>) > 1){
            ret = std::byteswap(ret);
        }
        return ret;
    }

    template<typename Arg>
    requires (std::same_as<Arg,std::string>)
    Arg deserialize(const char* buffer){
        char* ptr = buffer;
        size_t size = deserialize<size_t>(ptr);
        ptr += sizeof(size_t);
        Arg rt{ptr,size};
        return rt;
    }

    template<typename Arg>
    requires (std::same_as<Arg,boost::uuids::uuid>)
    Arg deserialize(const char* buffer){
        boost::uuids::uuid id;
        std::memcpy(id.data,buffer,boost::uuids::uuid::static_size());
        return id;
    }

};

