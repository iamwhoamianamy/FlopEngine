#include <math.h>

#include "rectangle.hpp"

rectangle::rectangle() :
    center({ 0, 0 }), halfDimensions({ 0, 0 })
{
}

rectangle::rectangle(const vector2& center, const vector2& halfDimensions) :
    center(center), halfDimensions(halfDimensions)
{
}

rectangle::rectangle(const vector2& center, float radius) :
    center(center), halfDimensions(radius, radius)
{
}

bool inRange(float value, float rangeCenter, float rangeHalfWidth)
{
    return 
        rangeCenter - rangeHalfWidth <= value &&
        value < rangeCenter + rangeHalfWidth;
}

bool rectangle::contains(float x, float y) const
{
    return
        inRange(x, center.x, halfDimensions.x) &&
        inRange(y, center.y, halfDimensions.y);
}

inline bool rectangle::contains(const vector2& point) const
{
    return contains(point.x, point.y);
}

bool rectangle::intersects(const rectangle& _Rect) const
{
    return rectangle(_Rect.center, halfDimensions + _Rect.halfDimensions).contains(center);
}