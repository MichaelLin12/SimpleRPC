#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#define BUFFER_CAPACITY 10

class Buffer
{
    int buffer[BUFFER_CAPACITY];
    int buffer_size = 0;
    int left = 0;
    int right = 0;

    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;

public:
    void produce(int thread_id, int num)
    {
        std::unique_lock<std::mutex> unique_lock(mtx);
        std::cout << "thread" << thread_id << " produced " << num << "\n";
        not_full.wait(unique_lock,
                      [this]() { return buffer_size != BUFFER_CAPACITY; });

        buffer[right] = num;
        right = (right + 1) % BUFFER_CAPACITY;
        buffer_size++;
        unique_lock.unlock();
        not_empty.notify_one();
    }

    int consume(int thread_id)
    {
        std::unique_lock<std::mutex> unique_lock(mtx);

        not_empty.wait(unique_lock, [this]() { return buffer_size != 0; });

        int result = buffer[left];
        std::cout << "thread" << thread_id << " consumed" << result
                  << std::endl;
        left = (left + 1) % BUFFER_CAPACITY;
        buffer_size--;
        unique_lock.unlock();
        not_full.notify_one();
        return result;
    }

    Buffer() {}
};

void produceInt(Buffer& buffer)
{
    for (int i = 0; i < 4; i++)
    {
        int new_int = rand() % 10 + 1;
        buffer.produce(0, new_int);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumeInt(Buffer& buffer)
{
    for (int i = 0; i < 4; i++)
    {
        buffer.consume(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
    std::cout << "Executing code in main" << std::endl;
    srand(time(NULL));
    Buffer buffer;
    std::thread produceThread0(produceInt, std::ref(buffer));
    std::thread produceThread1(consumeInt, std::ref(buffer));

    produceThread0.join();
    produceThread1.join();
}