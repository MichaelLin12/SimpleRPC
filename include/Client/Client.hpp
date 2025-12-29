#pragma once
#include <string>
#include <concepts>
#include <type_traits>
#include "Codec/Encoder.hpp"
#include "Msg/Message.hpp"
#include "Utility/TransCeive.hpp"


class Client{
public:
    Client()=default;
    void create();

    template<typename R,typename... Args>
    void call(std::string& funcName,Args&&... args){
        Encoder encoder{};
        std::size_t size = sizeof(size_t) + getSize(funcName) + (getSize<std::remove_cvref_t<Args>>(args) + ...);
        Message m{size};
        encoder.encode(funcName,m);
        (encoder.encode<std::remove_cvref_t<Args>>(args,m),...);
        sendAll(sockfd,m.getBuffer());
    }

    template<typename T>
    requires (std::integral<T>)
    std::size_t getSize(T arg){
        return sizeof(T);
    }

    template<typename T>
    requires (std::same_as<T,std::string>)
    std::size_t getSize(T arg){
        return sizeof(std::size_t) + arg.size();
    }

    ~Client();
private:
    int sockfd;
};