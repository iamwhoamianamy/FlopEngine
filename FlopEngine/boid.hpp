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

    void updatePosition(float viscosity, std::chrono::milliseconds ellapsed);
    void avoid(Vector2 target, float strength, std::chrono::milliseconds ellapsed);
    void align(const auto& friends, float strength, std::chrono::milliseconds ellapsed, auto projection);
    void gather(const auto& targets, float strength, std::chrono::milliseconds ellapsed, auto projection);
    void wander(float strength, std::chrono::milliseconds ellapsed);
};

inline void Boid::align(const auto& friends, float strength, std::chrono::milliseconds ellapsed, auto projection)
{
    Vector2 direction =
        std::transform_reduce(
            friends.begin(), friends.end(),
            Vector2{},
            std::plus{},
            projection);

    direction /= friends.size();
    direction.setLength(velocity.length());
    direction = Vector2::lerp(velocity, direction, strength);

    velocity = direction;
}

void Boid::gather(const auto& targets, float strength, std::chrono::milliseconds ellapsed, auto projection)
{
    Vector2 direction =
        std::transform_reduce(
            targets.begin(), targets.end(),
            Vector2{},
            std::plus{},
            projection);

    direction /= targets.size();
    direction = Vector2::direction(position, direction);

    acceleration += direction * strength * ellapsed.count() / 1000;
}
