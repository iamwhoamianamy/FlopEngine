#pragma once
#include "quadtree.hpp"

template<class Point>
class QuadtreePointHolder
{
public:
    static float x(Point*)
    {
        return 0;
    }

    static float y(Point*)
    {
        return 0;
    }
};

template<class Point>
Quadtree<Point>::Quadtree(const Rect& rectangle, const size_t capacity) :
    _rectangle(rectangle), _capacity(capacity)
{

}

template<class Point>
void Quadtree<Point>::insert(std::vector<Point>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<class Point>
void Quadtree<Point>::insert(Point* point)
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
void Quadtree<Point>::subdivide()
{
    float x = _rectangle.center.x;
    float y = _rectangle.center.y;

    float w = _rectangle.halfDimensions.x;
    float h = _rectangle.halfDimensions.y;

    Vector2 childrenHalfDimensions(w / 2, h / 2);

    _children.reserve(4);

    _children.emplace_back(new Quadtree(Rect({ x - w / 2, y - h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Quadtree(Rect({ x + w / 2, y - h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Quadtree(Rect({ x + w / 2, y + h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Quadtree(Rect({ x - w / 2, y + h / 2 }, childrenHalfDimensions), _capacity));
}

template<class Point>
inline std::vector<Point*> Quadtree<Point>::quarry(const Rect& box)
{
    std::vector<Point*> found;
    quarry(box, found);

    return found;
}

template<class Point>
void Quadtree<Point>::quarry(const Rect& range, std::vector<Point*>& found)
{
    if(!_rectangle.intersects(range))
    {
        return;
    }
    else
    {
        for(auto point : _points)
        {
            if(range.contains(
                QuadtreePointHolder<Point>::x(point),
                QuadtreePointHolder<Point>::y(point)))
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
inline const Rect& Quadtree<Point>::box() const
{
    return this->_rectangle;
}

template<class Point>
inline const bool Quadtree<Point>::subdivided() const
{
    return _children.size();
}

template<class Point>
inline std::vector<Quadtree<Point>*>& Quadtree<Point>::children()
{
    return _children;
}

template<class Point>
inline const std::vector<Quadtree<Point>*>& Quadtree<Point>::children() const
{
    return _children;
}

template<class Point>
Quadtree<Point>::~Quadtree()
{
    for (auto child : _children)
    {
        delete child;
    }
}