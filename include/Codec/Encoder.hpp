#pragma once
#include <array>
#include <bit>
#include <cstddef>
#include <concepts>
#include <span>
#include <vector>


class Encoder{
public:
    Encoder()=default;
    ~Encoder()=default;

    template<typename T>
    requires (std::same_as<T,std::string>)
    std::vector<std::byte> encode(T arg){
        std::size_t sz = arg.size();
        std::vector<std::byte> res(sizeof(std::size_t) + sz);
        std::memcpy(res.data(),&(sz),sizeof(std::size_t));
        std::memcpy(res.data()+sizeof(std::size_t),arg.data(),arg.size());
        return res;
    }
};