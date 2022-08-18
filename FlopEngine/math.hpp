#pragma once
#include <algorithm>
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
    T limit(T x, T low, T high)
    {
        return std::max(std::min(x, high), low);
    }

    template <class Old, class New>
    New map(Old x, Old oldLow, Old oldHigh, New newLow, New newHigh)
    {
        return newLow + (float)(x - oldLow) / (oldHigh - oldLow) * (newHigh - newLow);
    }
}