#pragma once
#include "vector2.hpp"

struct Rect
{
    Vector2 center;
    Vector2 halfDimensions;

    Rect();
    Rect(const Vector2& center, const Vector2& halfDimensions);
    Rect(const Vector2& center, float radius);
    bool contains(float x, float y) const;
    inline bool contains(const Vector2& point) const;
    bool intersects(const Rect& _rectangle) const;
};