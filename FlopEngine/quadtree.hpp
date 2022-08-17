#pragma once
#include <vector>

#include "vector2.hpp"
#include "rectangle.hpp"

template<class Point>
class Quadtree
{
private:
    size_t _capacity;
    Rect _rectangle;
    std::vector<Point*> _points;
    std::vector<Quadtree*> _children;

public:
    Quadtree(const Quadtree& quadtree);
    Quadtree(Quadtree&& quadtree);
    Quadtree(const Rect& rectangle, const size_t capacity = 1);

    void insert(std::vector<Point>& points);
    void insert(Point* point);
    void subdivide();

    std::vector<Point*> quarry(const Rect& range) const;

    inline const Rect& box() const;
    inline const bool subdivided() const;
    inline std::vector<Quadtree*>& children();
    inline const std::vector<Quadtree*>& children() const;

    Quadtree<Point>& operator =(const Quadtree<Point>& quadtree);
    Quadtree<Point>& operator =(Quadtree<Point>&& quadtree);

    ~Quadtree();
private:
    void quarry(const Rect& range, std::vector<Point*>& found) const;
    void clearData();
    void copyFields(const Quadtree& quadtree);
    void moveFields(Quadtree&& quadtree);
};

#include "quadtree_implementation.hpp"