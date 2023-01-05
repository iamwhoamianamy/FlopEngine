#pragma once
#include <memory>
#include "quadtree.hpp"

template<class Point>
class QuadtreePointHolder
{
public:
    static Vector2 position(Point*)
    {
        return 0;
    }
};

template<class Point, size_t capacity>
inline Quadtree<Point, capacity>::Quadtree(const Quadtree& quadtree)
{
    copyFields(quadtree);
}

template<class Point, size_t capacity>
inline Quadtree<Point, capacity>::Quadtree(Quadtree&& quadtree)
{
    moveFields(quadtree);
}

template<class Point, size_t capacity>
Quadtree<Point, capacity>::Quadtree(const Rect& rectangle) : _rectangle(rectangle)
{
}

template<class Point, size_t capacity>
void Quadtree<Point, capacity>::insert(std::vector<Point>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<class Point, size_t capacity>
void Quadtree<Point, capacity>::insert(Point* point)
{
    if(!_rectangle.contains(QuadtreePointHolder<Point>::position(point)))
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

template<class Point, size_t capacity>
void Quadtree<Point, capacity>::subdivide()
{
    float x = _rectangle.center.x;
    float y = _rectangle.center.y;

    float w = _rectangle.halfDimensions.x;
    float h = _rectangle.halfDimensions.y;

    Vector2 childrenHalfDimensions(w / 2, h / 2);

    _children.reserve(4);

    _children.emplace_back(new Quadtree(Rect({ x - w / 2, y - h / 2 }, childrenHalfDimensions)));
    _children.emplace_back(new Quadtree(Rect({ x + w / 2, y - h / 2 }, childrenHalfDimensions)));
    _children.emplace_back(new Quadtree(Rect({ x + w / 2, y + h / 2 }, childrenHalfDimensions)));
    _children.emplace_back(new Quadtree(Rect({ x - w / 2, y + h / 2 }, childrenHalfDimensions)));
}

template<class Point, size_t capacity>
inline std::vector<Point*> Quadtree<Point, capacity>::quarry(const Rect& range) const
{
    std::vector<Point*> found;
    quarry(range, found);

    return found;
}

template<class Point, size_t capacity>
void Quadtree<Point, capacity>::quarry(const Rect& range, std::vector<Point*>& found) const
{
    if(!_rectangle.intersects(range))
    {
        return;
    }
    else
    {
        for(auto point : _points)
        {
            if(range.contains(QuadtreePointHolder<Point>::position(point)))
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

template<class Point, size_t capacity>
inline void Quadtree<Point, capacity>::clearData()
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

template<class Point, size_t capacity>
inline void Quadtree<Point, capacity>::copyFields(const Quadtree& quadtree)
{
    _rectangle = quadtree._rectangle;
    _points = quadtree._points;
}

template<class Point, size_t capacity>
inline void Quadtree<Point, capacity>::moveFields(Quadtree&& quadtree)
{
    _rectangle = quadtree._rectangle;
    _points = std::move(quadtree._points);
}

template<class Point, size_t capacity>
auto& Quadtree<Point, capacity>::box() const
{
    return this->_rectangle;
}

template<class Point, size_t capacity>
auto Quadtree<Point, capacity>::subdivided() const
{
    return _children.size();
}

template<class Point, size_t capacity>
auto& Quadtree<Point, capacity>::children()
{
    return _children;
}

template<class Point, size_t capacity>
auto& Quadtree<Point, capacity>::children() const
{
    return _children;
}

template<class Point, size_t capacity>
constexpr auto Quadtree<Point, capacity>::get_capacity() const
{
    return capacity;
}

template<class Point, size_t capacity>
auto& Quadtree<Point, capacity>::operator=(const Quadtree<Point, capacity>& quadtree)
{
    clearData();
    copyFields(quadtree);

    return *this;
}

template<class Point, size_t capacity>
auto& Quadtree<Point, capacity>::operator=(Quadtree<Point, capacity>&& quadtree) noexcept
{
    clearData();
    moveFields(std::move(quadtree));

    return *this;
}

template<class Point, size_t capacity>
Quadtree<Point, capacity>::~Quadtree()
{
    clearData();
}