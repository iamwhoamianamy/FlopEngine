#pragma once

#include "libs/geometry/vector2.hpp"
#include "libs/math/math.hpp"

struct rectangle
{
    vector2 center;
    vector2 half_dimensions;

    constexpr rectangle() noexcept;
    constexpr rectangle(const vector2& center, const vector2& half_dimensions)      noexcept;
    constexpr rectangle(const vector2& center, float radius)                        noexcept;
    constexpr rectangle(const vector2& center, float half_width, float half_height) noexcept;

    constexpr rectangle(const rectangle&) noexcept = default;
    constexpr rectangle(rectangle&&)      noexcept = default;

    constexpr rectangle& operator=(const rectangle&) noexcept = default;
    constexpr rectangle& operator=(rectangle&&)      noexcept = default;

    constexpr bool contains(float x, float y)     const noexcept;
    constexpr bool contains(const vector2& point) const noexcept;

    constexpr bool intersects(const rectangle& other) const noexcept;

    constexpr float left()  const noexcept;
    constexpr float right() const noexcept;
    constexpr float top()   const noexcept;
    constexpr float bot()   const noexcept;

    constexpr float width()  const noexcept;
    constexpr float height() const noexcept;

    constexpr vector2 top_left()  const noexcept;
    constexpr vector2 top_right() const noexcept;
    constexpr vector2 bot_left()  const noexcept;
    constexpr vector2 bot_right() const noexcept;

    float diagonal() const;
};

constexpr rectangle::rectangle() noexcept
    : center{0, 0}
    , half_dimensions{0, 0}
{

}

constexpr rectangle::rectangle(const vector2& center, const vector2& half_dimensions) noexcept
    : center{center}
    , half_dimensions{half_dimensions}
{

}

constexpr rectangle::rectangle(const vector2& center, float radius) noexcept
    : center{center}
    , half_dimensions{radius, radius}
{

}

constexpr rectangle::rectangle(const vector2& center, float half_width, float half_height) noexcept
    : center{center}
    , half_dimensions{half_width, half_height}
{

}

constexpr bool rectangle::contains(float x, float y) const noexcept
{
    return
        math::is_in_range(x, center.x, half_dimensions.x) &&
        math::is_in_range(y, center.y, half_dimensions.y);
}

constexpr bool rectangle::contains(const vector2& point) const noexcept
{
    return contains(point.x, point.y);
}

constexpr bool rectangle::intersects(const rectangle& other) const noexcept
{
    return 
        left() < other.right() && other.left() < right() &&
        bot() < other.top() && other.bot() < top();
}

constexpr float rectangle::left() const noexcept
{
    return center.x - half_dimensions.x;
}

constexpr float rectangle::right() const noexcept
{
    return center.x + half_dimensions.x;
}

constexpr float rectangle::top() const noexcept
{
    return center.y + half_dimensions.y;
}

constexpr float rectangle::bot() const noexcept
{
    return center.y - half_dimensions.y;
}

constexpr float rectangle::width() const noexcept
{
    return 2.0f * half_dimensions.x;
}

constexpr float rectangle::height() const noexcept
{
    return 2.0f * half_dimensions.y;
}

inline constexpr vector2 rectangle::top_left() const noexcept
{
    return {left(), top()};
}

inline constexpr vector2 rectangle::top_right() const noexcept
{
    return {right(), top()};
}

inline constexpr vector2 rectangle::bot_left() const noexcept
{
    return {left(), bot()};
}

inline constexpr vector2 rectangle::bot_right() const noexcept
{
    return {right(), bot()};
}

inline float rectangle::diagonal() const
{
    return 2.0f * std::sqrtf(
        half_dimensions.x * half_dimensions.x + 
        half_dimensions.y * half_dimensions.y);
}
