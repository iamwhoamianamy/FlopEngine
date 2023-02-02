#include <math.h>

#include "rectangle.hpp"

rectangle_t::rectangle_t() :
    center({ 0, 0 }), halfDimensions({ 0, 0 })
{
}

rectangle_t::rectangle_t(const vector2& center, const vector2& halfDimensions) :
    center(center), halfDimensions(halfDimensions)
{
}

rectangle_t::rectangle_t(const vector2& center, float radius) :
    center(center), halfDimensions(radius, radius)
{
}