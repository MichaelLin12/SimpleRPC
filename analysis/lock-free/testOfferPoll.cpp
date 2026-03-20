#include "SimpleQ.hpp"
#include <iostream>

int main()
{
    SimpleQ<int> test{10};

    for (int i = 0; i < 20; ++i)
    {
        test.offer(i);
    }

    while (!test.isEmpty())
    {
        int rt = 0;
        test.poll(rt);
        std::cout << rt << std::endl;
    }
    return 0;
}