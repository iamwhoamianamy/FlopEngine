#pragma once

#include <vector>

#include "libs/geometry/vector2.hpp"
#include "libs/geometry/rectangle.hpp"
#include "libs/math/math.hpp"
#include "utils/concepts.hpp"
#include "utils/utils.hpp"

namespace utils
{

struct agent
{
    vector2 position;
    vector2 velocity;
    vector2 acceleration;

    agent(
        const vector2& position = vector2(),
        const vector2& velocity = vector2(),
        const vector2& acceleration = vector2());

    void update_position(float viscosity, flp::duration auto ellapsed);
    void go_through_borders(const rectangle& screen_borders);
    void bounce_from_borders(const rectangle& screen_borders);

    static auto generate_random(const rectangle& range, size_t count, float max_speed) -> std::vector<agent>;
};

template <>
struct vector2_traits<agent>
{
    const vector2& operator()(const agent& a) const
    {
        return a.position;
    }
};

inline void agent::update_position(float viscosity, flp::duration auto ellapsed)
{
    velocity += acceleration * utils::true_ellapsed(ellapsed);
    position += velocity * utils::true_ellapsed(ellapsed);
    acceleration.zero();
    velocity *= viscosity;
}

inline auto agent::generate_random(const rectangle& range, size_t count, float max_speed) -> std::vector<agent>
{
    std::vector<agent> result(count);

    std::ranges::generate_n(result.begin(), count,
        [&range, max_speed]
        {
            auto position{
                vector2{
                    math::random_in_range(0, range.width()),
                    math::random_in_range(0, range.height())
                }
            };

            auto velocity{math::generate_random_vector() * max_speed};

            return agent{position, velocity};
        });

    return result;
}

}
