#pragma once

#include <vector>
#include <span>
#include <iterator>
#include <functional>
#include <execution>

template <typename Grid>
struct grid_plane_iterator;

template <typename Grid>
struct grid_plane_iterator_proxy
{
    using value_t = Grid::value_t;
    using pointer = value_t*;
    using reference = value_t&;

public:
    grid_plane_iterator_proxy(pointer ptr, const Grid& grid) noexcept;

public:
    reference operator*() const;

public:
    template<size_t Index>
    size_t get_index() const;

public:
    pointer _ptr{};
    const Grid& _grid;

private:
    friend struct grid_plane_iterator<Grid>;
};

template <typename Grid>
struct grid_plane_iterator
{
public:
    using value_t = grid_plane_iterator_proxy<Grid>;

    // std boilerplate :(
    using iterator_concept = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = value_t;
    using pointer = value_t*;
    using reference = value_t&;

public:
    grid_plane_iterator(Grid::value_t* ptr, const Grid& grid) noexcept;

public:
    reference operator*();
    pointer operator->() const;

    auto& operator++();
    auto operator++(int);

    friend bool operator==(const grid_plane_iterator& a, const grid_plane_iterator& b)
    {
        return a._proxy->_ptr == b._proxy->_ptr;
    }

    friend bool operator!=(const grid_plane_iterator& a, const grid_plane_iterator& b)
    {
        return a._proxy->_ptr != b._proxy->_ptr;
    }

private:
    std::shared_ptr<value_t> _proxy;
};

template <typename Grid, typename F>
concept GridPlaneIterationFunc = std::invocable<
    F, 
    typename Grid::value_t& /* val */,
    size_t /* x */,
    size_t /* y */,
    size_t /* i */
>;

template <typename T>
struct grid
{
public:
    using value_t = T;
    using plane_iterator_t = grid_plane_iterator<grid<T>>;
    
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
    size_t width() const;
    size_t height() const;

    std::span<T> row(size_t n);

    decltype(auto) data(this auto& self) noexcept;

    void resize(size_t widht, size_t height);

    decltype(auto) operator[](this auto& self, size_t i);

public:
    auto as_plain_range();

    template <typename F>
        requires GridPlaneIterationFunc<grid<T>, F>
    void for_each_plane(auto&& execution_policy, F&& f);

    template <typename F>
        requires GridPlaneIterationFunc<grid<T>, F>
    void for_each_plane(F&& f);

private:
    size_t _width;
    size_t _height;

    std::vector<T> _data;
};

#include "grid_impl.hpp"
#include "grid_iterator_impl.hpp"
