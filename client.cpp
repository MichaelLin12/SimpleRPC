#include "Client/Client.hpp"
#include <string>

int main(){
    Client c{};
    c.create();
    std::string funcName = "add";
    c.call<int,int,int>(funcName,3,5);
    return 0;
}