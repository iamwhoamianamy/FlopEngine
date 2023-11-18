#pragma once

#include "grid.hpp"

#include "utils/ranges.h"
#include "utils/utils.hpp"

#include <ranges>

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
inline decltype(auto) grid<T>::get(this auto&& self, size_t x, size_t y)
{
    return self._data[self.plain_id(x, y)];
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
    return std::span<T>{_data.data() + n * _width, _data.data() + (n + 1) * _width};
}

template<typename T>
inline decltype(auto) grid<T>::data(this auto&& self) noexcept
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
inline void grid<T>::reinit(size_t widht, size_t height)
{
    _data.clear();
    resize(widht, height);
}

template<typename T>
inline decltype(auto) grid<T>::operator[](this auto&& self, size_t i)
{
    return self._data[i];
}

template<typename T>
inline auto grid<T>::top(size_t x, size_t y) -> std::optional<T*>
{
    if (static_cast<int>(y) - 1 >= 0)
        return _data.data() + plain_id(x, y) - _width;

    return std::nullopt;
}

template<typename T>
inline auto grid<T>::bot(size_t x, size_t y) -> std::optional<T*>
{
    if (y + 1 < _height)
        return _data.data() + plain_id(x, y) + _width;

    return std::nullopt;
}

template<typename T>
inline auto grid<T>::left(size_t x, size_t y) -> std::optional<T*>
{
    if (static_cast<int>(x) - 1 >= 0)
        return _data.data() + plain_id(x, y) - 1;

    return std::nullopt;
}

template<typename T>
inline auto grid<T>::right(size_t x, size_t y) -> std::optional<T*>
{
    if (x + 1 < _width)
        return _data.data() + plain_id(x, y) + 1;

    return std::nullopt;
}

template<typename T>
inline void grid<T>::swap(grid& other)
{
    std::swap(_data, other._data);
}

template<typename T>
inline auto grid<T>::as_plain_range()
{
    return utils::make_iterator_range(
        grid_plane_iterator<grid<T>>{_data.data(), * this},
        grid_plane_iterator<grid<T>>{_data.data() + size(), * this}
    );
}

template<typename T>
template<typename F>
requires GridPlaneIterationFunc<grid<T>, F>
inline void grid<T>::for_each_plane(auto&& execution_policy, F&& f)
{
    auto row_ids = utils::iota(_height);
    std::for_each(std::move(execution_policy), row_ids.begin(), row_ids.end(),
        [&](size_t row_id)
        {
            size_t col_id{};

            for (auto& val : row(row_id))
            {
                f(val, col_id, row_id, plain_id(col_id, row_id));

                col_id++;
            }
        });
}

template<typename T>
template<typename F>
    requires GridPlaneIterationFunc<grid<T>, F>
inline void grid<T>::for_each_plane(F&& f)
{
    for_each_plane(std::move(std::execution::seq), std::forward<F>(f));
}
