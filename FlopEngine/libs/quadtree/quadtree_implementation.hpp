#pragma once
#include "libs/quadtree/quadtree.hpp"

#include "utils/utils.hpp"

template<traits::quadtree_point Point>
inline quadtree<Point>::quadtree(const quadtree& other)
{
    copy_fields(other);
}

template<traits::quadtree_point Point>
inline quadtree<Point>::quadtree(quadtree&& other)
{
    move_fields(other);
}

template<traits::quadtree_point Point>
inline quadtree<Point>::quadtree(
    const rectangle& boundary_rectangle,
    size_t capacity)
    : quadtree{boundary_rectangle, capacity, 0}
{

}

template<traits::quadtree_point Point>
inline quadtree<Point>::quadtree(
    const rectangle& boundary_rectangle,
    size_t capacity,
    size_t level)
    : _rectangle(boundary_rectangle)
    , _capacity{capacity}
    , _level{level}
{

}

template<traits::quadtree_point Point>
void quadtree<Point>::insert(std::vector<Point>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<traits::quadtree_point Point>
void quadtree<Point>::insert(Point* point)
{
    if(!_rectangle.contains(traits::access<Point>::position(point)))
    {
        return;
    }

    if(_points.size() < _capacity)
    {
        if(!subdivided())
        {
            _points.push_back(point);
        }
        else
        {
            for(auto& child : _children)
            {
                child->insert(point);
            }
        }
    }
    else
    {
        subdivide();
        _points.push_back(point);

        for(auto p : _points)
        {
            for(auto& child : _children)
            {
                child->insert(p);
            }
        }

        _points.clear();
        _points.shrink_to_fit();
    }
}

template<traits::quadtree_point Point>
inline void quadtree<Point>::commit()
{
    std::stack<node_t*> nodes_to_visit;
    nodes_to_visit.push(const_cast<node_t*>(this));

    while (!nodes_to_visit.empty())
    {
        const auto& node = nodes_to_visit.top();
        nodes_to_visit.pop();

        std::vector<std::unique_ptr<node_t>> new_children;

        for (auto i : utils::iota(node->_children.size()))
        {
            if (!node->_children[i]->useless())
            {
                new_children.emplace_back(std::move(node->_children[i]));
            }
        }

        node->_children = std::move(new_children);

        for (const auto& child : node->_children)
            nodes_to_visit.push(child.get());
    }
}

template<traits::quadtree_point Point>
void quadtree<Point>::subdivide()
{
    float x = _rectangle.center.x;
    float y = _rectangle.center.y;

    float w = _rectangle.half_dimensions.x;
    float h = _rectangle.half_dimensions.y;

    vector2 children_half_dimensions(w / 2, h / 2);

    auto capacity = _level == (max_depth - 1) ? std::numeric_limits<size_t>::max() : _capacity;

    auto t1 = new quadtree<Point>{rectangle{{x - w / 2, y - h / 2}, children_half_dimensions}, capacity, _level + 1};
    auto t2 = new quadtree<Point>{rectangle{{x + w / 2, y - h / 2}, children_half_dimensions}, capacity, _level + 1};
    auto t3 = new quadtree<Point>{rectangle{{x + w / 2, y + h / 2}, children_half_dimensions}, capacity, _level + 1};
    auto t4 = new quadtree<Point>{rectangle{{x - w / 2, y + h / 2}, children_half_dimensions}, capacity, _level + 1};

    _children.emplace_back(t1);
    _children.emplace_back(t2);
    _children.emplace_back(t3);
    _children.emplace_back(t4);
}

template<traits::quadtree_point Point>
inline std::vector<Point*> quadtree<Point>::quarry(const rectangle& range) const
{
    std::vector<Point*> found;
    quarry(range, found);

    return found;
}

template<traits::quadtree_point Point>
inline auto quadtree<Point>::quarry_as_range(const rectangle& range) const
{
    return utils::make_iterator_range(
        const_iterator{*this, range},
        const_iterator::make_end()
    );
}

template<traits::quadtree_point Point>
inline void quadtree<Point>::quarry(const rectangle& range, std::vector<Point*>& found) const
{
    std::stack<const quadtree<Point>*> nodes_to_visit;
    nodes_to_visit.push(this);

    while (!nodes_to_visit.empty())
    {
        auto node = nodes_to_visit.top();
        nodes_to_visit.pop();

        if (!node->_rectangle.intersects(range))
            continue;

        for (auto point : node->_points)
            if (range.contains(traits::access<Point>::position(point)))
                found.push_back(point);

        for (auto& child : node->_children)
            nodes_to_visit.push(child.get());
    }
}

template<traits::quadtree_point Point>
inline void quadtree<Point>::copy_fields(const quadtree& other)
{
    _rectangle = other._rectangle;
    _points = other._points;
    _children = other._children;
}

template<traits::quadtree_point Point>
inline void quadtree<Point>::move_fields(quadtree&& other)
{
    _rectangle = std::move(other._rectangle);
    _points = std::move(other._points);
    _children = std::move(other._children);
}

template<traits::quadtree_point Point>
auto& quadtree<Point>::box() const
{
    return this->_rectangle;
}

template<traits::quadtree_point Point>
auto& quadtree<Point>::children() const
{
    return _children;
}

template<traits::quadtree_point Point>
constexpr auto quadtree<Point>::capacity() const
{
    return _capacity;
}

template<traits::quadtree_point Point>
inline const auto& quadtree<Point>::points() const
{
    return _points;
}

template<traits::quadtree_point Point>
bool quadtree<Point>::subdivided() const
{
    return !_children.empty();
}

template<traits::quadtree_point Point>
inline bool quadtree<Point>::empty() const
{
    return _points.empty();
}

template<traits::quadtree_point Point>
inline bool quadtree<Point>::useless() const
{
    return !subdivided() && empty();
}

template<traits::quadtree_point Point>
auto& quadtree<Point>::operator=(const quadtree<Point>& other)
{
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point>
auto& quadtree<Point>::operator=(quadtree<Point>&& other) noexcept
{
    move_fields(std::move(other));

    return *this;
}

template<traits::quadtree_point Point>
quadtree<Point>::~quadtree()
{

}

#include "quadtree_const_iterator_impl.h"