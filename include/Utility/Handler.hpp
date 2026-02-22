#pragma once
#include "Msg/Message.hpp"

struct Handler{
    void(*call)(void* functionPointer, int socket, Message& msg);
    void* functionPointer;
};