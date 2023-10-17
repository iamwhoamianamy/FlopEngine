#pragma once
#include "libs/quadtree/quadtree.hpp"

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::quadtree(const quadtree& other)
{
    copy_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
inline quadtree<Point, Capacity>::quadtree(quadtree&& other)
{
    move_fields(other);
}

template<traits::quadtree_point Point, size_t Capacity>
quadtree<Point, Capacity>::quadtree(const rectangle& boundary_rectangle)
    : _rectangle(boundary_rectangle)
{

}

template<traits::quadtree_point Point, size_t Capacity>
void quadtree<Point, Capacity>::insert(std::vector<Point>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<traits::quadtree_point Point, size_t Capacity>
void quadtree<Point, Capacity>::insert(Point* point)
{
    if(!_rectangle.contains(traits::access<Point>::position(point)))
    {
        return;
    }

    if(_points.size() < Capacity)
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

template<traits::quadtree_point Point, size_t Capacity>
void quadtree<Point, Capacity>::subdivide()
{
    float x = _rectangle.center.x;
    float y = _rectangle.center.y;

    float w = _rectangle.half_dimensions.x;
    float h = _rectangle.half_dimensions.y;

    vector2 children_half_dimensions(w / 2, h / 2);

    _children.reserve(4);

    _children.emplace_back(std::make_unique<quadtree>(rectangle({x - w / 2, y - h / 2}, children_half_dimensions)));
    _children.emplace_back(std::make_unique<quadtree>(rectangle({x + w / 2, y - h / 2}, children_half_dimensions)));
    _children.emplace_back(std::make_unique<quadtree>(rectangle({x + w / 2, y + h / 2}, children_half_dimensions)));
    _children.emplace_back(std::make_unique<quadtree>(rectangle({x - w / 2, y + h / 2}, children_half_dimensions)));
}

template<traits::quadtree_point Point, size_t Capacity>
inline std::vector<Point*> quadtree<Point, Capacity>::quarry(const rectangle& range) const
{
    std::vector<Point*> found;
    quarry(range, found);

    return found;
}

template<traits::quadtree_point Point, size_t Capacity>
inline auto quadtree<Point, Capacity>::quarry_as_range(const rectangle& range) const
{
    return utils::make_iterator_range(
        const_iterator{*this, range},
        const_iterator::make_end()
    );
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::quarry(const rectangle& range, std::vector<Point*>& found) const
{
    std::stack<const quadtree<Point, Capacity>*> nodes_to_visit;
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

        if (node->subdivided())
            for (auto& child : node->_children)
                nodes_to_visit.push(child.get());
    }
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::copy_fields(const quadtree& other)
{
    _rectangle = other._rectangle;
    _points = other._points;
    _children = other._children;
}

template<traits::quadtree_point Point, size_t Capacity>
inline void quadtree<Point, Capacity>::move_fields(quadtree&& other)
{
    _rectangle = std::move(other._rectangle);
    _points = std::move(other._points);
    _children = std::move(other._children);
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::box() const
{
    return this->_rectangle;
}

template<traits::quadtree_point Point, size_t Capacity>
bool quadtree<Point, Capacity>::subdivided() const
{
    return _children.size();
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::children() const
{
    return _children;
}

template<traits::quadtree_point Point, size_t Capacity>
constexpr auto quadtree<Point, Capacity>::capacity() const
{
    return Capacity;
}

template<traits::quadtree_point Point, size_t Capacity>
inline const auto& quadtree<Point, Capacity>::points() const
{
    return _points;
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::operator=(const quadtree<Point, Capacity>& other)
{
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
auto& quadtree<Point, Capacity>::operator=(quadtree<Point, Capacity>&& other) noexcept
{
    move_fields(std::move(other));

    return *this;
}

template<traits::quadtree_point Point, size_t Capacity>
quadtree<Point, Capacity>::~quadtree()
{

}

#include "quadtree_const_iterator_impl.h"