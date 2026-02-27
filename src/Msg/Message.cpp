#include "Msg/Message.hpp"
#include <array>
#include <bit>
#include <span>
#include <vector>
#include <cstring>
#include <iostream>

//size must include sz as well even if not explicit. It is up to the user
Message::Message(std::size_t size):buffer(size),offset{0},sz{size}{}

std::span<std::byte> Message::getData(){
    return std::span{buffer.data() + offset,sz - offset};
}

std::span<std::byte> Message::getBuffer(){
    return buffer;
}

std::size_t Message::getSize(){
    return sz;
}

std::size_t Message::getOffset(){
    return offset;
}

void Message::setOffset(std::size_t noffset){
    offset = noffset;
}