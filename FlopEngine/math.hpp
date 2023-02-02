#pragma once
#include <algorithm>
#include "vector2.hpp"

namespace math
{

constexpr float PI = 3.141592f;
constexpr float TWO_PI = 2 * PI;

vector2 generate_random_vector();

inline float random_normed()
{
    return (float)rand() / RAND_MAX;
}

inline float random_in_range(float low, float high)
{
    return low + random_normed() * (high - low);
}
        
template <class T>
T limit(T x, T low, T high)
{
    return std::max(std::min(x, high), low);
}

template <class Old, class New>
constexpr New map(Old x, Old old_low, Old old_high, New new_low, New new_high)
{
    return new_low + (float)(x - old_low) / (old_high - old_low) * (new_high - new_low);
}

inline float lerp(float low, float high)
{
    return low + (high - low) / 2;
}

inline bool is_in_range(float value, float range_center, float range_half_width)
{
    return
        range_center - range_half_width <= value &&
        value < range_center + range_half_width;
}

}