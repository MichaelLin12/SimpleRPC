#pragma once
#include <array>
#include <bit>
#include <span>
#include <vector>
#include <concepts>
#include <string>
#include <cstring>

class Message{
public:
    template<typename T>
    requires (std::same_as<T,std::string>)
    void addData(const T& arg){
        std::memcpy(buffer.data()+offset, arg.data(),arg.size());
        offset+=arg.size();
    }

    template<typename T>
    requires (std::integral<T>)
    void addData(const T& arg){
        std::memcpy(buffer.data()+offset, &arg, sizeof(T));
        offset+=sizeof(T);
    }

    std::span<std::byte> getData();
    std::span<std::byte> getBuffer();
    Message(std::size_t size);
    std::size_t getSize();
    std::size_t getOffset();
    void setOffset(std::size_t noffset);
    

    ~Message()=default;
private:
    std::vector<std::byte> buffer;
    std::size_t offset;
    std::size_t sz;
};