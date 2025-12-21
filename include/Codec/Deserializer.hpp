#pragma once
#include <cstddef>
#include <cstring>
#include <concepts>
#include <bit>

class Deserializer{
public:
    Deserializer()=default;

    template<typename Arg>
    requires (std::integral<Arg>)
    Arg deserialize(const char* buffer){
        Arg ret{};
        std::memcpy(&ret,buffer,sizeof(Arg));
        if constexpr(sizeof(std::decay_t<Arg>) > 1){
            ret = std::byteswap(ret);
        }
        return ret;
    }
};

// need move semantics as default
// delete copy semantics
// build tests for this