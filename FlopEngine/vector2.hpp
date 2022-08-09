#pragma once
#include <ostream>

class Vector2
{
public:
    float x;
    float y;

    Vector2(const Vector2& vector);
    Vector2(Vector2&& vector) noexcept;
    Vector2(float x = 0, float y = 0);

    float& operator[](int i);
    const float operator[](int i) const;

    Vector2& operator=(Vector2&& vector) noexcept;
    Vector2& operator=(const Vector2& vector);

    Vector2 operator +(const Vector2& rhs) const;
    Vector2 operator -(const Vector2& rhs) const;
    Vector2 operator *(const float fac) const;
    Vector2 operator /(const float fac) const;

    Vector2& operator +=(const Vector2& rhs);
    Vector2& operator -=(const Vector2& rhs);
    Vector2& operator *=(float fac);
    Vector2& operator /=(float fac);

    float lengthSquared() const;
    float length() const;
    Vector2 normalized() const;
    Vector2 perp() const;

    void normalize();
    void limit(float limitLength);
    void setLength(float newLength);

    static Vector2 direction(const Vector2& from, const Vector2& to);
    static float distanceSquared(const Vector2& vec1, const Vector2& vec2);
    static float dot(const Vector2& vec1, const Vector2& vec2);
    float cross(const Vector2& vec1, const Vector2& vec2);

    static Vector2 xAxis();
    static Vector2 yAxis();

    void printWithWidth(std::ostream& os, size_t width);
};

std::ostream& operator<<(std::ostream& os, const Vector2& vec);