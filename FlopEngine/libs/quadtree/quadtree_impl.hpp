#pragma once

#include "quadtree.hpp"

#include "utils/utils.hpp"

namespace flp
{
template<
    flp::concepts::quadtree_point Point,
    flp::concepts::quadtree_node Node>
inline quadtree<Point, Node>::quadtree(
    const geo::rectangle& boundary,
    size_t capacity)
    : _head{std::make_unique<node_t>(boundary, capacity)}
{

}

template<
    flp::concepts::quadtree_point Point,
    flp::concepts::quadtree_node Node>
inline void quadtree<Point, Node>::insert(std::vector<Point>& points)
{
    for (auto& point : points)
    {
        _head->insert(&point);
    }
}

template<
    flp::concepts::quadtree_point Point,
    flp::concepts::quadtree_node Node>
inline void quadtree<Point, Node>::commit()
{
    //std::stack<node_t*> nodes_to_visit;
    //nodes_to_visit.push(const_cast<node_t*>(this));

    //while (!nodes_to_visit.empty())
    //{
    //    const auto& node = nodes_to_visit.top();
    //    nodes_to_visit.pop();

    //    std::vector<std::unique_ptr<node_t>> new_children;

    //    for (auto i : utils::iota(node->_children.size()))
    //    {
    //        if (!node->_children[i]->useless())
    //        {
    //            new_children.emplace_back(std::move(node->_children[i]));
    //        }
    //    }

    //    node->_children = std::move(new_children);

    //    for (const auto& child : node->_children)
    //        nodes_to_visit.push(child.get());
    //}
}

template<
    flp::concepts::quadtree_point Point,
    flp::concepts::quadtree_node Node>
inline auto quadtree<Point, Node>::quarry(
    const geo::rectangle& range) const
{
    return utils::make_iterator_range(
        iterator_t{_head.get(), range},
        iterator_t::make_end()
    );
}

} // namespace flp

#include "quadtree_iterator_impl.hpp"
#include "quadtree_node_impl.hpp"