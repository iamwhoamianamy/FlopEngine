#pragma once
#include <ostream>
#include <iomanip>

class vector2;

template <typename V>
struct vector2_traits;

template <typename V>
concept vector2_convertible = requires(V v)
{
    { vector2_traits<V>::get(v) } -> std::convertible_to<vector2>;
};

template <>
struct vector2_traits<vector2>
{
    static const vector2& get(const vector2& v)
    {
        return v;
    }
};

template <>
struct vector2_traits<vector2*>
{
    static const vector2& get(const vector2* v)
    {
        return *v;
    }
};

class vector2
{
public:
    float x;
    float y;

    vector2(const vector2& vector);
    vector2(vector2&& vector) noexcept;
    vector2(float x = 0, float y = 0);

    float& operator[](int i);
    float operator[](int i) const;

    vector2& operator=(vector2&& vector) noexcept;
    vector2& operator=(const vector2& vector);

    vector2 operator +(const vector2& rhs) const;
    vector2 operator -(const vector2& rhs) const;
    vector2 operator *(const float fac) const;
    vector2 operator /(const float fac) const;

    vector2& operator +=(const vector2& rhs);
    vector2& operator -=(const vector2& rhs);
    vector2& operator *=(float fac);
    vector2& operator /=(float fac);

    float length() const;
    float length_squared() const;
    vector2 normalized() const;
    vector2 perp() const;
    void zero();

    void normalize();
    void limit(float max_length);
    void set_length(float new_length);

    static vector2 lerp(const vector2& a, const vector2& b, float factor);
    static vector2 direction(const vector2& from, const vector2& to);
    static float distance(const vector2& vec1, const vector2& vec2);
    static float distance_squared(const vector2& vec1, const vector2& vec2);
    static float dot(const vector2& vec1, const vector2& vec2);
    static float cross(const vector2& vec1, const vector2& vec2);
    static bool is_close_enough(const vector2& a, const vector2& b, float threshold);

    static vector2 xAxis();
    static vector2 yAxis();

    void print_with_width(std::ostream& os, size_t width);
};

inline vector2::vector2(const vector2& vector)
{
    x = vector.x;
    y = vector.y;
}

inline vector2::vector2(vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;
}

inline vector2::vector2(float x, float y)
    : x(x), y(y)
{
}

inline float& vector2::operator[](int i)
{
    switch (i)
    {
        case 0: return x;
        case 1: return y;
    }
}

inline float vector2::operator[](int i) const
{
    return *((float*)this + i);
}

inline vector2& vector2::operator=(vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;

    return *this;
}

inline vector2& vector2::operator=(const vector2& vector)
{
    x = vector.x;
    y = vector.y;

    return *this;
}

inline vector2 vector2::operator +(const vector2& rhs) const
{
    vector2 res;
    res.x = x + rhs.x;
    res.y = y + rhs.y;

    return res;
}

inline vector2 vector2::operator -(const vector2& rhs) const
{
    vector2 res;
    res.x = x - rhs.x;
    res.y = y - rhs.y;

    return res;
}

inline vector2 vector2::operator *(const float fac) const
{
    vector2 res;
    res.x = x * fac;
    res.y = y * fac;

    return res;
}

inline vector2 vector2::operator /(const float fac) const
{
    vector2 res;
    res.x = x / fac;
    res.y = y / fac;

    return res;
}

inline vector2& vector2::operator +=(const vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

inline vector2& vector2::operator -=(const vector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

inline vector2& vector2::operator *=(float fac)
{
    x *= fac;
    y *= fac;

    return *this;
}

inline vector2& vector2::operator /=(float fac)
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

inline float vector2::distance(const vector2& vec1, const vector2& vec2)
{
    return (vec1 - vec2).length();
}

inline float vector2::distance_squared(const vector2& vec1, const vector2& vec2)
{
    return (vec1 - vec2).length_squared();
}

inline float vector2::dot(const vector2& vec1, const vector2& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

inline float vector2::cross(const vector2& vec1, const vector2& vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

inline bool vector2::is_close_enough(const vector2& a, const vector2& b, float threshold)
{
    return distance_squared(a, b) < threshold * threshold;
}

inline vector2 vector2::xAxis()
{
    return vector2(1);
}

inline vector2 vector2::yAxis()
{
    return vector2(0, 1);
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
    return sqrt(length_squared());
}

inline vector2 vector2::perp() const
{
    return vector2(-y, x);
}

inline void vector2::zero()
{
    x = 0;
    y = 0;
}

inline vector2 vector2::lerp(const vector2& a, const vector2& b, float factor)
{
    return a + (b - a) * factor;
}

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

// questionable
namespace math
{

bool close_enough(float a, float b);

}

inline bool operator==(const vector2& v1, const vector2& v2)
{
    return math::close_enough(v1.x, v2.x) && math::close_enough(v1.y, v2.y);
}

inline bool operator==(
    const std::pair<vector2, vector2>& p1,
    const std::pair<vector2, vector2>& p2)
{
    return 
        p1.first == p2.first && p1.second == p2.second || 
        p1.first == p2.second && p1.second == p2.first;
}

inline bool operator<(const vector2& v1, const vector2& v2)
{
    return v1.x < v2.x;
}