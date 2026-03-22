#pragma once

template <typename T>
class SQBuff
{
public:
    SQBuff(std::size_t capacity);
    ~SQBuff();
    bool offer(const T& element);
    bool poll(T& ret);
    bool empty() const;
    size_t size() const;

private:
    static_assert(std::atomic<std::size_t>::is_always_lock_free);
    alignas(
        std::hardware_destructive_interference_size) std::atomic<size_t> head;
    alignas(
        std::hardware_destructive_interference_size) std::atomic<size_t> tail;
    const std::size_t capacity;
    T* data;
};