#include <math.h>

#include "rectangle.hpp"

Rect::Rect(const Vector2& center, const Vector2& halfDimensions) :
    center(center), halfDimensions(halfDimensions)
{
}

bool inRange(float value, float rangeCenter, float rangeHalfWidth)
{
    return 
        rangeCenter - rangeHalfWidth <= value &&
        value < rangeCenter + rangeHalfWidth;
}

bool Rect::contains(float x, float y) const
{
    return
        inRange(x, center.x, halfDimensions.x) &&
        inRange(y, center.y, halfDimensions.y);
}

inline bool Rect::contains(const Vector2& point) const
{
    return contains(point.x, point.y);
}

bool Rect::intersects(const Rect& _Rect) const
{
    return Rect(_Rect.center, halfDimensions + _Rect.halfDimensions).contains(center);
}
