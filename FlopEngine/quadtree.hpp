#pragma once
#include <vector>

#include "rectangle.hpp"

namespace traits
{

template<class Point>
struct access
{
};

template<typename T>
concept quadtree_point = requires(T* p)
{
    { access<T>::position(p) } -> std::convertible_to<vector2>;
};

}

template<traits::quadtree_point Point, size_t capacity>
class quadtree
{
private:
    rectangle _rectangle;
    std::vector<Point*> _points;
    std::vector<quadtree*> _children;

public:
    quadtree(const quadtree& other);
    quadtree(quadtree&& other);
    explicit quadtree(const rectangle& rectangle);

    void insert(std::vector<Point>& points);
    void insert(Point* point);
    void subdivide();

    std::vector<Point*> quarry(const rectangle& range) const;

    bool subdivided() const;

    auto& box() const;
    auto& children();
    auto& children() const;
    constexpr auto get_capacity() const;

    auto& operator =(const quadtree<Point, capacity>& other);
    auto& operator =(quadtree<Point, capacity>&& other) noexcept;

    ~quadtree();
private:
    void quarry(const rectangle& range, std::vector<Point*>& found) const;
    void clear_data();
    void copy_fields(const quadtree& other);
    void move_fields(quadtree&& other);
};

#include "quadtree_implementation.hpp"