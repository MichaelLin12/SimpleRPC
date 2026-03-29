#include "Msg/Message.hpp"
#include "Msg/MessageTypes.hpp"
#include <array>
#include <bit>
#include <cstring>
#include <span>
#include <vector>

// size must include sz as well even if not explicit. It is up to the user
Message::Message(Msg type, std::size_t size)
    : buffer{size}, offset{0}, sz{size}, type{type}
{
    addData(static_cast<uint8_t>(type));
    if constexpr (std::endian::native != std::endian::big)
    {
        addData(std::byteswap(size));
    }
    else
    {
        addData(size);
    }
    offset = 1 + sizeof(size);
}

std::span<const std::byte> Message::getBuffer() const { return buffer; }

void Message::addRawData(std::span<std::byte> data)
{
    memcpy(buffer.data() + offset, data.data(), data.size_bytes());
    offset += data.size_bytes();
}

std::size_t Message::getSize() const { return sz; }

std::size_t Message::getOffset() const { return offset; }

void Message::setOffset(std::size_t noffset) { offset = noffset; }

Msg Message::getType() const { return type; }