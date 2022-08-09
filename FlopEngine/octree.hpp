#pragma once
#include <vector>

#include "vector2.hpp"
#include "rectangle.hpp"

template<class OctreePoint>
class OctreePointHolder
{
public:
    static float x(OctreePoint*)
    {
        return 0;
    }

    static float y(OctreePoint*)
    {
        return 0;
    }
};

template<class OctreePoint>
class Octree
{
private:
    size_t _capacity;
    Rect _rectangle;
    std::vector<OctreePoint*> _points;
    std::vector<Octree*> _children;

public:
    Octree(const Rect& _Rect, const size_t capacity);

    void insert(std::vector<OctreePoint>& points);
    void insert(OctreePoint* point);
    void subdivide();
    void quarry(const Rect& box, std::vector<OctreePoint*>& found);

    inline const Rect& box() const;
    inline const bool subdivided() const;
    inline std::vector<Octree*>& children();
    inline const std::vector<Octree*>& children() const;

    ~Octree();
};

#include "octree_implementation.hpp"