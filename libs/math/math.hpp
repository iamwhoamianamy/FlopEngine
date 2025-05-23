#pragma once

#include <algorithm>
#include <cmath>

#include "libs/geometry/vector2.hpp"

namespace flp::math
{

constexpr float PI = 3.141592f;
constexpr float TWO_PI = 2.0f * PI;
constexpr float FLOAT_CLOSE_ENOUGH_THRESHOLD = 1e-5f;

inline float random_normed()
{
    return static_cast<float>(std::rand()) / RAND_MAX;
}

inline float random_in_range(float low, float high)
{
    return low + random_normed() * (high - low);
}

inline vector2 generate_random_vector()
{
    float angle = random_normed() * TWO_PI;
    float x = cosf(angle);
    float y = sinf(angle);

    return vector2{x, y};
}

template <class T>
T limit(T x, T low, T high)
{
    return std::max(std::min(x, high), low);
}

template <class Old, class New>
constexpr New map(Old x, Old old_low, Old old_high, New new_low, New new_high)
{
    x = limit(x, old_low, old_high);
    auto result = new_low + (float)(x - old_low) / (old_high - old_low) * (new_high - new_low);

    return static_cast<New>(result);
}

constexpr bool is_in_range(float value, float range_center, float range_half_width) noexcept
{
    return
        range_center - range_half_width <= value &&
        value < range_center + range_half_width;
}

constexpr float deg_to_rad(float deg) noexcept
{
    return deg * PI / 180.0f;
}

constexpr float rad_to_deg(float rad) noexcept
{
    return rad * 180.0f / PI;
}

inline bool close_enough(float a, float b) noexcept
{
    return std::abs(a - b) < FLOAT_CLOSE_ENOUGH_THRESHOLD;
}

} // namespace flp::math