#pragma once

#include "grid.hpp"

#include "utils/ranges.h"

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
inline size_t grid<T>::width() const
{
    return _width;
}

template<typename T>
inline size_t grid<T>::height() const
{
    return _height;
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

template<typename T>
inline auto grid<T>::as_plain_range()
{
    return utils::make_iterator_range(
        grid_plane_iterator<grid<T>>{_data.data(), * this},
        grid_plane_iterator<grid<T>>{_data.data() + size(), * this}
    );
}