#pragma once
#include <array>
#include <bit>
#include <cstddef>
#include <concepts>
#include <span>
#include <vector>
#include "Msg/Message.hpp"


class Encoder{
public:
    Encoder()=default;
    ~Encoder()=default;

    template<typename T>
    requires (std::same_as<T,std::string>)
    void encode(T arg, Message& msg){
        encode(arg.size(),msg);
        msg.addData(arg);
        return;
    }

    template<typename T>
    requires (std::integral<T>)
    void encode(T arg, Message& msg){
        // need to deal with big endian and little endian
        if constexpr(std::endian::native == std::endian::little && sizeof(arg) > 1){
            arg = std::byteswap(arg);
        }
        msg.addData(arg);
    }
};