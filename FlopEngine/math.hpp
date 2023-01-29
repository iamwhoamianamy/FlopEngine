#pragma once
#include <algorithm>
#include "vector2.hpp"

namespace math
{
    constexpr float PI = 3.141592f;
    constexpr float TWO_PI = 2 * PI;

    inline float random_normed()
    {
        return (float)rand() / RAND_MAX;
    }

    vector2 generate_random_vector();
    float random_in_range(float low, float high);
    
    template <class T>
    T limit(T x, T low, T high)
    {
        return std::max(std::min(x, high), low);
    }

    template <class Old, class New>
    constexpr New map(Old x, Old oldLow, Old oldHigh, New newLow, New newHigh)
    {
        return newLow + (float)(x - oldLow) / (oldHigh - oldLow) * (newHigh - newLow);
    }

    inline float lerp(float low, float high)
    {
        return low + (high - low) / 2;
    }
}