#pragma once
#include "Utility/Colors.hpp"
#include <cerrno>
#include <cstring>
#include <fmt/color.h>
#include <fmt/core.h>
#include <iostream>
#include <string>

#ifdef ENABLELOGGING
#define LOGGING(x, ...)                                                        \
    do                                                                         \
    {                                                                          \
        std::string a = fmt::format(__VA_ARGS__);                              \
        logger((x), a);                                                        \
    } while (0)
#else
#define LOGGING(x, ...)                                                        \
    do                                                                         \
    {                                                                          \
    } while (0)
#endif

enum class LogLevel
{
    INFO,
    DEBUG,
    ERROR
};

inline void log_debug(const std::string& s)
{
    fmt::print("{}{}{}\n", BOLDWHITE, s, RESET);
};

inline void log_info(const std::string& s)
{
    fmt::print("{}{}{}\n", BOLDGREEN, s, RESET);
};

inline void log_error(const std::string& s)
{
    fmt::print(stderr, "{}{}{}\n", BOLDRED, s, RESET);
};

inline void logger(const LogLevel level, const std::string& s)
{
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