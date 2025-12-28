#include "Msg/Message.hpp"
#include <array>
#include <bit>
#include <span>
#include <vector>
#include <cstring>

Message::Message(std::size_t size):buffer(size),offset{0},sz{size}{
    std::memcpy(buffer.data(),&sz,sizeof(sz));
    offset += sizeof(sz);
}

std::span<std::byte> Message::getData(){
    return std::span<std::byte>{buffer.data(),offset};
}

void Message::addData(const std::span<std::byte> arr){
    if(offset + arr.size() <= sz){
        std::memcpy(buffer.data() + offset, arr.data(), arr.size());
        offset += arr.size();
    }
}