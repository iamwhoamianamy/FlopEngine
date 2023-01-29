#pragma once
#include "vector2.hpp"

struct rectangle
{
    vector2 center;
    vector2 halfDimensions;

    rectangle();
    rectangle(const vector2& center, const vector2& halfDimensions);
    rectangle(const vector2& center, float radius);
    bool contains(float x, float y) const;
    inline bool contains(const vector2& point) const;
    bool intersects(const rectangle& _rectangle) const;
};