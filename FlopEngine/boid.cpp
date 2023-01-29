#include "boid.hpp"
#include "math.hpp"

Boid::Boid(
    const Vector2& position,
    const Vector2& velocity,
    const Vector2& acceleration) :
    position(position), velocity(velocity), acceleration(acceleration)
{
}

void Boid::updatePosition(float viscosity, std::chrono::milliseconds ellapsed)
{
    velocity += acceleration * ellapsed.count() / 1000;
    position += velocity * ellapsed.count() / 1000;
    acceleration.zero();
    velocity *= viscosity;
}

void Boid::avoid(Vector2 target, float strength, std::chrono::milliseconds ellapsed)
{
    Vector2 direction = Vector2::direction(position, target);
    strength *= (strength + sqrt(Vector2::distanceSquared(position, target)));
    acceleration -= direction * strength * ellapsed.count() / 1000;
}

void Boid::wander(float strength, std::chrono::milliseconds ellapsed)
{
    Vector2 direction = math::generateRandomVector() * velocity.length();
    direction += velocity.normalized() * 2;
    direction.setLength(velocity.length());

    velocity = Vector2::lerp(velocity, direction, strength);
}
