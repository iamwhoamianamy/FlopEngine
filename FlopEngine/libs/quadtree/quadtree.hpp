#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <iterator>

#include "libs/geometry/rectangle.hpp"
#include "utils/ranges.hpp"
#include "libs/meta/concepts.hpp"

namespace flp
{

namespace concepts
{

template<typename Point>
concept quadtree_point = flp::concepts::trait_convertible_to<Point*, vector2>;

// WIP
template<typename Quadtree>
concept quadtree = requires(Quadtree qtree)
{
    //qtree.quarry(std::declval<geo::rectangle>());
    typename Quadtree::point_t;
};

// WIP
template<typename Node>
concept quadtree_node = requires(Node node)
{
    typename Node::point_t;
};

} // namespace concepts

template<typename Derived, concepts::quadtree_point Point>
struct quadtree_node_base
{
public:
    using me_t                 = Derived;
    using point_t              = Point;
    using point_container_t    = std::vector<point_t*>;
    using children_container_t = std::vector<std::unique_ptr<me_t>>;

public:
    static constexpr const size_t max_depth = 15;
    
public:
    explicit quadtree_node_base(
        const geo::rectangle& boundary,
        size_t capacity);

public:
    quadtree_node_base(const quadtree_node_base&)     = delete;
    quadtree_node_base(quadtree_node_base&&) noexcept = delete;

    quadtree_node_base& operator=(const quadtree_node_base&)     = delete;
    quadtree_node_base& operator=(quadtree_node_base&&) noexcept = delete;

public:
    void insert(point_t* point);

public:
    const geo::rectangle& boundary() const;
    const auto&           children() const;
    const auto&           points()   const;

    size_t capacity() const;
    size_t level()    const;

    bool subdivided() const;
    bool empty()      const;
    bool useless()    const;

protected:
    void subdivide();

protected:
    explicit quadtree_node_base(
        const geo::rectangle& boundary,
        size_t capacity,
        size_t level);

protected:
    geo::rectangle       _boundary{};

    size_t               _capacity{};
    size_t               _level{};

    point_container_t    _points;
    children_container_t _children;
};

template<concepts::quadtree_point Point>
struct quadtree_node final : public quadtree_node_base<quadtree_node<Point>, Point>
{
    using quadtree_node_base<quadtree_node<Point>, Point>::quadtree_node_base;
};

template<concepts::quadtree_point Point, typename Data>
struct quadtree_node_with_data final
    : public quadtree_node_base<quadtree_node_with_data<Point, Data>, Point>
{
    using quadtree_node_base<quadtree_node_with_data<Point, Data>, Point>::quadtree_node_base;

    mutable Data data{};
};

template<concepts::quadtree Quadtree>
struct quadtree_iterator final
{
public:
    using me_t    = quadtree_iterator<Quadtree>;
    using node_t  = Quadtree::node_t;
    using point_t = Quadtree::point_t;

    using current_node_t    = const node_t*;
    using current_point_t   = std::vector<point_t*>::const_iterator;
    using visit_container_t = std::stack<const node_t*>;

public:
    // std boilerplate :(
    using iterator_concept  = std::input_iterator_tag;
    using iterator_category = std::input_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using point_type        = point_t;
    using pointer           = point_t*;
    using reference         = point_t&;

public:
    quadtree_iterator() = default;
    quadtree_iterator(const node_t* node, const geo::rectangle& range);

public:
    ~quadtree_iterator() = default;

    quadtree_iterator(const quadtree_iterator&)     = default;
    quadtree_iterator(quadtree_iterator&&) noexcept = default;

    quadtree_iterator& operator=(const quadtree_iterator&)     = default;
    quadtree_iterator& operator=(quadtree_iterator&&) noexcept = default;

public:
    reference operator*() const;
    pointer operator->() const;

    auto& operator++();
    auto operator++(int);

    friend bool operator==(const quadtree_iterator& a, const quadtree_iterator& b)
    {
        return a.equal(b);
    }

    friend bool operator!=(const quadtree_iterator& a, const quadtree_iterator& b)
    {
        return !a.equal(b);
    }

public:
    static quadtree_iterator make_end();

private:
    bool equal(const quadtree_iterator& other) const;

private:
    void find_first();
    void find_next();
    void find_next_impl();

    void copy_fields(const quadtree_iterator& other);

private:
    struct control_block
    {
        current_node_t        current_node = nullptr;
        current_point_t       current_point;
        const geo::rectangle& range;
        visit_container_t     nodes_to_visit;
        bool                  is_end = true;

        control_block(const node_t* node, const geo::rectangle& range);
    };

    std::shared_ptr<control_block> _cb;
};

template<
    concepts::quadtree_point Point = vector2,
    concepts::quadtree_node Node = quadtree_node<Point>>
struct quadtree final
{
public:
    using me_t       = quadtree<Point, Node>;
    using point_t    = Point;
    using node_t     = Node;
    using iterator_t = quadtree_iterator<me_t>;

public:
    static constexpr const size_t default_capacity = 2;

public:
    quadtree() = default;

    explicit quadtree(
        const geo::rectangle& boundary,
        size_t capacity = default_capacity);

public:
    ~quadtree() = default;

    quadtree(const quadtree&)     = default;
    quadtree(quadtree&&) noexcept = default;

    quadtree& operator=(const quadtree&)     = default;
    quadtree& operator=(quadtree&&) noexcept = default;

public:
    void insert(std::vector<Point>& points);

    // cuts down nodes without points or children
    // questionable usefulness
    void commit();

    auto quarry(const geo::rectangle& range) const;

    void traverse_by_depth(std::invocable<const node_t*> auto&& visitor) const;
    void traverse_by_width(std::invocable<const node_t*> auto&& visitor) const;

    void traverse_by_width_reverse(std::invocable<const node_t*> auto&& visitor) const;

private:
    std::unique_ptr<node_t> _head;

private:
    friend struct iterator_t;
};

} // namespace flp

#include "quadtree_impl.hpp"
