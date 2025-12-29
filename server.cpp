#include "Server/Server.hpp"
#include <iostream>
#include <string>

int add(int x, int y){
    std::cout << x+y << std::endl;
    return 5;
}

int main(){
    Server s{};
    s.create();
    std::string sp = "add";
    s.registerFunction(sp,add);
    s.run();
    return 0;
}