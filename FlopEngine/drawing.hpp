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

    void drawLine(Vector2 a, Vector2 b);
    void drawPoint(Vector2 point, float size);
    void drawRect(Vector2 center, float halfWidth, float halfHeight);
    void drawRect(Vector2 a, Vector2 b, Vector2 c, Vector2 d);
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