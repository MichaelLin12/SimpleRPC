#include <iostream>
#include "Client.hpp"

/*
int add(int a, int b){
    return a+b;
}
*/

int main(int argc, char** argv){
    Client c{};
    c.call<int>("foo");
    return 0;
}