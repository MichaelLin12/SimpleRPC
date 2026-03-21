#include "log_1.h"
#include <chrono>
#include <format>

int main(int argc, char** argv)
{
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; i++)
    {
        LOGGING(LogLevel::INFO, "{}", "test");
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed << std::endl;
    return 0;
}