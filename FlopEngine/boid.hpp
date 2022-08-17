#pragma once
#include <vector>
#include "vector2.hpp"

constexpr float BOID_SIZE = 10.0f;

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
    void align(const std::vector<Vector2>& velocities, float strength, float ellapsed);
    void gather(const std::vector<Vector2>& targets, float strength, float ellapsed);
    void wander(float strength, float ellapsed);
};

