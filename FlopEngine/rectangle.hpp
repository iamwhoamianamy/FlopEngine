#pragma once
#include "vector2.hpp"
#include "math.hpp"

struct rectangle_t
{
    vector2 center;
    vector2 halfDimensions;

    rectangle_t();
    rectangle_t(const vector2& center, const vector2& halfDimensions);
    rectangle_t(const vector2& center, float radius);

    inline bool contains(float x, float y) const;
    inline bool contains(const vector2& point) const;
    inline bool intersects(const rectangle_t& _rectangle) const;
};

bool rectangle_t::contains(float x, float y) const
{
    return
        math::is_in_range(x, center.x, halfDimensions.x) &&
        math::is_in_range(y, center.y, halfDimensions.y);
}

inline bool rectangle_t::contains(const vector2& point) const
{
    return contains(point.x, point.y);
}

bool rectangle_t::intersects(const rectangle_t& _Rect) const
{
    return rectangle_t(_Rect.center, halfDimensions + _Rect.halfDimensions).contains(center);
}