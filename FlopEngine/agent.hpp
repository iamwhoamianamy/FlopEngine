#pragma once
#include <vector>

#include "vector2.hpp"
#include "concepts.hpp"
#include "rectangle.hpp"
#include "math.hpp"

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

inline void agent::update_position(float viscosity, flp::duration auto ellapsed)
{
    velocity += acceleration * ellapsed.count() / 1000;
    position += velocity * ellapsed.count() / 1000;
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
