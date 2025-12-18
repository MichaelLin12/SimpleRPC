#pragma once
#include <type_traits>
#include "Msg/Message.hpp"
#include "Utility/Traits.hpp"
#include <bit>
#include <limits>
#include <climits>

//https://www.studyplan.dev/pro-cpp/type-traits
//https://en.cppreference.com/w/cpp/types/is_integral.html
//https://en.cppreference.com/w/cpp/types/endian.html
//https://en.cppreference.com/w/cpp/numeric/byteswap.html
template<typename T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T,bool>,int> = 0>
void encoder(Message& msg, const T data){
    static_assert(std::numeric_limits<T>::digits == sizeof(T) * CHAR_BIT,"encoder<T>: T must have no padding bits");
    if constexpr (std::endian::native == std::endian::little){
        msg.add(std::byteswap(data));
    }else{
        msg.add(data);
    }
}

template<typename T, std::enable_if_t<std::is_same_v<T,bool>,int> = 0>
void encoder(Message& msg, const T data){
    msg.add(data);
}

//https://en.cppreference.com/w/cpp/types/is_floating_point.html
template<typename T, std::enable_if_t<std::is_floating_point_v<T>,int> = 0>
void encoder(Message& msg, T& data){
    // TODO
}