#pragma once
#include "vector2.hpp"

namespace math
{
    constexpr float PI = 3.141592f;
    constexpr float TWO_PI = 2 * PI;

    inline float randomNormed()
    {
        return (float)rand() / RAND_MAX;
    }

    Vector2 generateRandomVector();
    float randomInRange(float low, float high);
    
    template <class T>
    T map(T x, T oldLow, T oldHigh, T newLow, T newHigh)
    {
        return newLow + (float)(x - oldLow) / (oldHigh - oldLow) * (newHigh - newLow);
    }
}