#pragma once
#include "Buffer/SQBuff.hpp"
#include "Codec/Decoder.hpp"
#include "Codec/Encoder.hpp"
#include "Error/ErrMessage.hpp"
#include "Msg/Message.hpp"
#include "Utility/Handler.hpp"
#include "Utility/Helper.hpp"
#include "Utility/Logger.hpp"
#include "Utility/TransCeive.hpp"
#include <expected>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>

template <typename Ret, typename... Args>
void dispatcher(void* fptr, int socket, Message& m)
{
    using ExpectedRet = std::expected<Ret, ErrMessage>;
    auto func = reinterpret_cast<ExpectedRet (*)(Args...)>(fptr);

    Decoder decoder{};
    Encoder encoder{};

    std::tuple<std::decay_t<Args>...> arguments{
        decoder.decode<std::decay_t<Args>>(m)...};

    ExpectedRet result = std::apply(func, arguments);

    if (!result.has_value())
    {
        std::string errStr = result.error().message;
        Message errM{Msg::Err, 1 + sizeof(std::size_t) + getSize(errStr)};
        encoder.encode(errStr, errM);
        sendAll(socket, errM);
        return;
    }

    if constexpr (std::is_void_v<Ret>)
    {
        Message voidM{Msg::Void, 1};
        sendAll(socket, voidM);
    }
    else
    {
        Message retM{Msg::Resp,
                     1 + sizeof(std::size_t) + getSize(result.value())};
        encoder.encode(result.value(), retM);
        sendAll(socket, retM);
    }
}

class Server
{
public:
    Server();
    void create();
    void createWorkerThread();
    void run();
    ~Server();

    template <typename Ret, typename... Args>
    void registerFunction(std::string key,
                          std::expected<Ret, ErrMessage> (*func)(Args... args))
    { // potential race here
        functions.insert(std::make_pair(
            std::move(key),
            Handler{&dispatcher<Ret, Args...>, reinterpret_cast<void*>(func)}));
    }

private:
    int sockfd;
    std::map<std::string, Handler> functions;
    SQBuff<int> buffer;
    std::jthread workerThread;
};