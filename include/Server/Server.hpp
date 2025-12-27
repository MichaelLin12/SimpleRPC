#pragma once

class Server{
public:
    Server()=default;
    void create();

    void run();

    ~Server();
private:
    int sockfd;
};