#include "octree.hpp"

Octree::Octree(const Box& _box, const size_t capacity) :
    _box(_box), _capacity(capacity)
{

}

void Octree::insert(std::vector<Vector3>& points)
{
    for(auto& point : points)
    {
        insert(point);
    }
}

void Octree::insert(Vector3& point)
{
    if(!_box.contains(point))
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
    float x = _box.center.x;
    float y = _box.center.y;
    float z = _box.center.z;

    float w = _box.halfDimensions.x;
    float h = _box.halfDimensions.y;
    float d = _box.halfDimensions.z;

    Vector3 childrenHalfDimensions(w / 2, h / 2, d / 2);

    _children.reserve(8);

    _children.emplace_back(new Octree(Box({ x - w / 2, y - h / 2, z - d / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Box({ x + w / 2, y - h / 2, z - d / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Box({ x + w / 2, y + h / 2, z - d / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Box({ x - w / 2, y + h / 2, z - d / 2 }, childrenHalfDimensions), _capacity));

    _children.emplace_back(new Octree(Box({ x - w / 2, y - h / 2, z + d / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Box({ x + w / 2, y - h / 2, z + d / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Box({ x + w / 2, y + h / 2, z + d / 2 }, childrenHalfDimensions), _capacity));
    _children.emplace_back(new Octree(Box({ x - w / 2, y + h / 2, z + d / 2 }, childrenHalfDimensions), _capacity));
}

void Octree::quarry(const Box& range, std::vector<Vector3*>& found)
{
    if(!this->_box.intersects(range))
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

const Box& Octree::box() const
{
    return this->_box;
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
