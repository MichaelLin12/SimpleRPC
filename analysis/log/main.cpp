#include "log_1.h"
#include <format>

int main(int argc, char** argv)
{
    for (int i = 0; i < 10000; i++)
    {
        LOGGING(LogLevel::INFO, "{}", "test");
    }
    return 0;
}