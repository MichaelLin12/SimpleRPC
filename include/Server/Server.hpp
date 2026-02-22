#pragma once
#include <map>
#include <functional>
#include <tuple>
#include <string>
#include "Codec/Decoder.hpp"
#include "Codec/Encoder.hpp"
#include "Msg/Message.hpp"
#include "Utility/TransCeive.hpp"
#include "Utility/Logger.hpp"
#include "Utility/Handler.hpp"

template<typename Ret, typename... Args>
void dispatcher(void* fptr, int socket, Message& m) {
    auto func = reinterpret_cast<Ret(*)(Args...)>(fptr);

    Decoder decoder{};
    Encoder encoder{};

    std::tuple<std::decay_t<Args>...> arguments{
        decoder.decode<std::decay_t<Args>>(m)...
    };

    Ret rt = std::apply(func, arguments); 

    Message retM{sizeof(Ret)};
    encoder.encode(rt, retM);
    sendAll(socket, retM.getBuffer());
}

class Server{
public:
    Server();
    void create();
    void run();
    ~Server();
    std::size_t receiveSize(int socket);

    template<typename Ret, typename... Args>
    void registerFunction(std::string key, Ret(*func)(Args... args)){
        functions.insert(
            std::make_pair(
                std::move(key), 
                Handler{ &dispatcher<Ret, Args...>, reinterpret_cast<void*>(func) }
            )
        );
    }
private:
    int sockfd;
    std::map<std::string, Handler> functions;
};