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
#ifdef LOGGING
        log_debug(std::format("size is: {}",size));
        log_debug(std::format("buffer size is: {}", buffer.size()));
#endif
        std::string rt = std::string{reinterpret_cast<const char*>(buffer.data()), size};
        m.setOffset(m.getOffset() + size);
        return rt;
    }

    template<typename T>
    requires (std::integral<T>)
    T decode(Message& m){
        auto buffer = m.getData();
#ifdef LOGGING
        log_debug(std::format("buffer size is: {}", buffer.size()));
        log_debug(std::format("argument size is: {}", sizeof(T)));
#endif
        T value;
        std::memcpy(&value, buffer.data(), sizeof(T));
#ifdef LOGGING
        log_debug(std::format("value is: {}", value));
#endif
        m.setOffset(m.getOffset() + sizeof(T));
        if constexpr(std::endian::native == std::endian::little){
            value = std::byteswap(value);
        }
#ifdef LOGGING
        log_debug(std::format("value is:(potentially after byteswap) {}",value));
#endif
        return value;
    }
};