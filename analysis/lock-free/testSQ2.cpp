#include "SimpleQ2.hpp"
#include <iostream>

int main()
{
    SimpleQ<int> buffer{10};
    for (int i = 0; i < 12; i++)
    {
        buffer.offer(i);
    }
    int ret = 0;
    for (int i = 0; i < 10; i++)
    {
        buffer.poll(ret);
        std::cout << ret << std::endl;
    }
    return 0;
}