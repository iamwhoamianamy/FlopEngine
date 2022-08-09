#pragma once
#include <vector>

#include "vector2.hpp"
#include "rectangle.hpp"
#include "i_octree_storable.hpp"

//template<class ElemHolder>
class Octree
{
private:
    size_t _capacity;
    Rect _Rect;
    std::vector<Vector2*> _points;
    std::vector<Octree*> _children;

public:
    Octree(const Rect& _Rect, const size_t capacity);

    void insert(Vector2& point);
    void insert(std::vector<Vector2>& points);
    void subdivide();
    void quarry(const Rect& box, std::vector<Vector2*>& found);

    const Rect& box() const;
    const bool subdivided() const;
    const std::vector<Octree*> children() const;

    ~Octree();
};
