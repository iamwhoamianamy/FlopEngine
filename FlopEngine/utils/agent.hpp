#pragma once

#include <vector>

#include "libs/geometry/vector2.hpp"
#include "libs/geometry/rectangle.hpp"
#include "libs/math/math.hpp"
#include "libs/meta/concepts.hpp"
#include "libs/meta/physics_object_traits.hpp"
#include "utils.hpp"

namespace flp::utils
{

struct agent
{
    vector2 position;
    vector2 velocity;
    vector2 acceleration;

    agent(
        const vector2& position = {},
        const vector2& velocity = {},
        const vector2& acceleration = {});

    void update_position(float viscosity, concepts::duration auto ellapsed);
    void go_through_borders(const geo::rectangle& screen_borders);
    void bounce_from_borders(const geo::rectangle& screen_borders);

    template<flp::concepts::physics_object Obj = agent>
    static auto generate_random(
        const geo::rectangle& range,
        size_t count,
        float max_speed) -> std::vector<Obj>;
};

} // namespace flp::utils

template<>
struct flp::traits::physics_object<flp::utils::agent>
{
    constexpr static vector2& position(utils::agent& obj)
    {
        return obj.position;
    }

    constexpr static vector2& velocity(utils::agent& obj)
    {
        return obj.velocity;
    }

    constexpr static vector2& acceleration(utils::agent& obj)
    {
        return obj.acceleration;
    }
};

template <>
struct flp::traits::converter<flp::utils::agent, vector2>
{
    static flp::concepts::base_same_as<vector2> auto&& convert(
        flp::concepts::base_same_as<utils::agent> auto&& a)
    {
        return a.position;
    }
};

template <>
struct flp::traits::converter<flp::utils::agent*, vector2>
{
    static flp::concepts::base_same_as<vector2> auto&& convert(
        flp::concepts::base_same_as<utils::agent*> auto&& a)
    {
        return a->position;
    }
};

namespace flp::utils
{

inline void agent::update_position(float viscosity, concepts::duration auto ellapsed)
{
    velocity += acceleration * utils::true_ellapsed(ellapsed) * utils::true_ellapsed(ellapsed) / 2;
    position += velocity * utils::true_ellapsed(ellapsed);
    acceleration.zero();
    velocity *= viscosity;
}

template<concepts::physics_object Obj>
inline auto agent::generate_random(
    const geo::rectangle& range,
    size_t count,
    float max_speed) -> std::vector<Obj>
{
    std::vector<Obj> result(count);

    std::ranges::generate_n(result.begin(), count,
        [&range, max_speed]
        {
            auto position{
                vector2{
                    math::random_in_range(0, range.width()),
                    math::random_in_range(0, range.height())
                }
            };

            auto velocity = math::generate_random_vector() * max_speed;

            Obj result_obj;

            flp::traits::physics_object<Obj>::position(result_obj) = position;
            flp::traits::physics_object<Obj>::velocity(result_obj) = velocity;

            return result_obj;
        });

    return result;
}

} // namespace flp::utils
