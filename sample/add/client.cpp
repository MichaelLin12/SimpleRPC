#include "Client/Client.hpp"
#include <string>
#include <iostream>

int main(){
    Client c{};
    c.create();
    std::string funcName = "add";
    int f = c.call<int,int,int>(funcName,3,5);
    std::cout << f << std::endl;
    return 0;
}