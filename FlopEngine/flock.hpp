#pragma once
#include <vector>
#include "boid.hpp"
#include "drawing.hpp"
#include "quadtree.hpp"

constexpr size_t FLOCK_DRAW_TYPE_COUNT = 4;

enum class FlockDrawType
{
    Points = 0,
    Triangles,
    TrianglesFilled,
    Letter
};

struct BoidParameters
{
    float avoidVision = 10;
    float avoidStrength = 40;

    float alignVision = 50;
    float alignStrength = 0.01;

    float gatherVision = 50;
    float gatherStrength = 500;

    float fleeVision = 25;
    float fleeStrength = 30;

    float wanderStrength = 0.01;

    float size = 8;

    float maxSpeed = 80;
    float minSpeed = 20;
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
    bool debug = false;

    Flock();

    void initRandomOnScreen(
        float screenWidth,
        float screenHeight,
        size_t boidsCount = 500);

    void updateBoidPositions(float viscosity, float ellapsed);
    void formQuadtree(const Rect& boidFieldBorders, size_t capacity);
    void performFlockingBehaviour(float ellapsed);
    void goThroughWindowBorders(float screenWidth, float screenHeight);
    void performFleeing(const Flock& flock, float ellapsed);

    draw::Color& color();
    const std::vector<Boid>& boids() const;
    FlockDrawType& drawType();
    void draw() const;
    const Quadtree<Boid>& quadtree() const;
private:
    void performAvoiding(Boid& boid, float ellapsed);
    void performAligning(Boid& boid, float ellapsed);
    void performGathering(Boid& boid, float ellapsed);
    void performWandering(Boid& boid, float ellapsed);
};