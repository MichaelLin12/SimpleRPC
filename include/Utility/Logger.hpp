#pragma once
#include <iostream>
#include <string>
#include <format>
#include <cerrno>
#include <cstring>
#include "Utility/Colors.hpp"

#ifdef ENABLELOGGING
#define LOGGING(x,...)\
do{\
    std::string a = std::format(__VA_ARGS__); \
    logger((x),a); \
}while(0)
#else
#define LOGGING(x,...) do{}while(0)
#endif

enum class LogLevel{
    INFO,
    DEBUG,
    ERROR
};

inline void log_debug(const std::string& s){
    std::cout << BOLDWHITE << s << RESET << std::endl;
};

inline void log_info(const std::string& s){
    std::cout << BOLDGREEN << s << RESET << std::endl;
};

inline void log_error(const std::string& s){
    std::cerr << BOLDRED << s << RESET << std::endl;
};

inline void logger(const LogLevel level, const std::string& s){
    switch (level)
    {
    case LogLevel::INFO:
        log_info(s);
        break;
    case LogLevel::DEBUG:
        log_debug(s);
        break;
    case LogLevel::ERROR:
        log_error(s);
        break;
    default:
        break;
    };
}