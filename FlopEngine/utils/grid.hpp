#pragma once

#include <vector>
#include <span>

namespace utils
{

template <typename T>
struct grid
{
public:
    grid(size_t widht = size_t{}, size_t height = size_t{});

public:
    grid(const grid& other) = default;
    grid(grid&& other) noexcept = default;

    grid& operator=(const grid& other) = default;
    grid& operator=(grid&& other) noexcept = default;

public:
    size_t size() const noexcept;

    size_t plain_id(size_t x, size_t y) const;
    void set(size_t x, size_t y, T&& val);
    const T& get(size_t x, size_t y) const;

    std::span<T> row(size_t n);

    decltype(auto) data(this auto& self) noexcept;

    void resize(size_t widht, size_t height);

    decltype(auto) operator[](this auto& self, size_t i);

private:
    size_t _width;
    size_t _height;

    std::vector<T> _data;
};

template<typename T>
inline grid<T>::grid(size_t widht, size_t height)
    : _width{widht}
    , _height{height}
{
    _data.resize(size());
}

template<typename T>
inline size_t grid<T>::size() const noexcept
{
    return _width * _height;
}

template<typename T>
inline size_t grid<T>::plain_id(size_t x, size_t y) const
{
    return y * _width + x;
}

template<typename T>
inline void grid<T>::set(size_t x, size_t y, T&& val)
{
    _data[plain_id(x, y)] = std::forward<T>(val);
}

template<typename T>
inline const T& grid<T>::get(size_t x, size_t y) const
{
    return _data[plain_id(x, y)];
}

template<typename T>
inline std::span<T> grid<T>::row(size_t n)
{
    return std::span<T>{_data + n * _width, _data + n * (_width + 1)};
}

template<typename T>
inline decltype(auto) grid<T>::data(this auto& self) noexcept
{
    return self._data.data();
}

template<typename T>
inline void grid<T>::resize(size_t widht, size_t height)
{
    _width = widht;
    _height = height;

    _data.resize(size());
}

template<typename T>
inline decltype(auto) grid<T>::operator[](this auto& self, size_t i)
{
    return self._data[i];
}

} // namespace utils
