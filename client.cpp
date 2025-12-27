#include "Client/Client.hpp"

int main(){
    Client c{};
    c.create();
    c.call();
    return 0;
}