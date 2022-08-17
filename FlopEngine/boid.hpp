#pragma once
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
    void allign(const std::vector<Vector2>& target, float strength, float ellapsed);
};

