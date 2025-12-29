#pragma once
#include <string>
#include <concepts>
#include <type_traits>
#include "Codec/Encoder.hpp"
#include "Codec/Decoder.hpp"
#include "Msg/Message.hpp"
#include "Utility/TransCeive.hpp"


class Client{
public:
    Client()=default;
    void create();

    template<typename R,typename... Args>
    void call(std::string& funcName,Args&&... args){
        Encoder encoder{};
        Decoder decoder{};
        std::size_t size = sizeof(size_t) + getSize(funcName) + (getSize<std::remove_cvref_t<Args>>(args) + ...);
        Message m{size};
        encoder.encode(size,m);
        encoder.encode(funcName,m);
        (encoder.encode<std::remove_cvref_t<Args>>(args,m),...);
        sendAll(sockfd,m.getBuffer());
        Message rec{sizeof(R)};
        std::cout << "rec size: " << rec.getSize() << std::endl;
        std::cout << "rec offset: " << rec.getOffset() << std::endl;
        receiveAll(sockfd,rec.getBuffer(),rec.getSize());
        R rt = decoder.decode<R>(rec);
        std::cout << rt << std::endl;
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