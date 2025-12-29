#include "Server/Server.hpp"
#include <iostream>
#include <string>

int add(int x, int y){
    return x+y;
}

int main(){
    Server s{};
    s.create();
    std::string sp = "add";
    s.registerFunction(sp,add);
    s.run();
    return 0;
}