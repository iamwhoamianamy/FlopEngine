#include "box.hpp"
#include <math.h>

Box::Box(const Vector3& center, const Vector3& halfDimensions) :
    center(center), halfDimensions(halfDimensions)
{
}

bool isInRange(float value, float rangeCenter, float rangeHalfWidth)
{
    return (rangeCenter - rangeHalfWidth <= value &&
        value < rangeCenter + rangeHalfWidth);
}

bool Box::doContain(const Vector3& point) const
{
    return (isInRange(point.x, center.x, halfDimensions.x) &&
        isInRange(point.y, center.y, halfDimensions.y) &&
        isInRange(point.z, center.z, halfDimensions.z));
}

bool Box::doIntersect(const Box& _box) const
{
    return Box(_box.center, halfDimensions + _box.halfDimensions).doContain(center);
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
