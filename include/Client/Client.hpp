#pragma once
#include "Codec/Decoder.hpp"
#include "Codec/Encoder.hpp"
#include "Msg/Message.hpp"
#include "Utility/Helper.hpp"
#include "Utility/TransCeive.hpp"
#include <string>
#include <type_traits>

class Client
{
public:
    Client() = default;
    void create();

    template <typename R, typename... Args>
    R call(std::string& funcName, Args&&... args)
    {
        std::size_t size = sizeof(size_t) + getSize(funcName) +
                           (getSize<std::remove_cvref_t<Args>>(args) + ...) + 1;
        Message m{Msg::Send, size};
        encoder.encode(funcName, m);
        (encoder.encode<std::remove_cvref_t<Args>>(args, m), ...);
        sendAll(sockfd, m);
        Message rec = receiveMsg(sockfd);
        rec.setOffset(sizeof(static_cast<uint8_t>(rec.getType())) +
                      sizeof(rec.getSize()));
        R rt{}; // assume default constructible for now
        switch (rec.getType())
        {
            case Msg::Resp:
            {
                rt = decoder.decode<R>(rec);
                break;
            }
            case Msg::Err:
            {
                break;
            }
            case Msg::Void:
            {
                break;
            }
            default:
            {
                break;
            }
        }
        return rt;
    }

    ~Client();

private:
    int sockfd;
    Encoder encoder;
    Decoder decoder;
};