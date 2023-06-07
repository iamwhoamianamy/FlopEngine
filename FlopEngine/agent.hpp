#pragma once
#include "vector2.hpp"
#include "concepts.hpp"

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
};

inline void agent::update_position(float viscosity, flp::duration auto ellapsed)
{
    velocity += acceleration * ellapsed.count() / 1000;
    position += velocity * ellapsed.count() / 1000;
    acceleration.zero();
    velocity *= viscosity;
}

}
