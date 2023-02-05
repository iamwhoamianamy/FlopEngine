#pragma once
#include <queue>

template<typename T, size_t size>
struct fixed_size_queue
{
private:
    std::queue<T>  _queue;

public:
    fixed_size_queue(const fixed_size_queue&) = default;
    fixed_size_queue(fixed_size_queue&&) = default;
    fixed_size_queue& operator=(const fixed_size_queue&) = default;
    fixed_size_queue& operator=(fixed_size_queue&&) = default;

    fixed_size_queue();
    void try_push(T&& value);
    void force_push(T&& value);

    T& front();
    const T& front() const;
};

template<typename T, size_t size>
inline fixed_size_queue<T, size>::fixed_size_queue()
{

}

template<typename T, size_t size>
inline void fixed_size_queue<T, size>::try_push(T&& value)
{
    if (_queue.size() < size)
    {
        _queue.push(std::forward(value));
    }
}

template<typename T, size_t size>
inline void fixed_size_queue<T, size>::force_push(T&& value)
{
    if (_queue.size() == size)
    {
        _queue.pop();
    }

    _queue.push(std::forward(value));
}

template<typename T, size_t size>
inline T& fixed_size_queue<T, size>::front()
{
    return _queue.front();
}

template<typename T, size_t size>
inline const T& fixed_size_queue<T, size>::front() const
{
    return _queue.front();
}
