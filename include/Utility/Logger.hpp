#pragma once
#include <iostream>
#include <string>

void log_info(const std::string s){
    std::cout << s << std::endl;
}

void log_error(const std::string s){
    std::cerr << s << std::endl;
}