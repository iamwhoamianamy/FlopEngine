#pragma once
#include "quadtree_implementation.hpp"

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator()
{

}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator(
    const const_iterator& other)
{
    copy_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator(
    const_iterator&& other) noexcept
{
    copy_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::const_iterator(
    const quadtree& root, const rectangle& range)
    : _cb{std::make_shared<control_block>(root, range)}
{
    _cb->is_end = false;
    find_first();
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::~const_iterator()
{
}

template<traits::quadtree_point Point, size_t Capacity>
inline const Point& quadtree<Point, Capacity>::const_iterator::operator*() const
{
    return *(*(_cb->node_iterator));
}

template<traits::quadtree_point Point, size_t Capacity>
inline const Point* quadtree<Point, Capacity>::const_iterator::operator->() const
{
    return *(*(_cb->node_iterator));
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator& quadtree<Point, Capacity>::const_iterator::operator++()
{
    find_next();
    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator quadtree<Point, Capacity>::const_iterator::operator++(int)
{
    auto result = const_iterator(*this);

    find_next();

    return result;
}

template<traits::quadtree_point Point, size_t Capacity>
inline bool quadtree<Point, Capacity>::const_iterator::equal(const const_iterator& other) const
{
    if (other._cb)
    {
        if (_cb->is_end == true)
            return true;
        else
        {
            return _cb->node == other._cb->node &&
                _cb->node_iterator == other._cb->node_iterator;
        }
    }
    else
        return _cb->is_end == true;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator& quadtree<Point, Capacity>::const_iterator::operator=(const const_iterator& other)
{
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator& quadtree<Point, Capacity>::const_iterator::operator=(const_iterator&& other) noexcept
{
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator quadtree<Point, Capacity>::const_iterator::make_end()
{
    return const_iterator{};
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::find_first()
{
    _cb->nodes_to_visit.push(_cb->node);

    find_next_impl();
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::find_next()
{
    _cb->node_iterator++;

    if (_cb->node_iterator != _cb->node->points().end() &&
        _cb->range.contains(traits::access<Point>::position(*_cb->node_iterator)))
        return;

    find_next_impl();
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::find_next_impl()
{
    while (!_cb->nodes_to_visit.empty())
    {
        _cb->node = _cb->nodes_to_visit.top();
        _cb->nodes_to_visit.pop();

        if (!_cb->node->_rectangle.intersects(_cb->range))
            continue;

        auto find_iterator = std::find_if(
            _cb->node->points().begin(), _cb->node->points().end(),
            [&](auto point)
            {
                return _cb->range.contains(traits::access<Point>::position(point));
            });

        if (find_iterator != _cb->node->points().end())
        {
            _cb->node_iterator = find_iterator;
            return;
        }
        else
        {
            if (_cb->node->subdivided())
                for (auto& child : _cb->node->children())
                    _cb->nodes_to_visit.push(child);
        }
    }

    _cb->is_end = true;
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::const_iterator::copy_fields(const const_iterator& other)
{
    _cb = other._cb;
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::const_iterator::control_block::control_block(
    const quadtree& root, const rectangle& range)
    : node{&root}
    , range{range}
{

}