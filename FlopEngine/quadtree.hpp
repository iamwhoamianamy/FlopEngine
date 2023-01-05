#pragma once
#include <vector>

#include "rectangle.hpp"

template<class Point, size_t capacity>
class Quadtree
{
private:
    Rect _rectangle;
    std::vector<Point*> _points;
    std::vector<Quadtree*> _children;

public:
    Quadtree(const Quadtree& quadtree);
    Quadtree(Quadtree&& quadtree);
    explicit Quadtree(const Rect& rectangle);

    void insert(std::vector<Point>& points);
    void insert(Point* point);
    void subdivide();

    std::vector<Point*> quarry(const Rect& range) const;

    auto& box() const;
    auto subdivided() const;
    auto& children();
    auto& children() const;
    constexpr auto get_capacity() const;

    auto& operator =(const Quadtree<Point, capacity>& quadtree);
    auto& operator =(Quadtree<Point, capacity>&& quadtree) noexcept;

    ~Quadtree();
private:
    void quarry(const Rect& range, std::vector<Point*>& found) const;
    void clearData();
    void copyFields(const Quadtree& quadtree);
    void moveFields(Quadtree&& quadtree);
};

#include "quadtree_implementation.hpp"