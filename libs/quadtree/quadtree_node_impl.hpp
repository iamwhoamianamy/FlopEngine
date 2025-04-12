#pragma once

#include "quadtree_impl.hpp"

#include <stack>
#include <queue>

namespace flp
{

template<typename Derived, concepts::quadtree_point Point>
inline quadtree_node_base<Derived, Point>::quadtree_node_base(
    const geo::rectangle& boundary,
    size_t capacity)
    : quadtree_node_base{boundary, capacity, 0}
{

}

template<typename Derived, concepts::quadtree_point Point>
inline quadtree_node_base<Derived, Point>::quadtree_node_base(
    const geo::rectangle& boundary,
    size_t capacity,
    size_t level)
    : _boundary{boundary}
    , _capacity{capacity}
    , _level{level}
{

}

template<typename Derived, concepts::quadtree_point Point>
inline void quadtree_node_base<Derived, Point>::insert(Point* point)
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

template<typename Derived, concepts::quadtree_point Point>
inline void quadtree_node_base<Derived, Point>::subdivide()
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

template<typename Derived, concepts::quadtree_point Point>
inline size_t quadtree_node_base<Derived, Point>::level() const
{
    return _level;
}

template<typename Derived, concepts::quadtree_point Point>
inline const geo::rectangle& quadtree_node_base<Derived, Point>::boundary() const
{
    return _boundary;
}

template<typename Derived, concepts::quadtree_point Point>
inline const auto& quadtree_node_base<Derived, Point>::children() const
{
    return _children;
}

template<typename Derived, concepts::quadtree_point Point>
inline const auto& quadtree_node_base<Derived, Point>::points() const
{
    return _points;
}

template<typename Derived, concepts::quadtree_point Point>
inline size_t quadtree_node_base<Derived, Point>::capacity() const
{
    return _capacity;
}

template<typename Derived, concepts::quadtree_point Point>
inline bool quadtree_node_base<Derived, Point>::subdivided() const
{
    return !_children.empty();
}

template<typename Derived, concepts::quadtree_point Point>
inline bool quadtree_node_base<Derived, Point>::empty() const
{
    return _points.empty();
}

template<typename Derived, concepts::quadtree_point Point>
inline bool quadtree_node_base<Derived, Point>::useless() const
{
    return !subdivided() && empty();
}

template<typename Derived, concepts::quadtree_point Point>
inline void quadtree_node_base<Derived, Point>::traverse_nodes_by_depth(
    auto&& visitor) const
{
    std::stack<const me_t*> nodes_to_visit;
    nodes_to_visit.push(this);

    while (nodes_to_visit.size())
    {
        const me_t* current_node = nodes_to_visit.top();
        nodes_to_visit.pop();

        visitor(current_node);

        for (auto& child : current_node->children())
            nodes_to_visit.push(child.get());
    }
}

template<typename Derived, concepts::quadtree_point Point>
inline void quadtree_node_base<Derived, Point>::traverse_nodes_by_width(
    auto&& visitor) const
{
    std::queue<decltype(this)> nodes_to_visit;
    nodes_to_visit.push(this);

    while (nodes_to_visit.size())
    {
        auto current_node = nodes_to_visit.front();
        nodes_to_visit.pop();

        visitor(static_cast<const me_t*>(current_node));

        for (auto& child : current_node->children())
            nodes_to_visit.push(child.get());
    }
}

template<typename Derived, concepts::quadtree_point Point>
inline void quadtree_node_base<Derived, Point>::traverse_nodes_by_width_reverse(
    auto&& visitor) const
{
    std::vector<decltype(this)> nodes_to_visit;

    traverse_nodes_by_width(
        [&nodes_to_visit](const me_t* node)
        {
            nodes_to_visit.push_back(node);
        });

    for (const auto* node : nodes_to_visit | std::views::reverse)
        visitor(static_cast<const me_t*>(node));
}

template<typename Derived, concepts::quadtree_point Point>
inline void quadtree_node_base<Derived, Point>::traverse_points_by_depth(
    auto&& visitor) const
{
    std::stack<decltype(this)> nodes_to_visit;
    nodes_to_visit.push(this);

    while (nodes_to_visit.size())
    {
        auto current_node = nodes_to_visit.top();
        nodes_to_visit.pop();

        for (auto& point : current_node->points())
            visitor(point);

        for (auto& child : current_node->children())
            nodes_to_visit.push(child.get());
    }
}

} // namespace flp