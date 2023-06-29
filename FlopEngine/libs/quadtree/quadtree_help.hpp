#pragma once

#include "libs/quadtree/quadtree.hpp"

static draw::color octreeColor(255, 255, 0);

template <size_t capacity>
void drawOctree(const quadtree<vector2, capacity>& octree)
{
    draw::set_color(octreeColor);
    draw::draw_rect(octree.box().center,
        octree.box().half_dimensions.x,
        octree.box().half_dimensions.y);

    if (octree.subdivided())
    {
        for (auto child : octree.children())
        {
            drawOctree(*child);
        }
    }
}