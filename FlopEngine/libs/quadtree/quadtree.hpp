#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <iterator>

#include "libs/geometry/rectangle.hpp"
#include "utils/ranges.h"

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

template<traits::quadtree_point Point, size_t Capacity>
class quadtree
{
public:
    using value_t = Point;
    using node_container_iterator_t = std::vector<Point*>::const_iterator;

public:
    quadtree(const quadtree& other);
    quadtree(quadtree&& other);
    explicit quadtree(const rectangle& boundary_rectangle);

    void insert(std::vector<Point>& points);
    void insert(Point* point);
    void subdivide();

    std::vector<Point*> quarry(const rectangle& range) const;
    auto quarry_as_range(const rectangle& range) const;

    bool subdivided() const;

    auto& box() const;
    auto& children();
    auto& children() const;
    constexpr auto get_capacity() const;
    const auto& points() const;

    auto& operator =(const quadtree<Point, Capacity>& other);
    auto& operator =(quadtree<Point, Capacity>&& other) noexcept;

    ~quadtree();

private:
    void quarry(const rectangle& range, std::vector<Point*>& found) const;
    void clear_data();
    void copy_fields(const quadtree& other);
    void move_fields(quadtree&& other);

private:
    rectangle _rectangle;
    std::vector<Point*> _points;
    std::vector<quadtree*> _children;

public:
    struct const_iterator
    {
    public:
        using iterator_concept = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Point*;
        using pointer = Point**;
        using reference = Point*&;

    public:
        const_iterator();
        const_iterator(const const_iterator& other);
        const_iterator(const_iterator&& other) noexcept;
        const_iterator(const quadtree* root, const rectangle* range);

    public:
        const Point& operator->() const;
        const Point& operator*() const;
        const const_iterator& operator++() const;
        const const_iterator& operator++(int) const;

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

        const_iterator& operator=(const const_iterator& other);
        const_iterator& operator=(const_iterator&& other) noexcept;
        
    public:
        static const_iterator make_end();

    private:
        void find_first() const;
        void find_next() const;
        void find_next_impl() const;

        void copy_fields(const const_iterator& other);

    private:
        struct control_block
        {
            const quadtree* root = nullptr;
            const quadtree* node = nullptr;
            node_container_iterator_t node_iterator;
            const rectangle* range = nullptr;
            std::stack<const quadtree*> nodes_to_visit;
            bool is_end = true;

            control_block();
            control_block(const quadtree* root, const rectangle* range);
        };

        std::shared_ptr<control_block> _cb;
    };
};

//template<traits::quadtree_point Point, size_t Capacity>
//auto begin(utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range);
//
//template<traits::quadtree_point Point, size_t Capacity>
//auto end(utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range);
//
//template<traits::quadtree_point Point, size_t Capacity>
//auto begin(const utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range);
//
//template<traits::quadtree_point Point, size_t Capacity>
//auto end(const utils::iterator_range<typename quadtree<Point, Capacity>::const_iterator>& range);

#include "quadtree_implementation.hpp"
