#include "flock.hpp"
#include "math.hpp"

Flock::Flock() :
    _drawType(FlockDrawType::Points),
    _color(255, 100, 100)
{
}

void Flock::initRandomOnScreen(
    size_t boidsCount,
    float screenWidth,
    float screenHeight)
{
    _boids.clear();
    _boids.reserve(boidsCount);

    for(size_t i = 0; i < boidsCount; i++)
    {
        Vector2 position = {
            math::randomInRange(0, screenWidth),
            math::randomInRange(0, screenHeight) };
        _boids.emplace_back(position, math::generateRandomVector());
    }
}

void Flock::updateBoidPositions(float ellapsed)
{
    for(auto& boid : _boids)
    {
        boid.updatePosition(ellapsed);
    }
}

const std::vector<Boid>& Flock::boids() const
{
    return _boids;
}

FlockDrawType Flock::drawType() const
{
    return _drawType;
}

void Flock::draw() const
{
    switch(_drawType)
    {
        case FlockDrawType::Points:
        {
            for(const auto& boid : _boids)
            {
                draw::drawPoint(boid.position, _color, 10);
            }

            break;
        }
    }
}
