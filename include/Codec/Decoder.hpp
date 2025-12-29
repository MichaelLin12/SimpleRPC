#pragma once
#include <array>
#include <bit>
#include <cstddef>
#include <span>
#include <string>
#include <cstring>
#include <bit>
#include <concepts>
#include <iostream>
#include "Msg/Message.hpp"

class Decoder{
public:
    Decoder()=default;
    ~Decoder()=default;

    template<typename T>
    requires (std::same_as<T,std::string>)
    T decode(Message& m){
        std::size_t size = decode<size_t>(m);
        auto buffer = m.getData();
        std::cout << "size is: " << size << std::endl;
        std::cout << "buffer size is: " << buffer.size() << std::endl;
        std::string rt = std::string{reinterpret_cast<const char*>(buffer.data()), size};
        m.setOffset(m.getOffset() + size);
        return rt;
    }

    template<typename T>
    requires (std::integral<T>)
    T decode(Message& m){
        // need to convert from big endian to little endian
        auto buffer = m.getData();
        std::cout << "buffer size is: " << buffer.size() << std::endl;
        std::cout << "argument size is: " << sizeof(T) << std::endl;
        T value;
        std::memcpy(&value, buffer.data(), sizeof(T));
        std::cout << "value is: " << value << std::endl;
        m.setOffset(m.getOffset() + sizeof(T));
        if constexpr(std::endian::native == std::endian::little){
            value = std::byteswap(value);
        }
        std::cout << "value is:(potentially after byteswap) " << value << std::endl;
        return value;
    }
};