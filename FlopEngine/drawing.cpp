#include "drawing.hpp"
#include "math.hpp"

namespace draw
{
    Color::Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a) :
        r(r), g(g), b(b), a(a)
    {
    }

    void drawLine(Vector2 a, Vector2 b)
    {
        glBegin(GL_LINES);
        {
            glVertex2f(a.x, a.y);
            glVertex2f(b.x, b.y);
        }
        glEnd();
    }

    void drawPoint(Vector2 point, float size)
    {
        glPointSize(size);
        glBegin(GL_POINTS);
        {
            glVertex2f(point.x, point.y);
        }
        glEnd();
    }

    void drawRect(Vector2 center, float halfWidth, float halfHeight)
    {
        glBegin(GL_LINE_LOOP);
        {
            glVertex2f(center.x - halfWidth, center.y - halfHeight);
            glVertex2f(center.x + halfWidth, center.y - halfHeight);
            glVertex2f(center.x + halfWidth, center.y + halfHeight);
            glVertex2f(center.x - halfWidth, center.y + halfHeight);
        }
        glEnd();
    }

    void drawRect(Vector2 a, Vector2 b, Vector2 c, Vector2 d)
    {
        glBegin(GL_LINE_LOOP);
        {
            glVertex3f(a.x, a.y, 0);
            glVertex3f(b.x, b.y, 0);
            glVertex3f(c.x, c.y, 0);
            glVertex3f(d.x, d.y, 0);
        }
        glEnd();
    }
    
    void drawTriangle(Vector2 a, Vector2 b, Vector2 c, bool filled)
    {
        if(filled)
        {
            glBegin(GL_TRIANGLES);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        {
            glVertex3f(a.x, a.y, 0);
            glVertex3f(b.x, b.y, 0);
            glVertex3f(c.x, c.y, 0);
        }
        glEnd();
    }

    Color generateRandomColor()
    {
        return Color(
            math::randomInRange(0, 255),
            math::randomInRange(0, 255),
            math::randomInRange(0, 255));
    }
}
