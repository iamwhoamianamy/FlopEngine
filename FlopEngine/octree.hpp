#pragma once
#include <vector>

#include "vector2.hpp"
#include "rectangle.hpp"

template<class Point>
class OctreePointHolder
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
class Octree
{
private:
    size_t _capacity;
    Rect _rectangle;
    std::vector<Point*> _points;
    std::vector<Octree*> _children;

public:
    Octree(const Rect& _Rect, const size_t capacity);

    void insert(std::vector<Point>& points);
    void insert(Point* point);
    void subdivide();
    void quarry(const Rect& box, std::vector<Point*>& found);

    inline const Rect& box() const;
    inline const bool subdivided() const;
    inline std::vector<Octree*>& children();
    inline const std::vector<Octree*>& children() const;

    ~Octree();
};

#include "octree_implementation.hpp"