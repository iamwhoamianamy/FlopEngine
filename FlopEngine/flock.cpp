#include <algorithm>
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
        _boids.emplace_back(position, math::generateRandomVector() * 10);
    }
}

void Flock::updateBoidPositions(float ellapsed)
{
    for(auto& boid : _boids)
    {
        boid.updatePosition(ellapsed);
    }
}

void Flock::goThroughWindowBorders(float screenWidth, float screenHeight)
{
    for (auto& boid : _boids)
    {
        float x = boid.position.x;
        float y = boid.position.y;

        if (x < 0)
        {
            boid.position.x = screenWidth - 1;
        }
        else
        {
            if (x >= screenWidth)
            {
                boid.position.x = 0;
            }
        }

        if (y < 0)
        {
            boid.position.y = screenHeight - 1;
        }
        else
        {
            if (y >= screenHeight)
            {
                boid.position.y = 0;
            }
        }

        //boid.position.x = std::max(std::min(boid.position.x, screenWidth - 1), 0.0f);
        //boid.position.y = std::max(std::min(boid.position.y, screenWidth - 1), 0.0f);
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
                draw::setColor(_color);
                draw::drawPoint(boid.position, 10);
            }

            break;
        }
    }
}
