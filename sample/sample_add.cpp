#include <iostream>
#include "Client.hpp"

/*
int add(int a, int b){
    return a+b;
}
*/

int main(int argc, char** argv){
    Client c{};
    int res = c.call<int>("foo",4,5);
    if(res == 9){
        std::cout << "\033[32m" << "[ SUCCESS ]" << std::endl;
    }else{
        std::cout << "\033[31m" << "[ ERROR ]" << std::endl;
    }
    return 0;
}