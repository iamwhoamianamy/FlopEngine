#pragma once
#include "quadtree.hpp"

static draw::Color octreeColor(255, 255, 0);

template <size_t capacity>
void drawOctree(const quadtree<vector2, capacity>& octree)
{
    draw::set_color(octreeColor);
    draw::draw_rect(octree.box().center,
        octree.box().halfDimensions.x,
        octree.box().halfDimensions.y);

    if (octree.subdivided())
    {
        for (auto child : octree.children())
        {
            drawOctree(*child);
        }
    }
}