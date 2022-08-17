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

    void drawPoint(Vector2 point, Color color, float size);
    void drawRect(Vector2 center, float halfWidth, float halfHeight, Color color);
    void drawRect(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Color color);
}