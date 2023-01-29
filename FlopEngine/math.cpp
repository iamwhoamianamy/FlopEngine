#include <cmath>
#include "math.hpp"

vector2 math::generate_random_vector()
{
    float angle = random_normed() * TWO_PI;
    float x = std::cosf(angle);
    float y = std::sinf(angle);

    return { x, y };
}

float math::random_in_range(float low, float high)
{
    return low + random_normed() * (high - low);
}
