#pragma once

#include "libs/geometry/vector2.hpp"
#include "libs/math/math.hpp"

struct rectangle
{
    vector2 center;
    vector2 half_dimensions;

    rectangle();
    rectangle(const vector2& center, const vector2& half_dimensions);
    rectangle(const vector2& center, float radius);
    rectangle(const vector2& center, float half_width, float half_height);

    bool contains(float x, float y) const;
    bool contains(const vector2& point) const;
    bool intersects(const rectangle& other) const;

    float left() const;
    float right() const;
    float top() const;
    float bot() const;

    float width() const;
    float height() const;
    float diagonal() const;
};

inline bool rectangle::contains(float x, float y) const
{
    return
        math::is_in_range(x, center.x, half_dimensions.x) &&
        math::is_in_range(y, center.y, half_dimensions.y);
}

inline bool rectangle::contains(const vector2& point) const
{
    return contains(point.x, point.y);
}

inline bool rectangle::intersects(const rectangle& other) const
{
    return rectangle(other.center, half_dimensions + other.half_dimensions).contains(center);
}

inline float rectangle::left() const
{
    return center.x - half_dimensions.x;
}

inline float rectangle::right() const
{
    return center.x + half_dimensions.x;
}

inline float rectangle::top() const
{
    return center.y + half_dimensions.y;
}

inline float rectangle::bot() const
{
    return center.y - half_dimensions.y;
}

inline float rectangle::width() const
{
    return 2 * half_dimensions.x;
}

inline float rectangle::height() const
{
    return 2 * half_dimensions.y;
}

inline float rectangle::diagonal() const
{
    return 2 * std::sqrtf(
        half_dimensions.x * half_dimensions.x + 
        half_dimensions.y * half_dimensions.y);
}
