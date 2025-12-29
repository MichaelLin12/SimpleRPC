#pragma once
#include <iostream>
#include <string>

inline void log_info(const std::string s){
    std::cout << s << std::endl;
}

inline void log_error(const std::string s){
    std::cerr << s << std::endl;
}