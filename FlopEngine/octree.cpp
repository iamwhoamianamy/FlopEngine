#include "octree.hpp"

Octree::Octree(const Rect& _Rect, const size_t capacity) :
    _Rect(_Rect), _capacity(capacity)
{

}

void Octree::insert(std::vector<Vector2>& points)
{
    for(auto& point : points)
    {
        insert(point);
    }
}

void Octree::insert(Vector2& point)
{
    if(!_Rect.contains(point))
    {
        return;
    }

    if(_points.size() < _capacity)
    {
        if(!subdivided())
        {
            _points.push_back(&point);
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

        _points.push_back(&point);

        for(auto p : _points)
        {
            for(auto& child : _children)
            {
                child->insert(*p);
            }
        }

        _points.clear();
        _points.shrink_to_fit();
    }
}

void Octree::subdivide()
{
    float x = _Rect.center.x;
    float y = _Rect.center.y;

    float w = _Rect.halfDimensions.x;
    float h = _Rect.halfDimensions.y;

    Vector2 childrenHalfDimensions(w / 2, h / 2);

    _children.reserve(4);

    _children.emplace_back(new Octree(Rect({ x - w / 2, y - h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Rect({ x + w / 2, y - h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Rect({ x + w / 2, y + h / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Rect({ x - w / 2, y + h / 2 }, childrenHalfDimensions), _capacity));
}

void Octree::quarry(const Rect& range, std::vector<Vector2*>& found)
{
    if(!this->_Rect.intersects(range))
    {
        return;
    }
    else
    {
        for(auto point : _points)
        {
            if(range.contains(*point))
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

const Rect& Octree::box() const
{
    return this->_Rect;
}

const bool Octree::subdivided() const
{
    return _children.size();
}

const std::vector<Octree*> Octree::children() const
{
    return _children;
}

Octree::~Octree()
{
    if(subdivided())
    {
        for(auto child : _children)
        {
            delete child;
        }
    }
}