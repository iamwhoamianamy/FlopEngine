#pragma once
#include <vector>
#include "boid.hpp"
#include "drawing.hpp"

enum class FlockDrawType
{
    Points = 0,
    Triangles
};

class Flock
{
private:
    std::vector<Boid> _boids;
    FlockDrawType _drawType;
    draw::Color _color;
public:
    Flock();

    void initRandomOnScreen(
        size_t boidsCount,
        float screenWidth,
        float screenHeight,
        float maxSpeed = 15);

    void updateBoidPositions(float ellapsed);
    void goThroughWindowBorders(float screenWidth, float screenHeight);

    const std::vector<Boid>& boids() const;
    FlockDrawType drawType() const;
    void draw() const;
};