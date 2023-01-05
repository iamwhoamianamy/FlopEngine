#pragma once
#include "GL/freeglut.h"
#include "vector2.hpp"

namespace draw
{
    struct Color
    {
        UCHAR r;
        UCHAR g;
        UCHAR b;
        UCHAR a;

        Color(UCHAR r = 0, UCHAR g = 0, UCHAR b = 0, UCHAR a = 255);
    };
     
    inline void drawLine(Vector2 a, Vector2 b);
    inline void drawPoint(Vector2 point, float size);
    inline void drawRect(Vector2 center, float halfWidth, float halfHeight);
    inline void drawRect(Vector2 a, Vector2 b, Vector2 c, Vector2 d);
    void drawTriangle(Vector2 a, Vector2 b, Vector2 c, bool filled);
    Color generateRandomColor();

    void renderString(Vector2 position, float size, const std::string& string);
    void renderLetter(Vector2 position, float size, char letter);

    inline void setColor(GLubyte r = 255, GLubyte g = 255, GLubyte b = 255, GLubyte a = 255)
    {
        glColor4ub(r, g, b, a);
    }

    inline void setColor(Color color)
    {
        glColor4ub(color.r, color.g, color.b, color.a);
    }
}

inline void draw::drawLine(Vector2 a, Vector2 b)
{
    glBegin(GL_LINES);
    {
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
    }
    glEnd();
}

inline void draw::drawPoint(Vector2 point, float size)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

inline void draw::drawRect(Vector2 center, float halfWidth, float halfHeight)
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

inline void draw::drawRect(Vector2 a, Vector2 b, Vector2 c, Vector2 d)
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