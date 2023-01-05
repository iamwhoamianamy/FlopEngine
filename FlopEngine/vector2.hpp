#pragma once
#include <ostream>
#include <iomanip>

class Vector2
{
public:
    float x;
    float y;

    inline Vector2(const Vector2& vector);
    inline Vector2(Vector2&& vector) noexcept;
    inline Vector2(float x = 0, float y = 0);

    inline float& operator[](int i);
    inline float operator[](int i) const;

    inline Vector2& operator=(Vector2&& vector) noexcept;
    inline Vector2& operator=(const Vector2& vector);

    inline Vector2 operator +(const Vector2& rhs) const;
    inline Vector2 operator -(const Vector2& rhs) const;
    inline Vector2 operator *(const float fac) const;
    inline Vector2 operator /(const float fac) const;

    inline Vector2& operator +=(const Vector2& rhs);
    inline Vector2& operator -=(const Vector2& rhs);
    inline Vector2& operator *=(float fac);
    inline Vector2& operator /=(float fac);

    inline float lengthSquared() const;
    inline float length() const;
    inline Vector2 normalized() const;
    inline Vector2 perp() const;
    inline void zero();

    static inline Vector2 lerp(Vector2 a, Vector2 b, float factor);
    inline void normalize();
    inline void limit(float limitLength);
    inline void setLength(float newLength);

    static Vector2 direction(const Vector2& from, const Vector2& to);
    static float distanceSquared(const Vector2& vec1, const Vector2& vec2);
    static float dot(const Vector2& vec1, const Vector2& vec2);
    static float cross(const Vector2& vec1, const Vector2& vec2);

    static Vector2 xAxis();
    static Vector2 yAxis();

    inline void printWithWidth(std::ostream& os, size_t width);
};

inline Vector2::Vector2(const Vector2& vector)
{
    x = vector.x;
    y = vector.y;
}

inline Vector2::Vector2(Vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;
}

inline Vector2::Vector2(float x, float y)
    : x(x), y(y)
{
}

float& Vector2::operator[](int i)
{
    switch (i)
    {
        case 0: return x;
        case 1: return y;
    }
}

inline float Vector2::operator[](int i) const
{
    return *((float*)this + i);
}

inline Vector2& Vector2::operator=(Vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;

    return *this;
}

inline Vector2& Vector2::operator=(const Vector2& vector)
{
    x = vector.x;
    y = vector.y;

    return *this;
}

inline Vector2 Vector2::operator +(const Vector2& rhs) const
{
    Vector2 res;
    res.x = x + rhs.x;
    res.y = y + rhs.y;

    return res;
}

inline Vector2 Vector2::operator -(const Vector2& rhs) const
{
    Vector2 res;
    res.x = x - rhs.x;
    res.y = y - rhs.y;

    return res;
}

inline Vector2 Vector2::operator *(const float fac) const
{
    Vector2 res;
    res.x = x * fac;
    res.y = y * fac;

    return res;
}

inline Vector2 Vector2::operator /(const float fac) const
{
    Vector2 res;
    res.x = x / fac;
    res.y = y / fac;

    return res;
}

inline Vector2& Vector2::operator +=(const Vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

inline Vector2& Vector2::operator -=(const Vector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

inline Vector2& Vector2::operator *=(float fac)
{
    x *= fac;
    y *= fac;

    return *this;
}

inline Vector2& Vector2::operator /=(float fac)
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

inline Vector2 Vector2::normalized() const
{
    Vector2 res;
    float l = length();

    if (l)
    {
        res.x = x / l;
        res.y = y / l;
    }

    return res;
}

void Vector2::normalize()
{
    float l = length();

    if (l)
    {
        operator/=(l);
    }
}

inline void Vector2::limit(float limitLength)
{
    float l = length();

    if (l && limitLength < l)
    {
        x = x / l * limitLength;
        y = y / l * limitLength;
    }
}

inline void Vector2::setLength(float newLength)
{
    normalize();
    operator*=(newLength);
}

inline Vector2 Vector2::direction(const Vector2& from, const Vector2& to)
{
    return (to - from).normalized();
}

inline float Vector2::distanceSquared(const Vector2& vec1, const Vector2& vec2)
{
    return (vec1 - vec2).lengthSquared();
}

inline float Vector2::dot(const Vector2& vec1, const Vector2& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

inline float Vector2::cross(const Vector2& vec1, const Vector2& vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

inline Vector2 Vector2::xAxis()
{
    return Vector2(1);
}

inline Vector2 Vector2::yAxis()
{
    return Vector2(0, 1);
}

inline void Vector2::printWithWidth(std::ostream& os, size_t width)
{
    os << std::fixed;
    os << "( ";
    os << std::setw(width) << x << ", ";
    os << std::setw(width) << y << " )";
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

inline float Vector2::lengthSquared() const
{
    return x * x + y * y;
}

inline float Vector2::length() const
{
    return sqrt(lengthSquared());
}

inline Vector2 Vector2::perp() const
{
    return Vector2(-y, x);
}

inline void Vector2::zero()
{
    x = 0;
    y = 0;
}

inline Vector2 Vector2::lerp(Vector2 a, Vector2 b, float factor)
{
    return a + (b - a) * factor;
}