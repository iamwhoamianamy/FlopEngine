#include <exception>
#include <iomanip>
#include "vector2.hpp"

Vector2::Vector2(const Vector2& vector)
{
    x = vector.x;
    y = vector.y;
}

Vector2::Vector2(Vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;
}

Vector2::Vector2(float x, float y)
    : x(x), y(y)
{
}

float& Vector2::operator[](int i)
{
    switch(i)
    {
        case 0: return x;
        case 1: return y;
    }
}

const float Vector2::operator[](int i) const
{
    return *((float*)this + i);
}

Vector2& Vector2::operator=(Vector2&& vector) noexcept
{
    x = vector.x;
    y = vector.y;

    return *this;
}

Vector2& Vector2::operator=(const Vector2& vector)
{
    x = vector.x;
    y = vector.y;

    return *this;
}

Vector2 Vector2::operator +(const Vector2& rhs) const
{
    Vector2 res;
    res.x = x + rhs.x;
    res.y = y + rhs.y;

    return res;
}

Vector2 Vector2::operator -(const Vector2& rhs) const
{
    Vector2 res;
    res.x = x - rhs.x;
    res.y = y - rhs.y;

    return res;
}

Vector2 Vector2::operator *(const float fac) const
{
    Vector2 res;
    res.x = x * fac;
    res.y = y * fac;

    return res;
}

Vector2 Vector2::operator /(const float fac) const
{
    Vector2 res;
    res.x = x / fac;
    res.y = y / fac;

    return res;
}

Vector2& Vector2::operator +=(const Vector2& rhs)
{
    x += rhs.x;
    y += rhs.y;

    return *this;
}

Vector2& Vector2::operator -=(const Vector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

Vector2& Vector2::operator *=(float fac)
{
    x *= fac;
    y *= fac;

    return *this;
}

Vector2& Vector2::operator /=(float fac)
{
    if(fac != 0)
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

float Vector2::lengthSquared() const
{
    return x * x + y * y;
}

float Vector2::length() const
{
    return sqrt(lengthSquared());
}

Vector2 Vector2::normalized() const
{
    Vector2 res;
    float l = length();

    if(l)
    {
        res.x = x / l;
        res.y = y / l;
    }

    return res;
}

void Vector2::normalize()
{
    float l = length();

    if(l)
    {
        operator/=(l);
    }
}

void Vector2::limit(float limitLength)
{
    float l = length();

    if(l && limitLength < l)
    {
        x = x / l * limitLength;
        y = y / l * limitLength;
    }
}

void Vector2::setLength(float newLength)
{
    normalize();
    operator*=(newLength);
}

Vector2 Vector2::perp() const
{
    return Vector2(-y, x);
}

Vector2 Vector2::direction(const Vector2& from, const Vector2& to)
{
    Vector2 res = to - from;
    return res.normalized();
}

float Vector2::distanceSquared(const Vector2& vec1, const Vector2& vec2)
{
    return (vec1 - vec2).lengthSquared();
}

float Vector2::dot(const Vector2& vec1, const Vector2& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

float Vector2::cross(const Vector2& vec1, const Vector2& vec2)
{
    return vec1.x * vec2.y - vec1.y * vec2.x;
}

Vector2 Vector2::xAxis()
{
    return Vector2(1);
}

Vector2 Vector2::yAxis()
{
    return Vector2(0, 1);
}

void Vector2::printWithWidth(std::ostream& os, size_t width)
{
    os << std::fixed;
    os << "( ";
    os << std::setw(width) << x << ", ";
    os << std::setw(width) << y << " )";
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
