#include <expected>
#include <string>

std::expected<int, std::string> foo(bool fail)
{
    if (fail)
        return std::unexpected("error");
    return 42;
}

int main()
{
    auto r = foo(false);
    return r ? *r : -1;
}