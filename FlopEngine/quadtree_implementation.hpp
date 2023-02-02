#pragma once
#include <memory>
#include "quadtree.hpp"

template<traits::quadtree_point Point, size_t capacity>
inline quadtree<Point, capacity>::quadtree(const quadtree& other)
{
    copy_fields(other);
}

template<traits::quadtree_point Point, size_t capacity>
inline quadtree<Point, capacity>::quadtree(quadtree&& other)
{
    move_fields(other);
}

template<traits::quadtree_point Point, size_t capacity>
quadtree<Point, capacity>::quadtree(const rectangle& rectangle) : _rectangle(rectangle)
{
}

template<traits::quadtree_point Point, size_t capacity>
void quadtree<Point, capacity>::insert(std::vector<Point>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<traits::quadtree_point Point, size_t capacity>
void quadtree<Point, capacity>::insert(Point* point)
{
    if(!_rectangle.contains(traits::access<Point>::position(point)))
    {
        return;
    }

    if(_points.size() < capacity)
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

template<traits::quadtree_point Point, size_t capacity>
void quadtree<Point, capacity>::subdivide()
{
    float x = _rectangle.center.x;
    float y = _rectangle.center.y;

    float w = _rectangle.halfDimensions.x;
    float h = _rectangle.halfDimensions.y;

    vector2 childrenHalfDimensions(w / 2, h / 2);

    _children.reserve(4);

    _children.emplace_back(new quadtree(rectangle({ x - w / 2, y - h / 2 }, childrenHalfDimensions)));
    _children.emplace_back(new quadtree(rectangle({ x + w / 2, y - h / 2 }, childrenHalfDimensions)));
    _children.emplace_back(new quadtree(rectangle({ x + w / 2, y + h / 2 }, childrenHalfDimensions)));
    _children.emplace_back(new quadtree(rectangle({ x - w / 2, y + h / 2 }, childrenHalfDimensions)));
}

template<traits::quadtree_point Point, size_t capacity>
inline std::vector<Point*> quadtree<Point, capacity>::quarry(const rectangle& range) const
{
    std::vector<Point*> found;
    quarry(range, found);

    return found;
}

template<traits::quadtree_point Point, size_t capacity>
void quadtree<Point, capacity>::quarry(const rectangle& range, std::vector<Point*>& found) const
{
    if(!_rectangle.intersects(range))
    {
        return;
    }
    else
    {
        for(auto point : _points)
        {
            if(range.contains(traits::access<Point>::position(point)))
            {
                found.push_back(point);
            }
        }

        if(subdivided())
        {
            for(auto& child : _children)
            {
                child->quarry(range, found);
            }
        }
    }
}

template<traits::quadtree_point Point, size_t capacity>
inline void quadtree<Point, capacity>::clear_data()
{
    if (subdivided())
    {
        for (auto child : _children)
        {
            delete child;
        }

        _children.clear();
    }
}

template<traits::quadtree_point Point, size_t capacity>
inline void quadtree<Point, capacity>::copy_fields(const quadtree& other)
{
    _rectangle = other._rectangle;
    _points = other._points;
}

template<traits::quadtree_point Point, size_t capacity>
inline void quadtree<Point, capacity>::move_fields(quadtree&& other)
{
    _rectangle = other._rectangle;
    _points = std::move(other._points);
}

template<traits::quadtree_point Point, size_t capacity>
auto& quadtree<Point, capacity>::box() const
{
    return this->_rectangle;
}

template<traits::quadtree_point Point, size_t capacity>
bool quadtree<Point, capacity>::subdivided() const
{
    return _children.size();
}

template<traits::quadtree_point Point, size_t capacity>
auto& quadtree<Point, capacity>::children()
{
    return _children;
}

template<traits::quadtree_point Point, size_t capacity>
auto& quadtree<Point, capacity>::children() const
{
    return _children;
}

template<traits::quadtree_point Point, size_t capacity>
constexpr auto quadtree<Point, capacity>::get_capacity() const
{
    return capacity;
}

template<traits::quadtree_point Point, size_t capacity>
auto& quadtree<Point, capacity>::operator=(const quadtree<Point, capacity>& other)
{
    clear_data();
    copy_fields(other);

    return *this;
}

template<traits::quadtree_point Point, size_t capacity>
auto& quadtree<Point, capacity>::operator=(quadtree<Point, capacity>&& other) noexcept
{
    clear_data();
    move_fields(std::move(other));

    return *this;
}

template<traits::quadtree_point Point, size_t capacity>
quadtree<Point, capacity>::~quadtree()
{
    clear_data();
}