#pragma once

#include <ostream>
#include <iomanip>

// questionable
namespace math
{

inline bool close_enough(float a, float b) noexcept;

} // namespace math

namespace flp::geometry
{

struct vector2;

struct vector2
{
    float x;
    float y;

    constexpr vector2(const vector2& vector)    noexcept;
    constexpr vector2(vector2&& vector)         noexcept;
    constexpr vector2(float x = 0, float y = 0) noexcept;

    float& operator[](int i);
    constexpr float operator[](int i) const noexcept;

    constexpr vector2& operator=(vector2&& vector)      noexcept;
    constexpr vector2& operator=(const vector2& vector) noexcept;

    constexpr vector2 operator +(const vector2& rhs) const noexcept;
    constexpr vector2 operator -(const vector2& rhs) const noexcept;
    constexpr vector2 operator *(const float fac)    const noexcept;
    constexpr vector2 operator /(const float fac)    const noexcept;

    constexpr vector2& operator +=(const vector2& rhs) noexcept;
    constexpr vector2& operator -=(const vector2& rhs) noexcept;
    constexpr vector2& operator *=(float fac)          noexcept;
    constexpr vector2& operator /=(float fac)          noexcept;

    float   length()         const;
    float   length_squared() const;
    vector2 normalized()     const;

    constexpr vector2 perp() const noexcept;
    constexpr void    zero() noexcept;

    void normalize ();
    void limit     (float max_length);
    void set_length(float new_length);

    static vector2 direction       (const vector2& from, const vector2& to);
    static float   distance        (const vector2& a, const vector2& b);
    static float   distance_squared(const vector2& a, const vector2& b);

    constexpr static vector2 lerp (const vector2& a, const vector2& b, float factor) noexcept;
    constexpr static float   dot  (const vector2& a, const vector2& b) noexcept;
    constexpr static float   cross(const vector2& a, const vector2& b) noexcept;

    static bool is_close_enough(const vector2& a, const vector2& b, float threshold);

    constexpr static vector2 x_axis() noexcept;
    constexpr static vector2 y_axis() noexcept;

    void print_with_width(std::ostream& os, size_t width);
};

constexpr vector2::vector2(const vector2& vector) noexcept
{
    x = vector.x;
    y = vector.y;
}

constexpr vector2::vector2(vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;
}

constexpr vector2::vector2(float x, float y) noexcept
    : x{x}, y{y}
{
}

inline float& vector2::operator[](int i)
{
    switch (i % 2)
    {
        case 0:  return x;
        default: return y;
    }
}

constexpr float vector2::operator[](int i) const noexcept
{
    return *((float*)this + (i % 2));
}

constexpr vector2& vector2::operator=(vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;

    return *this;
}

constexpr vector2& vector2::operator=(const vector2& vector) noexcept
{
    x = vector.x;
    y = vector.y;

    return *this;
}

constexpr vector2 vector2::operator +(const vector2& rhs) const noexcept
{
    vector2 res;
    res.x = x + rhs.x;
    res.y = y + rhs.y;

    return res;
}

constexpr vector2 vector2::operator -(const vector2& rhs) const noexcept
{
    vector2 res;
    res.x = x - rhs.x;
    res.y = y - rhs.y;

    return res;
}

constexpr vector2 vector2::operator *(const float fac) const noexcept
{
    vector2 res;
    res.x = x * fac;
    res.y = y * fac;

    return res;
}

constexpr vector2 vector2::operator /(const float fac) const noexcept
{
    vector2 res;
    res.x = x / fac;
    res.y = y / fac;

    return res;
}

constexpr vector2& vector2::operator +=(const vector2& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

constexpr vector2& vector2::operator -=(const vector2& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

constexpr vector2& vector2::operator *=(float fac) noexcept
{
    x *= fac;
    y *= fac;

    return *this;
}

constexpr vector2& vector2::operator /=(float fac) noexcept
{
    if (fac != 0)
    {
        x /= fac;
        y /= fac;
    }
    else
    {
        x = 0;
        y = 0;
    }

    return *this;
}

inline vector2 vector2::normalized() const
{
    vector2 res;
    float l = length();

    if (l)
    {
        res.x = x / l;
        res.y = y / l;
    }

    return res;
}

inline void vector2::normalize()
{
    float l = length();

    if (l)
    {
        operator/=(l);
    }
}

inline void vector2::limit(float max_length)
{
    float l = length();

    if (l && max_length < l)
    {
        x = x / l * max_length;
        y = y / l * max_length;
    }
}

inline void vector2::set_length(float new_length)
{
    normalize();
    operator*=(new_length);
}

inline vector2 vector2::direction(const vector2& from, const vector2& to)
{
    return (to - from).normalized();
}

inline float vector2::distance(const vector2& a, const vector2& b)
{
    return (a - b).length();
}

inline float vector2::distance_squared(const vector2& a, const vector2& b)
{
    return (a - b).length_squared();
}

constexpr float vector2::dot(const vector2& a, const vector2& b) noexcept
{
    return a.x * b.x + a.y * b.y;
}

constexpr float vector2::cross(const vector2& a, const vector2& b) noexcept
{
    return a.x * b.y - a.y * b.x;
}

inline bool vector2::is_close_enough(const vector2& a, const vector2& b, float threshold)
{
    return distance_squared(a, b) < threshold * threshold;
}

constexpr vector2 vector2::x_axis() noexcept
{
    return vector2{1};
}

constexpr vector2 vector2::y_axis() noexcept
{
    return vector2{0, 1};
}

inline void vector2::print_with_width(std::ostream& os, size_t width)
{
    os << std::fixed;
    os << "( ";
    os << std::setw(width) << x << ", ";
    os << std::setw(width) << y << " )";
}

inline std::ostream& operator<<(std::ostream& os, const vector2& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

inline float vector2::length_squared() const
{
    return x * x + y * y;
}

inline float vector2::length() const
{
    return std::sqrt(length_squared());
}

constexpr vector2 vector2::perp() const noexcept
{
    return vector2(-y, x);
}

constexpr void vector2::zero() noexcept
{
    x = 0;
    y = 0;
}

constexpr vector2 vector2::lerp(const vector2& a, const vector2& b, float factor) noexcept
{
    return a + (b - a) * factor;
}

inline bool operator==(const vector2& v1, const vector2& v2) noexcept
{
    return math::close_enough(v1.x, v2.x) && math::close_enough(v1.y, v2.y);
}

constexpr bool operator<(const vector2& v1, const vector2& v2) noexcept
{
    return v1.x < v2.x;
}

} // namespace flp::geometry

using vector2 = flp::geometry::vector2;

template <>
struct std::hash<vector2>
{
    size_t operator()(const vector2& vec) const
    {
        return std::hash<float>()(vec.x) ^ std::hash<float>()(vec.y);
    }
};

template <>
struct std::hash<std::pair<vector2, vector2>>
{
    size_t operator()(const std::pair<vector2, vector2>& pair) const
    {
        return std::hash<vector2>()(pair.first) ^ std::hash<vector2>()(pair.second);
    }
};

inline bool operator==(
    const std::pair<vector2, vector2>& p1,
    const std::pair<vector2, vector2>& p2) noexcept
{
    return
        p1.first == p2.first && p1.second == p2.second ||
        p1.first == p2.second && p1.second == p2.first;
}