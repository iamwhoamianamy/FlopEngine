#include "drawing.hpp"

namespace drawing
{
    Color::Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a) :
        r(r), g(g), b(b), a(a)
    {
    }

    void drawPoint(Vector2 point, Color color, float size)
    {
        glColor3ub(color.r, color.g, color.b);
        glPointSize(size);
        glBegin(GL_POINTS);
        {
            glVertex2f(point.x, point.y);
        }
        glEnd();
    }

    void drawRect(Vector2 center, float halfWidth, float halfHeight, Color color)
    {
        glColor3ub(color.r, color.g, color.b);
        glBegin(GL_LINE_LOOP);
        {
            glVertex2f(center.x - halfWidth, center.y - halfHeight);
            glVertex2f(center.x + halfWidth, center.y - halfHeight);
            glVertex2f(center.x + halfWidth, center.y + halfHeight);
            glVertex2f(center.x - halfWidth, center.y + halfHeight);
        }
        glEnd();
    }

    void drawRect(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Color color)
    {
        glColor3ub(color.r, color.g, color.b);
        glBegin(GL_LINE_LOOP);
        {
            glVertex3f(a.x, a.y, 0);
            glVertex3f(b.x, b.y, 0);
            glVertex3f(c.x, c.y, 0);
            glVertex3f(d.x, d.y, 0);
        }
        glEnd();
    }

    Color octreeColor(255, 255, 0);

    void drawOctree(const Octree& octree)
    {
        drawRect(octree.box().center,
            octree.box().halfDimensions.x,
            octree.box().halfDimensions.y, octreeColor);

        if(octree.subdivided())
        {
            for(auto child : octree.children())
            {
                drawOctree(*child);
            }
        }
    }
}
