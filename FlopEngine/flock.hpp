#pragma once
#include <vector>
#include "boid.hpp"
#include "drawing.hpp"
#include "quadtree.hpp"

enum class FlockDrawType
{
    Points = 0,
    Triangles
};

struct BoidParameters
{
    float avoidVision = 30;
    float avoidStrength = 5000;

    float alignVision = 50;
    float alignStrength = 500;
};

class Flock
{
private:
    std::vector<Boid> _boids;
    FlockDrawType _drawType;
    draw::Color _color;
    Quadtree<Boid> _quadtree;
    BoidParameters _boidParams;
public:
    Flock();

    void initRandomOnScreen(
        size_t boidsCount,
        float screenWidth,
        float screenHeight,
        float maxSpeed = 40);

    void updateBoidPositions(float viscosity, float ellapsed);
    void formQuadtree(const Rect& boidFieldBorders, size_t capacity);
    void performFlockingBehaviour(float ellapsed);
    void goThroughWindowBorders(float screenWidth, float screenHeight);

    const std::vector<Boid>& boids() const;
    FlockDrawType drawType() const;
    void draw() const;
private:
    void performAvoiding(Boid& boid, float ellapsed);
    void performAligning(Boid& boid, float ellapsed);
};