#include "Client/Client.hpp"
#include "Error/ErrMessage.hpp"
#include <expected>
#include <iostream>
#include <string>

int main()
{
    Client c{};
    c.create();
    std::string funcName = "doNothing";
    std::expected<void, ErrMessage> f = c.call<void>(funcName);
    f.and_then(
        []() -> std::expected<void, ErrMessage>
        {
            std::cout << "Things have worked as expected" << std::endl;
            return {};
        });
    return 0;
}