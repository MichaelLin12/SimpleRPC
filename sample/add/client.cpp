#include "Client/Client.hpp"
#include "Error/ErrMessage.hpp"
#include <expected>
#include <iostream>
#include <string>

int main()
{
    Client c{};
    c.create();
    std::string funcName = "add";
    std::expected<int, ErrMessage> f = c.call<int, int, int>(funcName, 3, 5);
    std::cout << *f << std::endl;
    return 0;
}