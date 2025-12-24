#pragma once
#include <Codec/Deserializer.hpp>

class Server{
public:
    static Server create();
    Server(const Server& other)=delete;
    Server(Server&& other);
    Server& operator=(const Server& other)=delete;
    Server& operator=(Server&& other);

    void run();
private:
    Server();
    Deserializer d;
    int soc;
};