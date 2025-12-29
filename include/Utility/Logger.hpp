#pragma once
#include <iostream>
#include <string>
#include "Utility/Colors.hpp"

inline void log_debug(const std::string s){
    std::cout << BOLDCYAN << s << RESET << std::endl;
}

inline void log_info(const std::string s){
    std::cout << BOLDGREEN << s << RESET << std::endl;
}

inline void log_error(const std::string s){
    std::cerr << BOLDRED << s << RESET << std::endl;
}