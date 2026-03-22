#pragma once
#include "Buffer/SQBuff.hpp"
#include "Codec/Decoder.hpp"
#include "Codec/Encoder.hpp"
#include "Msg/Message.hpp"
#include "Utility/Handler.hpp"
#include "Utility/Logger.hpp"
#include "Utility/TransCeive.hpp"
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <tuple>

template <typename Ret, typename... Args>
void dispatcher(void* fptr, int socket, Message& m)
{
    auto func = reinterpret_cast<Ret (*)(Args...)>(fptr);

    Decoder decoder{};
    Encoder encoder{};

    std::tuple<std::decay_t<Args>...> arguments{
        decoder.decode<std::decay_t<Args>>(m)...};

    Ret rt = std::apply(func, arguments);

    Message retM{sizeof(Ret)}; // no pointer
    encoder.encode(rt, retM);
    sendAll(socket,
            retM.getBuffer()); // could be the case that it doesn't send all
}

class Server
{
public:
    Server();
    void create();
    void createWorkerThread();
    void run();
    ~Server();
    std::size_t receiveSize(int socket);

    template <typename Ret, typename... Args>
    void registerFunction(std::string key, Ret (*func)(Args... args))
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