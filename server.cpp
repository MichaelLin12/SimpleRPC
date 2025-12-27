#include "Server/Server.hpp"

int main(){
    Server s{};
    s.create();
    s.run();
    return 0;
}