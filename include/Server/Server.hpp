#pragma once
#include <Codec/Deserializer.hpp>
#include <expected>
#include <Utility/Error.hpp>
#include <unordered_map>
#include <string>
#include <functional>

class Server{
public:
    static std::expected<Server,ERROR> create();
    Server(const Server& other)=delete;
    Server(Server&& other);
    Server& operator=(const Server& other)=delete;
    Server& operator=(Server&& other);

    ~Server();

    void run();
    std::expected<void,ERROR> respond();
private:
    Server();
    Deserializer deserializer;
    int acceptSoc;
    std::unordered_map<std::string, std::function<void(char* buffer)>> functions;
};