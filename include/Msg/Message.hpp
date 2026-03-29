#pragma once
#include "Msg/MessageTypes.hpp"
#include <array>
#include <bit>
#include <concepts>
#include <cstring>
#include <span>
#include <string>
#include <vector>

class Message
{
public:
    template <typename T>
        requires(std::same_as<T, std::string>)
    void addData(const T& arg)
    {
        std::memcpy(buffer.data() + offset, arg.data(), arg.size());
        offset += arg.size();
    }

    template <typename T>
        requires(std::integral<T> || std::same_as<T, bool> ||
                 std::same_as<T, char>)
    void addData(const T& arg)
    {
        std::memcpy(buffer.data() + offset, &arg, sizeof(T));
        offset += sizeof(T);
    }

    std::span<const std::byte> getBuffer() const;
    Message(Msg type, std::size_t size);
    std::size_t getSize() const;
    std::size_t getOffset() const;
    Msg getType() const;
    void setOffset(std::size_t noffset);
    void addRawData(std::span<std::byte> data);

    ~Message() = default;

private:
    std::vector<std::byte> buffer;
    std::size_t offset;
    std::size_t sz;
    Msg type;
};