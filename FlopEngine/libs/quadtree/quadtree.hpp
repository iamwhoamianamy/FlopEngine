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
    using node_t = quadtree;

public:
    quadtree(const quadtree& other);
    quadtree(quadtree&& other);
    explicit quadtree(const rectangle& boundary_rectangle);

    virtual ~quadtree();

public:
    void insert(std::vector<Point>& points);
    void insert(Point* point);

    std::vector<Point*> quarry(const rectangle& range) const;
    auto quarry_as_range(const rectangle& range) const;

public:
    bool subdivided() const;
    auto& box() const;
    auto& children() const;
    constexpr auto capacity() const;
    const auto& points() const;

public:
    auto& operator=(const quadtree& other);
    auto& operator=(quadtree&& other) noexcept;

private:
    void quarry(const rectangle& range, std::vector<Point*>& found) const;
    void copy_fields(const quadtree& other);
    void move_fields(quadtree&& other);
    void subdivide();

private:
    rectangle _rectangle;
    std::vector<Point*> _points;
    std::vector<std::unique_ptr<node_t>> _children;

public:
    struct const_iterator
    {
    public:
        // std boilerplate :(
        using iterator_concept = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Point;
        using pointer = const Point*;
        using reference = const Point&;

    public:
        const_iterator();
        const_iterator(const const_iterator& other);
        const_iterator(const_iterator&& other) noexcept;
        const_iterator(const quadtree& root, const rectangle& range);

        ~const_iterator();

    public:
        reference operator*() const;
        pointer operator->() const;

        auto& operator++();
        auto operator++(int);

        friend bool operator==(const const_iterator& a, const const_iterator& b)
        {
            return a.equal(b);
        }

        friend bool operator!=(const const_iterator& a, const const_iterator& b)
        {
            return !a.equal(b);
        }

        auto& operator=(const const_iterator& other);
        auto& operator=(const_iterator&& other) noexcept;
        
    public:
        static const_iterator make_end();

    private:
        bool equal(const const_iterator& other) const;

    private:
        void find_first();
        void find_next();
        void find_next_impl();

        void copy_fields(const const_iterator& other);

    private:
        struct control_block
        {
            const node_t* node = nullptr;
            node_container_iterator_t node_iterator;
            const rectangle& range;
            std::stack<const node_t*> nodes_to_visit;
            bool is_end = true;

            control_block(const quadtree& root, const rectangle& range);
        };

        std::shared_ptr<control_block> _cb;
    };
};

#include "quadtree_implementation.hpp"
