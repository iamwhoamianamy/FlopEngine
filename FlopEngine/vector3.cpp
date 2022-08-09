#include <exception>
#include <iomanip>
#include "vector3.hpp"

Vector3::Vector3(const Vector3& vector)
{
   x = vector.x;
   y = vector.y;
   z = vector.z;
}

Vector3::Vector3(Vector3&& vector) noexcept
{
   x = std::move(vector.x);
   y = std::move(vector.y);
   z = std::move(vector.z);
}

Vector3::Vector3(float x, float y, float z)
   : x(x), y(y), z(z) {}

float& Vector3::operator[](int i)
{
   switch(i)
   {
      case 0: return x;
      case 1: return y;
      case 2: return z;
   }
}

const float Vector3::operator[](int i) const
{
   return *((float*)this + i);
}

Vector3& Vector3::operator=(Vector3&& vector) noexcept
{
   x = std::move(vector.x);
   y = std::move(vector.y);
   z = std::move(vector.z);

   return *this;
}

Vector3& Vector3::operator=(const Vector3& vector)
{
   x = vector.x;
   y = vector.y;
   z = vector.z;

   return *this;
}

Vector3 Vector3::operator +(const Vector3& rhs) const
{
   Vector3 res;
   res.x = this->x + rhs.x;
   res.y = this->y + rhs.y;
   res.z = this->z + rhs.z;

   return res;
}

Vector3 Vector3::operator -(const Vector3& rhs) const
{
   Vector3 res;
   res.x = this->x - rhs.x;
   res.y = this->y - rhs.y;
   res.z = this->z - rhs.z;

   return res;
}

Vector3 Vector3::operator *(const float fac) const
{
   Vector3 res;
   res.x = this->x * fac;
   res.y = this->y * fac;
   res.z = this->z * fac;

   return res;
}

Vector3 Vector3::operator /(const float fac) const
{
   Vector3 res;
   res.x = this->x / fac;
   res.y = this->y / fac;
   res.z = this->z / fac;

   return res;
}

Vector3& Vector3::operator +=(const Vector3& rhs)
{
   this->x += rhs.x;
   this->y += rhs.y;
   this->z += rhs.z;

   return *this;
}

Vector3& Vector3::operator -=(const Vector3& rhs)
{
   this->x -= rhs.x;
   this->y -= rhs.y;
   this->z -= rhs.z;

   return *this;
}

Vector3& Vector3::operator *=(const float fac)
{
   this->x *= fac;
   this->y *= fac;
   this->z *= fac;

   return *this;
}

Vector3& Vector3::operator /=(const float fac)
{
   if(fac != 0)
   {
      this->x /= fac;
      this->y /= fac;
      this->z /= fac;
   }
   else
   {
      this->x = 0;
      this->y = 0;
      this->z = 0;
   }

   return *this;
}

float Vector3::lengthSquared() const
{
   return this->x * this->x +
      this->y * this->y +
      this->z * this->z;
}

float Vector3::length() const
{
   return sqrt(this->x * this->x +
               this->y * this->y +
               this->z * this->z);
}

Vector3 Vector3::normalized() const
{
   Vector3 res;
   float l = length();

   if(l)
   {
      res.x = this->x / l;
      res.y = this->y / l;
      res.z = this->z / l;
   }

   return res;
}

void Vector3::normalize()
{
   float l = length();

   if(l != 0)
   {
      this->x /= l;
      this->y /= l;
      this->z /= l;
   }
}

void Vector3::limit(const float limitLength)
{
   float l = length();

   if(l != 0 && l > limitLength)
   {
      this->x = this->x / l * limitLength;
      this->y = this->y / l * limitLength;
      this->z = this->z / l * limitLength;
   }
}

void Vector3::setLength(const float newLength)
{
   normalize();
   this->x *= newLength;
   this->y *= newLength;
   this->z *= newLength;
}

Vector3 Vector3::perp() const
{
   return Vector3(-y, x);
}

Vector3 Vector3::direction(const Vector3& from, const Vector3& to)
{
   Vector3 res = to - from;
   return res.normalized();
}

float Vector3::distanceSquared(const Vector3& vec1, const Vector3& vec2)
{
   return (vec1 - vec2).lengthSquared();
}

float Vector3::dot(const Vector3& vec1, const Vector3& vec2)
{
   return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 Vector3::cross(const Vector3& vec1, const Vector3& vec2)
{
   return Vector3(vec1.y * vec2.z - vec1.z * vec2.y,
                  vec1.z * vec2.x - vec1.x * vec2.z,
                  vec1.x * vec2.y - vec1.y * vec2.x);
}

Vector3 Vector3::xAxis()
{
    return Vector3(1);
}

Vector3 Vector3::yAxis()
{
   return Vector3(0, 1);
}

Vector3 Vector3::zAxis()
{
   return Vector3(0, 0, 1);
}

void Vector3::printWithWidth(std::ostream& os, size_t width)
{
   os << std::fixed;
   os << "( ";
   os << std::setw(width) << x << ", ";
   os << std::setw(width) << y << ", ";
   os << std::setw(width) << z << " )";
}

Vector3 Vector3::leadingCos()
{
   float l = length();

   return Vector3(x / l, y / l, z / l);
}

float Vector3::r() const
{
   return sqrt(x * x + y * y + z * z);
}

float Vector3::t() const
{
   return atan(sqrt(x * x + y + y) / z);
}

float Vector3::f() const
{
   return atan2(y, x);
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
   os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
   return os;
}
