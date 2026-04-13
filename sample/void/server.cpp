#include "Server/Server.hpp"
#include "Error/ErrMessage.hpp"
#include <expected>
#include <string>

std::expected<void, ErrMessage> doNothing() { return {}; }

int main()
{
    Server s{};
    s.create();
    std::string sp = "doNothing";
    s.registerFunction<void>(sp, doNothing);
    s.run();
    return 0;
}