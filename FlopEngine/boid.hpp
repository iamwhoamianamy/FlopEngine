#pragma once
#include "vector2.hpp"

constexpr float BOID_SIZE = 15.0f;

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

    void updatePosition(float ellapsed);
};

