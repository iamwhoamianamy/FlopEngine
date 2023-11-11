#pragma once

#include "grid.hpp"

#include <utility>
#include <type_traits>

template<typename Grid>
inline grid_plane_iterator_proxy<Grid>::grid_plane_iterator_proxy(
    grid_plane_iterator_proxy::pointer ptr,
    const Grid& grid) noexcept
    : _ptr{ptr}
    , _grid{grid}
{
}

template<typename Grid>
inline grid_plane_iterator_proxy<Grid>::reference
grid_plane_iterator_proxy<Grid>::operator*() const
{
    return *_ptr;
}

template<typename Grid>
template<size_t Index>
inline size_t grid_plane_iterator_proxy<Grid>::get_index() const
{
    auto plane_id = std::distance(const_cast<pointer>(_grid.data()), _ptr);

    if constexpr (Index == 0) return plane_id % _grid.width();
    if constexpr (Index == 1) return plane_id / _grid.width();
}

template<typename Grid>
inline grid_plane_iterator<Grid>::grid_plane_iterator(
    Grid::value_t* ptr,
    const Grid& grid) noexcept
    : _proxy{std::make_shared<grid_plane_iterator_proxy<Grid>>(ptr, grid)}
{
}

template<typename Grid>
inline grid_plane_iterator<Grid>::reference
grid_plane_iterator<Grid>::operator*()
{
    return *_proxy;
}

template<typename Grid>
inline grid_plane_iterator<Grid>::pointer
grid_plane_iterator<Grid>::operator->() const
{
    return _proxy.get();
}

template<typename Grid>
inline auto& grid_plane_iterator<Grid>::operator++()
{
    _proxy->_ptr++;
    return *this;
}

template<typename Grid>
inline auto grid_plane_iterator<Grid>::operator++(int)
{
    auto new_iterator = *this;

    _proxy->_ptr++;

    return new_iterator;
}

namespace std
{

template<typename Grid>
struct tuple_size<grid_plane_iterator_proxy<Grid>> : integral_constant<size_t, 3>
{
};

template<typename Grid>
struct tuple_element<0, grid_plane_iterator_proxy<Grid>>
{
    using type = grid_plane_iterator_proxy<Grid>::reference;
};

template<typename Grid>
struct tuple_element<1, grid_plane_iterator_proxy<Grid>>
{
    using type = size_t;
};

template<typename Grid>
struct tuple_element<2, grid_plane_iterator_proxy<Grid>>
{
    using type = size_t;
};

template<size_t Index, typename Grid>
decltype(auto) get(grid_plane_iterator_proxy<Grid>& iterator)
{
    if constexpr (Index == 0) return *iterator;
    if constexpr (Index == 1) return iterator.get_index<0>();
    if constexpr (Index == 2) return iterator.get_index<1>();
};

}