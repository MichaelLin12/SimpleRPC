#pragma once
#include "Codec/Decoder.hpp"
#include "Codec/Encoder.hpp"
#include "Error/ErrMessage.hpp"
#include "Msg/Message.hpp"
#include "Utility/Helper.hpp"
#include "Utility/TransCeive.hpp"
#include <expected>
#include <string>
#include <type_traits>

class Client
{
public:
    Client() = default;
    void create();

    template <typename R, typename... Args>
    std::expected<R, ErrMessage> call(std::string& funcName, Args&&... args)
    {
        std::size_t size = MSGTYPESIZE + sizeof(size_t) + getSize(funcName);
        if constexpr (sizeof...(Args) > 0)
        {
            size += (getSize<std::remove_cvref_t<Args>>(args) + ...);
        }
        Message m{Msg::Send, size};
        encoder.encode(funcName, m);
        (encoder.encode<std::remove_cvref_t<Args>>(args, m), ...);
        sendAll(sockfd, m);
        Message rec = receiveMsg(sockfd);
        rec.setOffset(sizeof(static_cast<uint8_t>(rec.getType())) +
                      sizeof(rec.getSize()));
        if constexpr (std::is_void_v<R>)
        {
            switch (rec.getType())
            {
                case Msg::Void:
                {
                    return {};
                }
                case Msg::Err:
                {
                    return std::unexpected{ErrMessage{"Nothing"}};
                }
                default:
                {
                    std::unreachable();
                }
            }
        }
        else
        {
            switch (rec.getType())
            {
                case Msg::Resp:
                {
                    return decoder.decode<R>(rec);
                }
                case Msg::Err:
                {
                    return std::unexpected{ErrMessage{"Nothing"}};
                }
                default:
                {
                    std::unreachable();
                }
            }
        }
        return std::unexpected{
            ErrMessage{"The Server didn't return a valid message type"}};
    }

    ~Client();

private:
    int sockfd;
    Encoder encoder;
    Decoder decoder;
};