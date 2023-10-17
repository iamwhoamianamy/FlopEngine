#pragma once

#include "libs/quadtree/quadtree.hpp"

static draw::color quadtree_color = draw::color::yellow();

void draw_quadtree(const quadtree<vector2>& octree)
{
    draw::set_color(quadtree_color);
    draw::draw_rect(octree.box().center,
        octree.box().half_dimensions.x,
        octree.box().half_dimensions.y);

    if (octree.subdivided())
    {
        for (const auto& child : octree.children())
        {
            draw_quadtree(*child);
        }
    }
}