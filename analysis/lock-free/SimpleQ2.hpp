#pragma once
#include <atomic>
#include <cstddef>
#include <numeric>
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
        data = new T[capacity];
    }

    ~SimpleQ() { delete[] data; }

    bool offer(T element)
    {
        size_t head_t = head.load(std::memory_order_relaxed);

        if (head_t == capacity)
        {
            head_t = 0;
        }

        if (head_t == tail.load(std::memory_order_acquire))
        {
            return false;
        }

        data[head_t] = element;
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

        ret = data[tail_t];
        ++tail_t;
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
        size_t head = m_head.load(std::memory_order_acquire);
        size_t tail = m_tail.load(std::memory_order_acquire);
        if (tail >= head)
        {
            return tail - head;
        }

        return capacity - head + tail;
    }

private:
#ifdef __cpp_lib_hardware_interference_size
    static constexpr size_t cache_line_size =
        std::hardware_destructive_interference_size;
#else
    static constexpr size_t cache_line_size = 64;
#endif

    alignas(cache_line_size) std::atomic<size_t> head;
    alignas(cache_line_size) std::atomic<size_t> tail;
    const std::size_t capacity;
    T* data;
};