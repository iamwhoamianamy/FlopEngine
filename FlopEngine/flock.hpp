#pragma once
#include <vector>
#include "boid.hpp"
#include "drawing.hpp"
#include "quadtree.hpp"

enum class FlockDrawType
{
    Points = 0,
    Triangles,
    TrianglesFilled
};

struct BoidParameters
{
    float avoidVision = 15;
    float avoidStrength = 40;

    float alignVision = 50;
    float alignStrength = 0.01;

    float gatherVision = 50;
    float gatherStrength = 500;

    float wanderStrength = 0.01;

    float size = 10;
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
        size_t boidsCount = 3000,
        float maxSpeed = 40);

    void updateBoidPositions(float viscosity, float ellapsed);
    void formQuadtree(const Rect& boidFieldBorders, size_t capacity);
    void performFlockingBehaviour(float ellapsed);
    void goThroughWindowBorders(float screenWidth, float screenHeight);
    //void performFleeing()

    const std::vector<Boid>& boids() const;
    FlockDrawType drawType() const;
    void draw() const;
private:
    void performAvoiding(Boid& boid, float ellapsed);
    void performAligning(Boid& boid, float ellapsed);
    void performGathering(Boid& boid, float ellapsed);
    void performWandering(Boid& boid, float ellapsed);
};