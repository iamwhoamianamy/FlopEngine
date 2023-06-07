#include "boid.hpp"
#include "math.hpp"

boid_t::boid_t(
    const vector2& position,
    const vector2& velocity,
    const vector2& acceleration)
    : agent{position, velocity, acceleration}
{
}
