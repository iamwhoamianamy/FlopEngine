#pragma once
#include "octree.hpp"

template<class OctreePoint>
Octree<OctreePoint>::Octree(const Rect& _Rect, const size_t capacity) :
    _rectangle(_Rect), _capacity(capacity)
{

}

template<class OctreePoint>
void Octree<OctreePoint>::insert(std::vector<OctreePoint>& points)
{
    for(auto& point : points)
    {
        insert(&point);
    }
}

template<class OctreePoint>
void Octree<OctreePoint>::insert(OctreePoint* point)
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

template<class OctreePoint>
void Octree<OctreePoint>::subdivide()
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

template<class OctreePoint>
void Octree<OctreePoint>::quarry(const Rect& range, std::vector<OctreePoint*>& found)
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
                OctreePointHolder<OctreePoint>::x(point),
                OctreePointHolder<OctreePoint>::y(point) }))
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

template<class OctreePoint>
inline const Rect& Octree<OctreePoint>::box() const
{
    return this->_rectangle;
}

template<class OctreePoint>
inline const bool Octree<OctreePoint>::subdivided() const
{
    return _children.size();
}

template<class OctreePoint>
inline std::vector<Octree<OctreePoint>*>& Octree<OctreePoint>::children()
{
    return _children;
}

template<class OctreePoint>
inline const std::vector<Octree<OctreePoint>*>& Octree<OctreePoint>::children() const
{
    return _children;
}

template<class OctreePoint>
Octree<OctreePoint>::~Octree()
{
    if(subdivided())
    {
        for(auto child : _children)
        {
            delete child;
        }
    }
}