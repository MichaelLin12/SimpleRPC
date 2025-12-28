#pragma once
#include <array>
#include <bit>
#include <span>
#include <vector>

class Message{
public:
    void addData(const std::span<std::byte> arr);

    std::span<std::byte> getData();

    Message(std::size_t size);

    ~Message()=default;
private:
    std::vector<std::byte> buffer;
    std::size_t offset;
    std::size_t sz;
};