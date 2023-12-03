#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include <functional>

#include "libs/geometry/vector2.hpp"
#include "libs/meta/concepts.hpp"
#include "libs/math/math.hpp"
#include "utils/agent.hpp"

namespace flp
{

class boid : public utils::agent
{
public:
    boid(
        const vector2& position = vector2{},
        const vector2& velocity = vector2{},
        const vector2& acceleration = vector2{});

    void avoid(
        vector2 target,
        float strength,
        concepts::duration auto ellapsed);

    void align(
        const std::ranges::range auto& friends,
        float strength,
        concepts::duration auto ellapsed,
        auto&& projection);

    void gather(
        const std::ranges::range auto& targets,
        float strength,
        concepts::duration auto ellapsed,
        auto&& projection);

    void wander(
        float strength,
        concepts::duration auto ellapsed);
};


inline boid::boid(
    const vector2& position,
    const vector2& velocity,
    const vector2& acceleration)
    : agent{position, velocity, acceleration}
{

}

inline void boid::avoid(
    vector2 target,
    float strength,
    concepts::duration auto ellapsed)
{
    vector2 direction = vector2::direction(position, target);
    strength *= (strength + sqrt(vector2::distance_squared(position, target)));
    acceleration -= direction * strength;
}

inline void boid::align(
    const std::ranges::range auto& friends,
    float strength,
    concepts::duration auto ellapsed,
    auto&& projection)
{
    vector2 direction =
        std::transform_reduce(
            friends.begin(), friends.end(),
            vector2{},
            std::plus{},
            projection);

    direction /= static_cast<float>(friends.size());
    direction.set_length(velocity.length());
    direction = vector2::lerp(velocity, direction, strength);
    velocity = direction;
}

inline void boid::gather(
    const std::ranges::range auto& targets,
    float strength,
    concepts::duration auto ellapsed,
    auto&& projection)
{
    vector2 direction =
        std::transform_reduce(
            targets.begin(), targets.end(),
            vector2{},
            std::plus{},
            projection);

    direction /= static_cast<float>(targets.size());
    direction = vector2::direction(position, direction);

    acceleration += direction * strength;
}

inline void boid::wander(
    float strength,
    concepts::duration auto ellapsed)
{
    vector2 direction = math::generate_random_vector() * velocity.length();
    direction += velocity.normalized() * 2;
    direction.set_length(velocity.length());

    velocity = vector2::lerp(velocity, direction, strength);
}

template<>
struct flp::traits::physics_object<boid>
{
    constexpr static vector2& position(boid& obj)
    {
        return obj.position;
    }

    constexpr static vector2& velocity(boid& obj)
    {
        return obj.velocity;
    }

    constexpr static vector2& acceleration(boid& obj)
    {
        return obj.acceleration;
    }
};

template <>
struct flp::traits::converter<boid, vector2>
{
    static vector2& convert(boid& a)
    {
        return a.position;
    }

    static vector2& convert(boid* a)
    {
        return a->position;
    }
};

} // namespace flp