#include "boid.hpp"

Boid::Boid(
    const Vector2& position,
    const Vector2& velocity,
    const Vector2& acceleration) :
    position(position), velocity(velocity), acceleration(acceleration)
{
}

void Boid::updatePosition(float ellapsed)
{
    velocity += acceleration * ellapsed;
    position += velocity * ellapsed;
}
