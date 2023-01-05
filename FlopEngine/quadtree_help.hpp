#pragma once
#include "quadtree.hpp"

static draw::Color octreeColor(255, 255, 0);

template <size_t capacity>
void drawOctree(const Quadtree<Vector2, capacity>& octree)
{
    draw::setColor(octreeColor);
    draw::drawRect(octree.box().center,
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