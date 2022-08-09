#pragma once
#include <ostream>

class Vector3
{
public:
    float x;
    float y;
    float z;

    Vector3(const Vector3& vector);
    Vector3(Vector3&& vector) noexcept;
    Vector3(float x = 0, float y = 0, float z = 0);

    float& operator[](int i);
    const float operator[](int i) const;

    Vector3& operator=(Vector3&& vector) noexcept;
    Vector3& operator=(const Vector3& vector);

    Vector3 operator +(const Vector3& rhs) const;
    Vector3 operator -(const Vector3& rhs) const;
    Vector3 operator *(const float fac) const;
    Vector3 operator /(const float fac) const;

    Vector3& operator +=(const Vector3& rhs);
    Vector3& operator -=(const Vector3& rhs);
    Vector3& operator *=(const float fac);
    Vector3& operator /=(const float fac);

    float lengthSquared() const;
    float length() const;
    Vector3 normalized() const;
    Vector3 perp() const;

    void normalize();
    void limit(const float limitLength);
    void setLength(const float newLength);
    Vector3 leadingCos();

    float r() const;
    float t() const;
    float f() const;

    static Vector3 direction(const Vector3& from, const Vector3& to);
    static float distanceSquared(const Vector3& vec1, const Vector3& vec2);
    static float dot(const Vector3& vec1, const Vector3& vec2);
    static Vector3 cross(const Vector3& vec1, const Vector3& vec2);

    static Vector3 xAxis();
    static Vector3 yAxis();
    static Vector3 zAxis();

    void printWithWidth(std::ostream& os, size_t width);
};

std::ostream& operator<<(std::ostream& os, const Vector3& vec);