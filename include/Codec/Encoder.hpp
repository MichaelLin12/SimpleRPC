#pragma once
#include "Msg/Message.hpp"
#include "Utility/rpcCompliant.hpp"
#include <array>
#include <bit>
#include <concepts>
#include <cstddef>
#include <span>
#include <vector>

class Encoder
{
public:
    Encoder() = default;
    ~Encoder() = default;

    template <typename T>
        requires(std::same_as<T, std::string>)
    void encode(T& arg, Message& msg)
    {
        encode(arg.size(), msg);
        msg.addData(arg);
    }

    template <typename T>
        requires(std::integral<T>)
    void encode(T arg, Message& msg)
    {
        if constexpr (std::endian::native == std::endian::little &&
                      sizeof(arg) > 1)
        {
            arg = std::byteswap(arg);
        }
        msg.addData(arg);
    }

    template <typename T>
        requires(rpcCompliant<T>)
    void encode(T& arg, Message& msg)
    {
        arg.serialize(msg);
    }
};