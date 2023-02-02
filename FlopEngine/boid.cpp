#include "boid.hpp"
#include "math.hpp"

boid_t::boid_t(
    const vector2& position,
    const vector2& velocity,
    const vector2& acceleration) :
    position(position), velocity(velocity), acceleration(acceleration)
{
}

void boid_t::update_position(float viscosity, flp::duration auto ellapsed)
{
    velocity += acceleration * ellapsed.count() / 1000;
    position += velocity * ellapsed.count() / 1000;
    acceleration.zero();
    velocity *= viscosity;
}

void boid_t::avoid(vector2 target, float strength, flp::duration auto ellapsed)
{
    vector2 direction = vector2::direction(position, target);
    strength *= (strength + sqrt(vector2::distance_squared(position, target)));
    acceleration -= direction * strength * ellapsed.count() / 1000;
}

void boid_t::wander(float strength, flp::duration auto ellapsed)
{
    vector2 direction = math::generate_random_vector() * velocity.length();
    direction += velocity.normalized() * 2;
    direction.set_length(velocity.length());

    velocity = vector2::lerp(velocity, direction, strength);
}
