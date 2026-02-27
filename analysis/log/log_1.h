#pragma once
#include <cerrno>
#include <cstring>
#include <format>
#include <iostream>
#include <string>

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#define LOGGING(x, ...)                                                        \
    do                                                                         \
    {                                                                          \
        std::string a = std::format(__VA_ARGS__);                              \
        logger((x), a);                                                        \
    } while (0)

enum class LogLevel
{
    INFO,
    DEBUG,
    ERROR
};

inline void log_debug(const std::string& s)
{
    std::cout << BOLDWHITE << s << RESET << std::endl;
};

inline void log_info(const std::string& s)
{
    std::cout << BOLDGREEN << s << RESET << std::endl;
};

inline void log_error(const std::string& s)
{
    std::cerr << BOLDRED << s << RESET << std::endl;
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