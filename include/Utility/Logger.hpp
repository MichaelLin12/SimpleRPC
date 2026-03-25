#pragma once
#include <cerrno>
#include <chrono>
#include <cstring>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <iostream>
#include <string>
#include <unistd.h>

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
    fmt::print(fmt::fg(fmt::color::cyan), "{} {} PID: {} TID: {} {} {}\n",
               "[DEBUG]", __FILE_NAME__, getpid(), gettid(),
               std::chrono::system_clock::now(), s);
};

inline void log_info(const std::string& s)
{
    fmt::print("{} {} PID: {} TID: {} {} {}\n", "[INFO]", __FILE_NAME__,
               getpid(), gettid(), std::chrono::system_clock::now(), s);
};

inline void log_error(const std::string& s)
{
    fmt::print(stderr, fmt::fg(fmt::color::red),
               "{} {} PID: {} TID: {} {} {}\n", "[ERROR]", __FILE_NAME__,
               getpid(), gettid(), std::chrono::system_clock::now(), s);
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