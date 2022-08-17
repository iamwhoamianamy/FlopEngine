#include <cmath>
#include "math.hpp"

Vector2 math::generateRandomVector()
{
    float angle = randomNormed() * TWO_PI;
    float x = std::cosf(angle);
    float y = std::sinf(angle);

    return { x, y };
}

float math::randomInRange(float low, float high)
{
    return low + randomNormed() * (high - low);
}
