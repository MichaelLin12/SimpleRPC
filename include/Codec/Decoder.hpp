#pragma once
#include <array>
#include <bit>
#include <cstddef>
#include <span>
#include <string>
#include <cstring>
#include <bit>
#include <concepts>
#include "Utility/Logger.hpp"
#include "Msg/Message.hpp"
#include <format>

class Decoder{
public:
    Decoder()=default;
    ~Decoder()=default;

    template<typename T>
    requires (std::same_as<T,std::string>)
    T decode(Message& m){
        std::size_t size = decode<size_t>(m);
        auto buffer = m.getData();
        LOGGING(LogLevel::DEBUG, "Size is: {}", size);
        LOGGING(LogLevel::DEBUG, "buffer size is: {}", buffer.size());
        std::string rt = std::string{reinterpret_cast<const char*>(buffer.data()), size};
        m.setOffset(m.getOffset() + size);
        return rt;
    }

    template<typename T>
    requires (std::integral<T>)
    T decode(Message& m){
        auto buffer = m.getData();
        LOGGING(LogLevel::DEBUG, "buffer size is: {}", buffer.size());
        LOGGING(LogLevel::DEBUG, "argument size is: {}", sizeof(T));
        T value;
        std::memcpy(&value, buffer.data(), sizeof(T));
        LOGGING(LogLevel::DEBUG, "value is: {}", value);
        m.setOffset(m.getOffset() + sizeof(T));
        if constexpr(std::endian::native == std::endian::little){
            value = std::byteswap(value);
        }
        LOGGING(LogLevel::DEBUG, "value is:(potentially after byteswap) {}", value);
        return value;
    }
};