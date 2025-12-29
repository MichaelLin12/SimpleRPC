#pragma once
#include <map>
#include <functional>
#include <span>
#include <bit>
#include <span>
#include <tuple>
#include "Codec/Decoder.hpp"
#include "Codec/Encoder.hpp"
#include <iostream>
#include <string>
#include "Msg/Message.hpp"
#include "Utility/TransCeive.hpp"
#include "Utility/Logger.hpp"

class Server{
public:
    Server();
    void create();
    void run();
    ~Server();
    std::size_t receiveSize(int socket);

    template<typename Ret, typename... Args>
    void registerFunction(std::string key, Ret(*func)(Args... args)){
        functions[key] = [func](int socket, Message& m)->void{
            Ret rt{};
            Decoder decoder{};
            Encoder encoder{};
            std::tuple arguments = std::make_tuple<std::decay_t<Args>...>(decoder.decode<std::decay_t<Args>>(m)...);
            rt = std::apply(func,arguments);
            log_debug(std::format("return value is: {}",rt));
            Message retM{sizeof(Ret)};
            encoder.encode(rt,retM);
            sendAll(socket,retM.getBuffer());
        };
    }
private:
    int sockfd;
    std::map<std::string, std::function<void(int,Message&)>> functions;
};