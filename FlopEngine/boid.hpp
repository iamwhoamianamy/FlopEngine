#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include <functional>
#include "vector2.hpp"

class boid_t
{
public:
    vector2 position;
    vector2 velocity;
    vector2 acceleration;

    boid_t(
        const vector2& position = vector2(),
        const vector2& velocity = vector2(),
        const vector2& acceleration = vector2());

    void update_position(float viscosity, std::chrono::milliseconds ellapsed);
    void avoid(vector2 target, float strength, std::chrono::milliseconds ellapsed);
    void align(const std::ranges::range auto& friends, float strength, std::chrono::milliseconds ellapsed, auto&& projection);
    void gather(const std::ranges::range auto& targets, float strength, std::chrono::milliseconds ellapsed, auto&& projection);
    void wander(float strength, std::chrono::milliseconds ellapsed);
};

inline void boid_t::align(const std::ranges::range auto& friends, float strength, std::chrono::milliseconds ellapsed, auto&& projection)
{
    vector2 direction =
        std::transform_reduce(
            friends.begin(), friends.end(),
            vector2{},
            std::plus{},
            projection);

    direction /= friends.size();
    direction.set_length(velocity.length());
    direction = vector2::lerp(velocity, direction, strength);

    velocity = direction;
}

void boid_t::gather(const std::ranges::range auto& targets, float strength, std::chrono::milliseconds ellapsed, auto&& projection)
{
    vector2 direction =
        std::transform_reduce(
            targets.begin(), targets.end(),
            vector2{},
            std::plus{},
            projection);

    direction /= targets.size();
    direction = vector2::direction(position, direction);

    acceleration += direction * strength * ellapsed.count() / 1000;
}
