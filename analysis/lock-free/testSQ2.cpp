#include <atomic>
#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>

// spsc

template <typename T>
class SimpleQ
{
public: // assume capacity is correct
    SimpleQ(std::size_t capacity) : head{0}, tail{0}, capacity{capacity}
    {
        if (capacity == 0)
        {
            throw std::invalid_argument(
                "Queue capacity must be greater than zero.");
        }

        if (capacity == std::numeric_limits<size_t>::max())
        {
            throw std::overflow_error(
                "Queue capacity overflow, ensure capacity does not exceed "
                "std::numeric_limits<size_t>::max()) - 1");
        }
        data = static_cast<T*>(::operator new[](capacity * sizeof(T)));
    }

    ~SimpleQ()
    {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_relaxed);
        while (t != h)
        {
            if (t == capacity)
                t = 0;
            data[t].~T();
            ++t;
        }
        ::operator delete[](data);
    }

    bool offer(const T& element)
    {
        size_t head_t = head.load(std::memory_order_relaxed);

        if (head_t == capacity)
        {
            head_t = 0;
        }

        if (size() == capacity)
        {
            return false;
        }

        new (&data[head_t]) T(element);
        ++head_t;
        head.store(head_t, std::memory_order_release);
        return true;
    }

    bool poll(T& ret)
    {
        size_t tail_t = tail.load(std::memory_order_relaxed);
        if (tail_t == capacity)
        {
            tail_t = 0;
        }

        if (tail_t == head.load(std::memory_order_acquire))
        {
            return false;
        }

        ret = std::move(data[tail_t]);
        data[tail_t].~T();
        tail.store(tail_t, std::memory_order_release);
        return true;
    }

    bool empty() const
    {
        return tail.load(std::memory_order_acquire) ==
               head.load(std::memory_order_acquire);
    }

    size_t size() const
    {
        size_t head_t = head.load(std::memory_order_acquire);
        size_t tail_t = tail.load(std::memory_order_acquire);
        if (tail_t >= head_t)
        {
            return tail_t - head_t;
        }

        return capacity - head_t + tail_t;
    }

private:
#ifdef __cpp_lib_hardware_interference_size
    static constexpr size_t cache_line_size =
        std::hardware_destructive_interference_size;
#else
    static constexpr size_t cache_line_size = 64;
#endif

    static_assert(std::atomic<std::size_t>::is_always_lock_free);
    alignas(cache_line_size) std::atomic<size_t> head;
    alignas(cache_line_size) std::atomic<size_t> tail;
    const std::size_t capacity;
    T* data;
};

int main()
{
    SimpleQ<int> buffer{10};
    buffer.offer(2);
    int ret = 0;
    buffer.poll(ret);
    std::cout << ret << std::endl;
    return 0;
}