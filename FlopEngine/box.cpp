#include <math.h>

#include "box.hpp"

Box::Box(const Vector3& center, const Vector3& halfDimensions) :
    center(center), halfDimensions(halfDimensions)
{
}

bool inRange(float value, float rangeCenter, float rangeHalfWidth)
{
    return (rangeCenter - rangeHalfWidth <= value &&
        value < rangeCenter + rangeHalfWidth);
}

bool Box::contains(const Vector3& point) const
{
    return (inRange(point.x, center.x, halfDimensions.x) &&
        inRange(point.y, center.y, halfDimensions.y) &&
        inRange(point.z, center.z, halfDimensions.z));
}

bool Box::intersects(const Box& _box) const
{
    return Box(_box.center, halfDimensions + _box.halfDimensions).contains(center);
}

//
//float Box::left() const
//{
//   return center.x - halfDimensions.x;
//}
//
//float Box::right() const
//{
//   return center.x + halfDimensions.x;
//}
//
//float Box::top() const
//{
//   return center.y - halfDimensions.y;
//}
//
//float Box::bot() const
//{
//   return center.y + halfDimensions.y;
//}
//
//float Box::near() const
//{
//   return center.z - halfDimensions.z;
//}
//
//float Box::far() const
//{
//   return center.z + halfDimensions.z;
//}
