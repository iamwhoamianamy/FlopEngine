#include "drawing.hpp"
#include "quadtree.hpp"

draw::Color octreeColor(255, 255, 0);

void drawOctree(const Quadtree<Vector2>& octree)
{
    draw::setColor(octreeColor);
    draw::drawRect(octree.box().center,
        octree.box().halfDimensions.x,
        octree.box().halfDimensions.y);

    if(octree.subdivided())
    {
        for(auto child : octree.children())
        {
            drawOctree(*child);
        }
    }
}