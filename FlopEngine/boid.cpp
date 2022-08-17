#include "boid.hpp"

Boid::Boid(
    const Vector2& position,
    const Vector2& velocity,
    const Vector2& acceleration) :
    position(position), velocity(velocity), acceleration(acceleration)
{
}

void Boid::updatePosition(float viscosity, float ellapsed)
{
    velocity += acceleration * ellapsed;
    position += velocity * ellapsed;
    acceleration.zero();
    velocity *= viscosity;
}

void Boid::avoid(Vector2 target, float strength, float ellapsed)
{
    Vector2 direction = Vector2::direction(position, target);
    acceleration -= direction * strength * ellapsed;
}

void Boid::align(const std::vector<Vector2>& targets, float strength, float ellapsed)
{
    Vector2 direction;

    for(const auto& target : targets)
    {
        direction += target;
    }

    direction.normalize();
    acceleration += direction * strength * ellapsed;
}
