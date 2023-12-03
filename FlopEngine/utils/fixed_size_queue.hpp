#pragma once

#include <deque>

namespace flp::utils
{

template<typename T, size_t Capacity = 1>
struct fixed_size_queue
{
public:
    fixed_size_queue();
    fixed_size_queue(const fixed_size_queue&) = default;
    fixed_size_queue(fixed_size_queue&&) = default;
    fixed_size_queue& operator=(const fixed_size_queue&) = default;
    fixed_size_queue& operator=(fixed_size_queue&&) = default;

    ~fixed_size_queue();

public:
    bool try_push(const T& value);
    bool try_push(T&& value);

    auto push(const T& value) -> std::optional<T>;
    auto push(T&& value) -> std::optional<T>;

    T& front();
    const T& front() const;

    T& back();
    const T& back() const;

    const auto& values() const;

    size_t capacity() const;
    size_t size() const;

private:
    bool try_push_impl(T&& value);
    auto push_impl(T&& value) -> std::optional<T>;

private:
    std::deque<T> _queue;
};

template<typename T, size_t Capacity>
inline fixed_size_queue<T, Capacity>::fixed_size_queue()
{

}

template<typename T, size_t Capacity>
inline fixed_size_queue<T, Capacity>::~fixed_size_queue()
{

}

template<typename T, size_t Capacity>
inline bool fixed_size_queue<T, Capacity>::try_push(const T& value)
{
    return try_push_impl(value);
}

template<typename T, size_t Capacity>
inline bool fixed_size_queue<T, Capacity>::try_push(T&& value)
{
    return try_push_impl(std::forward<T>(value));
}

template<typename T, size_t Capacity>
inline auto fixed_size_queue<T, Capacity>::push(const T& value) -> std::optional<T>
{
    return push_impl(value);
}

template<typename T, size_t Capacity>
inline auto fixed_size_queue<T, Capacity>::push(T&& value) -> std::optional<T>
{
    return push_impl(std::forward<T>(value));
}

template<typename T, size_t Capacity>
inline T& fixed_size_queue<T, Capacity>::front()
{
    return _queue.front();
}

template<typename T, size_t Capacity>
inline const T& fixed_size_queue<T, Capacity>::front() const
{
    return _queue.front();
}

template<typename T, size_t Capacity>
inline T& fixed_size_queue<T, Capacity>::back()
{
    return _queue.back();
}

template<typename T, size_t Capacity>
inline const T& fixed_size_queue<T, Capacity>::back() const
{
    return _queue.back();
}

template<typename T, size_t Capacity>
inline const auto& fixed_size_queue<T, Capacity>::values() const
{
    return _queue;
}

template<typename T, size_t Capacity>
inline size_t fixed_size_queue<T, Capacity>::capacity() const
{
    return Capacity;
}

template<typename T, size_t Capacity>
inline size_t fixed_size_queue<T, Capacity>::size() const
{
    return _queue.size();
}

template<typename T, size_t Capacity>
inline bool fixed_size_queue<T, Capacity>::try_push_impl(T&& value)
{
    if (_queue.size() < Capacity)
    {
        _queue.push_back(std::forward(value));
        return true;
    }

    return false;
}

template<typename T, size_t Capacity>
inline auto fixed_size_queue<T, Capacity>::push_impl(T&& value) -> std::optional<T>
{
    if (_queue.size() == Capacity)
    {
        T&& result = std::move(_queue.front());
        _queue.pop_front();

        _queue.push_back(std::forward<T>(value));

        return result;
    }

    _queue.push_back(std::forward<T>(value));
    return std::nullopt;
}

} // namespace flp::utils