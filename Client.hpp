#pragma once
#include <string>

class Client{
public:
    Client();


    // Function templates (including template member functions) must be defined 
    // in the header, not just declared.
    template<typename Return, typename... Args>
    Return call(const std::string& functionName, Args&& ... args){
        if constexpr (std::is_void_v<Return>) {
            // do work
            return;
        } else {
            // do work
            return result;
        }
    }
};