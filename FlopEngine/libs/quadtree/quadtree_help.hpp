#pragma once

#include "libs/quadtree/quadtree.hpp"

namespace flp
{

static draw::color quadtree_color = draw::color::yellow();

template <flp::concepts::quadtree_point Point>
inline void draw_quadtree(const quadtree<Point>& qtree)
{
    using node_t = std::remove_cvref_t<decltype(qtree)>::node_t;

    qtree.traverse_by_depth(
        [](const node_t& node)
        {
            draw::draw_rect(node.boundary());
        });
}

} // namespace flp