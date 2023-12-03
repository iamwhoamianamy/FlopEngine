#pragma once

#include "libs/quadtree/quadtree.hpp"

namespace flp
{

static draw::color quadtree_color = draw::color::yellow();

template <flp::concepts::quadtree_point Point>
inline void draw_quadtree(const quadtree<Point>& qtree)
{
    draw::draw_rect(qtree.box().center,
        qtree.box().half_dimensions.x,
        qtree.box().half_dimensions.y);

    if (qtree.subdivided())
    {
        for (const auto& child : qtree.children())
        {
            draw_quadtree(*child);
        }
    }
}

} // namespace flp