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
        std::size_t size = sizeof(size_t) + getSize(funcName) +
                           (getSize<std::remove_cvref_t<Args>>(args) + ...) + 1;
        Message m{Msg::Send, size};
        encoder.encode(funcName, m);
        (encoder.encode<std::remove_cvref_t<Args>>(args, m), ...);
        sendAll(sockfd, m);
        Message rec = receiveMsg(sockfd);
        rec.setOffset(sizeof(static_cast<uint8_t>(rec.getType())) +
                      sizeof(rec.getSize()));
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
            case Msg::Void:
            {
                return {};
            }
        }
        return std::unexpected
        {
            ErrMessage{"The Server didn't return a valid message type"};
        }

        ~Client();

    private:
        int sockfd;
        Encoder encoder;
        Decoder decoder;
    };