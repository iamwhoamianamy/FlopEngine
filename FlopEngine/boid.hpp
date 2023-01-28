#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include <functional>
#include "vector2.hpp"

class Boid
{
public:
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    Boid(
        const Vector2& position = Vector2(),
        const Vector2& velocity = Vector2(),
        const Vector2& acceleration = Vector2());

    void updatePosition(float viscosity, float ellapsed);

    void avoid(Vector2 target, float strength, float ellapsed);

    void align(const auto& velocities, float strength, float ellapsed, auto projection);
    void gather(const auto& targets, float strength, float ellapsed, auto projection);

    void wander(float strength, float ellapsed);
};

inline void Boid::align(const auto& velocities, float strength, float ellapsed, auto projection)
{
    Vector2 direction =
        std::transform_reduce(
            velocities.begin(), velocities.end(),
            Vector2{},
            std::plus{},
            projection);

    direction /= velocities.size();
    direction.setLength(velocity.length());
    direction = Vector2::lerp(velocity, direction, strength);

    velocity = direction;
}

void Boid::gather(const auto& targets, float strength, float ellapsed, auto projection)
{
    Vector2 direction =
        std::transform_reduce(
            targets.begin(), targets.end(),
            Vector2{},
            std::plus{},
            projection);

    direction /= targets.size();
    direction = Vector2::direction(position, direction);

    acceleration += direction * strength * ellapsed;
}
