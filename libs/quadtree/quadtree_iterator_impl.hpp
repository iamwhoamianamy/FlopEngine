#pragma once

#include "quadtree_impl.hpp"

namespace flp
{

template<concepts::quadtree Quadtree>
inline quadtree_iterator<Quadtree>::quadtree_iterator(
    const node_t* node, const geo::rectangle& range)
    : _cb{std::make_shared<control_block>(node, range)}
{
    _cb->is_end = false;
    find_first();
}

template<concepts::quadtree Quadtree>
inline quadtree_iterator<Quadtree>::reference
quadtree_iterator<Quadtree>::operator*() const
{
    return *(*(_cb->current_point));
}

template<concepts::quadtree Quadtree>
inline quadtree_iterator<Quadtree>::pointer
quadtree_iterator<Quadtree>::operator->() const
{
    return *(*(_cb->current_point));
}

template<concepts::quadtree Quadtree>
inline auto& quadtree_iterator<Quadtree>::operator++()
{
    find_next();
    return *this;
}

template<concepts::quadtree Quadtree>
inline auto quadtree_iterator<Quadtree>::operator++(int)
{
    auto result = quadtree_iterator(*this);

    find_next();

    return result;
}

template<concepts::quadtree Quadtree>
inline bool quadtree_iterator<Quadtree>::equal(
    const quadtree_iterator& other) const
{
    if (other._cb)
    {
        if (_cb->is_end == true)
            return true;
        else
        {
            return
                _cb->current_node  == other._cb->current_node &&
                _cb->current_point == other._cb->current_point;
        }
    }
    else
        return _cb->is_end == true;
}

template<concepts::quadtree Quadtree>
inline quadtree_iterator<Quadtree>
quadtree_iterator<Quadtree>::make_end()
{
    return quadtree_iterator{};
}

template<concepts::quadtree Quadtree>
inline void quadtree_iterator<Quadtree>::find_first()
{
    _cb->nodes_to_visit.push(_cb->current_node);

    find_next_impl();
}

template<concepts::quadtree Quadtree>
inline void quadtree_iterator<Quadtree>::find_next()
{
    _cb->current_point++;

    if (_cb->current_point != _cb->current_node->points().end() &&
        _cb->range.contains(flp::traits::converter<point_t*, vector2>::convert(*_cb->current_point)))
        return;

    find_next_impl();
}

template<concepts::quadtree Quadtree>
inline void quadtree_iterator<Quadtree>::find_next_impl()
{
    while (!_cb->nodes_to_visit.empty())
    {
        _cb->current_node = _cb->nodes_to_visit.top();
        _cb->nodes_to_visit.pop();

        if (!_cb->current_node->boundary().intersects(_cb->range))
            continue;

        auto find_iterator = std::find_if(
            _cb->current_node->points().begin(),
            _cb->current_node->points().end(),
            [&](const auto& point)
            {
                return _cb->range.contains(flp::traits::converter<point_t*, vector2>::convert(point));
            });

        if (find_iterator != _cb->current_node->points().end())
        {
            _cb->current_point = find_iterator;
            return;
        }
        else
        {
            for (const auto& child : _cb->current_node->children())
                _cb->nodes_to_visit.push(child.get());
        }
    }

    _cb->is_end = true;
}

template<concepts::quadtree Quadtree>
inline quadtree_iterator<Quadtree>::control_block::control_block(
    const node_t* node,
    const geo::rectangle& range)
    : current_node{node}
    , range{range}
{

}

} // namespace flp