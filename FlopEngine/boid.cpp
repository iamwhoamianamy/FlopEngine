#include "boid.hpp"
#include "math.hpp"

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
    strength *= (strength + sqrt(Vector2::distanceSquared(position, target)));
    acceleration -= direction * strength * ellapsed;
}

void Boid::align(const std::vector<Vector2>& velocities, float strength, float ellapsed)
{
    Vector2 direction;

    for(const auto& velocity : velocities)
    {
        direction += velocity;
    }

    direction /= velocities.size();
    direction.setLength(velocity.length());
    direction = Vector2::lerp(velocity, direction, strength);

    velocity = direction;
}

void Boid::gather(const std::vector<Vector2>& targets, float strength, float ellapsed)
{
    Vector2 direction;
    
    for(const auto& target : targets)
    {
        direction += target;
    }

    direction /= targets.size();
    direction = Vector2::direction(position, direction);

    acceleration += direction * strength * ellapsed;
}

void Boid::wander(float strength, float ellapsed)
{
    Vector2 direction = math::generateRandomVector() * velocity.length();
    direction += velocity.normalized() * 2;
    direction.setLength(velocity.length());

    velocity = Vector2::lerp(velocity, direction, strength);
}
