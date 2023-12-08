#pragma once

#include "quadtree_impl.hpp"

namespace flp
{

template<concepts::quadtree_point Point>
inline quadtree_node<Point>::quadtree_node(
    const geo::rectangle& boundary,
    size_t capacity)
    : quadtree_node{boundary, capacity, 0}
{

}

template<concepts::quadtree_point Point>
inline quadtree_node<Point>::quadtree_node(
    const geo::rectangle& boundary,
    size_t capacity,
    size_t level)
    : _boundary{boundary}
    , _capacity{capacity}
    , _level{level}
{

}

template<concepts::quadtree_point Point>
inline void quadtree_node<Point>::insert(Point* point)
{
    if (!_boundary.contains(flp::traits::converter<Point*, vector2>::convert(point)))
    {
        return;
    }

    if (_points.size() < _capacity)
    {
        if (!subdivided())
        {
            _points.push_back(point);
        }
        else
        {
            for (auto& child : _children)
            {
                child->insert(point);
            }
        }
    }
    else
    {
        subdivide();
        _points.push_back(point);

        for (auto p : _points)
        {
            for (auto& child : _children)
            {
                child->insert(p);
            }
        }

        _points.clear();
        _points.shrink_to_fit();
    }
}

template<concepts::quadtree_point Point>
inline void quadtree_node<Point>::subdivide()
{
    float x = _boundary.center.x;
    float y = _boundary.center.y;

    float w = _boundary.half_dimensions.x;
    float h = _boundary.half_dimensions.y;

    vector2 children_half_dimensions(w / 2, h / 2);

    auto capacity = _level == (max_depth - 1) ? std::numeric_limits<size_t>::max() : _capacity;

    auto t1 = new me_t{geo::rectangle{{x - w / 2, y - h / 2}, children_half_dimensions}, capacity, _level + 1};
    auto t2 = new me_t{geo::rectangle{{x + w / 2, y - h / 2}, children_half_dimensions}, capacity, _level + 1};
    auto t3 = new me_t{geo::rectangle{{x + w / 2, y + h / 2}, children_half_dimensions}, capacity, _level + 1};
    auto t4 = new me_t{geo::rectangle{{x - w / 2, y + h / 2}, children_half_dimensions}, capacity, _level + 1};

    _children.emplace_back(t1);
    _children.emplace_back(t2);
    _children.emplace_back(t3);
    _children.emplace_back(t4);
}

template<concepts::quadtree_point Point>
inline const geo::rectangle& quadtree_node<Point>::boundary() const
{
    return this->_boundary;
}

template<concepts::quadtree_point Point>
inline const auto& quadtree_node<Point>::children() const
{
    return _children;
}

template<concepts::quadtree_point Point>
inline const auto& quadtree_node<Point>::points() const
{
    return _points;
}

template<concepts::quadtree_point Point>
inline size_t quadtree_node<Point>::capacity() const
{
    return _capacity;
}

template<concepts::quadtree_point Point>
inline bool quadtree_node<Point>::subdivided() const
{
    return !_children.empty();
}

template<concepts::quadtree_point Point>
inline bool quadtree_node<Point>::empty() const
{
    return _points.empty();
}

template<concepts::quadtree_point Point>
inline bool quadtree_node<Point>::useless() const
{
    return !subdivided() && empty();
}

} // namespace flp