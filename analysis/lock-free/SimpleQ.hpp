#pragma once

template <typename T>
class SimpleQ
{
public: // assume capacity is correct
    SimpleQ(std::size_t capacity)
        : write{0}, read{0}, size{0}, capacity{capacity}
    {
        data = new T[capacity];
    }

    ~SimpleQ() { delete[] data; }

    bool offer(T element)
    {
        if (size == capacity)
        {
            return false;
        }

        ++size;
        data[write % capacity] = element;
        ++write;
        return true;
    }

    bool poll(T& ret)
    {
        if (size == 0)
        {
            return false;
        }
        --size;
        ret = data[read % capacity];
        ++read;
        return true;
    }

    bool isEmpty() { return size == 0; }

private:
    std::size_t write;
    std::size_t read;
    std::size_t size;
    std::size_t capacity;
    T* data;
};