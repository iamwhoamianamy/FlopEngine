#pragma once
#include "vector2.hpp"
#include "math.hpp"

struct rectangle
{
    vector2 center;
    vector2 half_dimensions;

    rectangle();
    rectangle(const vector2& center, const vector2& half_dimensions);
    rectangle(const vector2& center, float radius);

    inline bool contains(float x, float y) const;
    inline bool contains(const vector2& point) const;
    inline bool intersects(const rectangle& other) const;
};

bool rectangle::contains(float x, float y) const
{
    return
        math::is_in_range(x, center.x, half_dimensions.x) &&
        math::is_in_range(y, center.y, half_dimensions.y);
}

inline bool rectangle::contains(const vector2& point) const
{
    return contains(point.x, point.y);
}

bool rectangle::intersects(const rectangle& other) const
{
    return rectangle(other.center, half_dimensions + other.half_dimensions).contains(center);
}