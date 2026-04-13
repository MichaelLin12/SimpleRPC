#include "Server/Server.hpp"
#include "Error/ErrMessage.hpp"
#include "Utility/Logger.hpp"
#include <expected>
#include <string>

std::expected<int, ErrMessage> add(int x, int y)
{
    LOGGING(LogLevel::INFO, "Calling Add with parameters {} {}", x, y);
    return x + y;
}

int main()
{
    Server s{};
    s.create();
    std::string sp = "add";
    s.registerFunction(sp, add);
    s.run();
    return 0;
}