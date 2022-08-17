#pragma once
#include "vector2.hpp"

namespace math
{
    constexpr float PI = 3.141592;
    constexpr float TWO_PI = 2 * PI;

    inline float randomNormed()
    {
        return (float)rand() / RAND_MAX;
    }

    Vector2 generateRandomVector();
    float randomInRange(float low, float high);
}