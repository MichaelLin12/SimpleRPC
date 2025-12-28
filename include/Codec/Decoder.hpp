#pragma once
#include <array>
#include <bit>
#include <cstddef>
#include <span>
#include <string>
#include <cstring>
#include <bit>
#include <concepts>

class Decoder{
public:
    Decoder()=default;
    ~Decoder()=default;

    template<typename T>
    requires (std::same_as<T,std::string>)
    T decode(std::span<std::byte>& buffer){
        std::size_t size = decode<size_t>(buffer);
        auto str = buffer.first(size);
        buffer = buffer.subspan(size);
        std::string rt = std::string{reinterpret_cast<const char*>(str.data()), size};
        return rt;
    }

    template<typename T>
    requires (std::integral<T>)
    T decode(std::span<std::byte>& buffer){
        // need to convert from big endian to little endian
        T value;
        std::memcpy(&value, buffer.data(), sizeof(T));
        buffer = buffer.subspan(sizeof(T));
        return value;
    }
};