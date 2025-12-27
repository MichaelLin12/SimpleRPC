#pragma once

class Server{
public:
    Server()=default;
    void create();

    void run();
private:
    int sockfd;
};