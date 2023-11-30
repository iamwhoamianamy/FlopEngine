#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <iterator>

#include "libs/geometry/rectangle.hpp"
#include "utils/ranges.h"
#include "libs/meta/concepts.hpp"

namespace flp::concepts
{

template<typename Point>
concept quadtree_point = flp::concepts::trait_convertible_to<Point*, vector2>;

} // namespace concepts

template<flp::concepts::quadtree_point Point>
class quadtree
{
public:
    using value_t = Point;
    using node_container_iterator_t = std::vector<Point*>::const_iterator;
    using node_t = quadtree;

    static constexpr size_t max_depth = 15;

public:
    quadtree() = default;
    quadtree(const quadtree& other);
    quadtree(quadtree&& other);

    explicit quadtree(
        const rectangle& boundary_rectangle,
        size_t capacity = 2);

    virtual ~quadtree();

public:
    void insert(std::vector<Point>& points);
    void insert(Point* point);

    // cuts down nodes without points or children
    // questionable usefulness
    void commit();

    std::vector<Point*> quarry(const rectangle& range) const;
    auto quarry_as_range(const rectangle& range) const;

public:
    auto& box() const;
    auto& children() const;
    constexpr auto capacity() const;
    const auto& points() const;

    bool subdivided() const;
    bool empty() const;
    bool useless() const;

public:
    auto& operator=(const quadtree& other);
    auto& operator=(quadtree&& other) noexcept;

private:
    void quarry(const rectangle& range, std::vector<Point*>& found) const;
    void copy_fields(const quadtree& other);
    void move_fields(quadtree&& other);
    void subdivide();

private:
    explicit quadtree(
        const rectangle& boundary_rectangle,
        size_t capacity,
        size_t level);

    rectangle _rectangle{};
    std::vector<Point*> _points;
    std::vector<std::unique_ptr<node_t>> _children;
    size_t _capacity{};
    size_t _level{};

public:
    struct const_iterator
    {
    public:
        // std boilerplate :(
        using iterator_concept = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Point;
        using pointer = Point*;
        using reference = Point&;

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
