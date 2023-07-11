#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include <functional>

#include "libs/geometry/vector2.hpp"
#include "utils/concepts.hpp"
#include "libs/math/math.hpp"
#include "utils/agent.hpp"

class boid_t : public utils::agent
{
public:
    boid_t(
        const vector2& position = vector2{},
        const vector2& velocity = vector2{},
        const vector2& acceleration = vector2{});

    void avoid(vector2 target, float strength, flp::duration auto ellapsed);
    void align(const std::ranges::range auto& friends, float strength, flp::duration auto ellapsed, auto&& projection);
    void gather(const std::ranges::range auto& targets, float strength, flp::duration auto ellapsed, auto&& projection);
    void wander(float strength, flp::duration auto ellapsed);
};

inline void boid_t::avoid(vector2 target, float strength, flp::duration auto ellapsed)
{
    vector2 direction = vector2::direction(position, target);
    strength *= (strength + sqrt(vector2::distance_squared(position, target)));
    acceleration -= direction * strength;
}

inline void boid_t::align(const std::ranges::range auto& friends, float strength, flp::duration auto ellapsed, auto&& projection)
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

inline void boid_t::gather(const std::ranges::range auto& targets, float strength, flp::duration auto ellapsed, auto&& projection)
{
    vector2 direction =
        std::transform_reduce(
            targets.begin(), targets.end(),
            vector2{},
            std::plus{},
            projection);

    direction /= targets.size();
    direction = vector2::direction(position, direction);

    acceleration += direction * strength;
}

inline void boid_t::wander(float strength, flp::duration auto ellapsed)
{
    vector2 direction = math::generate_random_vector() * velocity.length();
    direction += velocity.normalized() * 2;
    direction.set_length(velocity.length());

    velocity = vector2::lerp(velocity, direction, strength);
}
