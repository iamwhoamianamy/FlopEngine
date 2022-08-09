#pragma once

class Vector3
{
public:
    float x;
    float y;
    float z;

    Vector3(float x = 0, float y = 0, float z = 0);

    float& operator[](int i);

    Vector3 operator +(const Vector3& rhs) const;
    Vector3 operator -(const Vector3& rhs) const;
    Vector3 operator *(const float fac) const;
    Vector3 operator /(const float fac) const;

    Vector3 operator +=(const Vector3& rhs);
    Vector3 operator -=(const Vector3& rhs);
    Vector3& operator *=(const float fac);
    Vector3& operator /=(const float fac);

    float LengthSquared() const;
    float Length() const;
    Vector3 Normalized() const;
    Vector3 Perp() const;

    void Normalize();
    void Limit(const float limitLength);
    void SetLength(const float newLength);
    Vector3 LeadingCos();

    static Vector3 Direction(const Vector3& from, const Vector3& to);
    static float DistanceSquared(const Vector3& vec1, const Vector3& vec2);
};