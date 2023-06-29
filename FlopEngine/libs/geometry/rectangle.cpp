#include <math.h>

#include "libs/geometry/rectangle.hpp"

rectangle::rectangle() :
    center({ 0, 0 }), half_dimensions({ 0, 0 })
{
}

rectangle::rectangle(const vector2& center, const vector2& half_dimensions) :
    center(center), half_dimensions(half_dimensions)
{
}

rectangle::rectangle(const vector2& center, float radius) :
    center(center), half_dimensions(radius, radius)
{
}