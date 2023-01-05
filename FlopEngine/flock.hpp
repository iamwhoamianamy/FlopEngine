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

    float fleeVision = 50;
    float fleeStrength = 60;

    float wanderStrength = 0.01;

    float size = 8;

    float maxSpeed = 80;
    float minSpeed = 60;
};

using quadtree_t = Quadtree<Boid, 8>;

class Flock
{
private:
    std::vector<Boid> _boids;
    FlockDrawType _drawType;
    draw::Color _color;
    quadtree_t _quadtree;
    BoidParameters _boidParams;
public:
    bool debug = false;

    Flock();

    void initRandomOnScreen(
        float screenWidth,
        float screenHeight,
        size_t boidsCount = 500);

    void updateBoidPositions(float viscosity, float ellapsed);
    void formQuadtree(const Rect& boidFieldBorders);
    void performFlockingBehaviour(float ellapsed);
    void goThroughWindowBorders(float screenWidth, float screenHeight);
    void performFleeing(const Flock& flock, float ellapsed);
    void draw() const;

    draw::Color& color();
    const std::vector<Boid>& boids() const;
    FlockDrawType& drawType();
    const quadtree_t& quadtree() const;
private:
    void performAvoiding(Boid& boid, float ellapsed);
    void performAligning(Boid& boid, float ellapsed);
    void performGathering(Boid& boid, float ellapsed);
    void performWandering(Boid& boid, float ellapsed);
};