#pragma once


class Client{
public:
    Client()=default;

    void create();
    void call();
    ~Client();
private:
    int sockfd;
};