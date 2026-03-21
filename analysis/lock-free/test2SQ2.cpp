#include "SimpleQ2.hpp"
#include <iostream>
#include <thread>

void produce(SimpleQ<int>& buff)
{
    for (int i = 0; i < 10; i++)
    {
        buff.offer(i);
    }
}

void consume(SimpleQ<int>& buff)
{
    int res = 0;
    for (int i = 0; i < 10; i++)
    {
        buff.poll(res);
        std::cout << res << std::endl;
    }
}

int main()
{
    SimpleQ<int> buff{10};
    std::jthread t0{produce, std::ref(buff)};
    std::jthread t1{consume, std::ref(buff)};
    return 0;
}