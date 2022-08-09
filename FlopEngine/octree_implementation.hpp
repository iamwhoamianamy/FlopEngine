#pragma once
#include "octree.hpp"

template<class Point>
Octree<Point>::Octree(const Rect& _Rect, const size_t capacity) :
    _rectangle(_Rect), _capacity(capacity)
{

}

template<class Point>
void Octree<Point>::insert(std::vector<Point>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<class Point>
void Octree<Point>::insert(Point* point)
{
    if(!_rectangle.contains(*point))
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

template<class Point>
void Octree<Point>::subdivide()
{
    float x = _rectangle.center.x;
    float y = _rectangle.center.y;

    float w = _rectangle.halfDimensions.x;
    float h = _rectangle.halfDimensions.y;

    Vector2 childrenHalfDimensions(w / 2, h / 2);

    _children.reserve(4);

    _children.emplace_back(new Octree(Rect({ x - w / 2, y - h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Rect({ x + w / 2, y - h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Rect({ x + w / 2, y + h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Rect({ x - w / 2, y + h / 2 }, childrenHalfDimensions), _capacity));
}

template<class Point>
void Octree<Point>::quarry(const Rect& range, std::vector<Point*>& found)
{
    if(!_rectangle.intersects(range))
    {
        return;
    }
    else
    {
        for(auto point : _points)
        {
            if(range.contains({
                OctreePointHolder<Point>::x(point),
                OctreePointHolder<Point>::y(point) }))
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

template<class Point>
inline const Rect& Octree<Point>::box() const
{
    return this->_rectangle;
}

template<class Point>
inline const bool Octree<Point>::subdivided() const
{
    return _children.size();
}

template<class Point>
inline std::vector<Octree<Point>*>& Octree<Point>::children()
{
    return _children;
}

template<class Point>
inline const std::vector<Octree<Point>*>& Octree<Point>::children() const
{
    return _children;
}

template<class Point>
Octree<Point>::~Octree()
{
    if(subdivided())
    {
        for(auto child : _children)
        {
            delete child;
        }
    }
}