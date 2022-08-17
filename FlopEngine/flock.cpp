#include <algorithm>
#include "flock.hpp"
#include "math.hpp"
#include "utility.hpp"

template <>
class QuadtreePointHolder<Boid>
{
public:
    static Vector2 position(Boid* boid)
    {
        return boid->position;
    }
};

Flock::Flock() :
    _drawType(FlockDrawType::Triangles),
    _color(255, 255, 255),
    _quadtree(Rect(Vector2(), Vector2()), 0)
{
}

void Flock::initRandomOnScreen(
    size_t boidsCount,
    float screenWidth,
    float screenHeight,
    float maxSpeed)
{
    _boids.clear();
    _boids.reserve(boidsCount);

    for(size_t i = 0; i < boidsCount; i++)
    {
        Vector2 position = {
            math::randomInRange(0, screenWidth),
            math::randomInRange(0, screenHeight) };
        _boids.emplace_back(position, math::generateRandomVector() * maxSpeed);
    }
}

void Flock::updateBoidPositions(float viscosity, float ellapsed)
{
    for(auto& boid : _boids)
    {
        boid.updatePosition(viscosity, ellapsed);
    }
}

void Flock::formQuadtree(const Rect& boidFieldBorders, size_t capacity)
{
    _quadtree = Quadtree<Boid>(boidFieldBorders, capacity);
    _quadtree.insert(_boids);
}

void Flock::performFlockingBehaviour(float ellapsed)
{
    for (auto& boid : _boids)
    {
        performAvoiding(boid, ellapsed);
        performAligning(boid, ellapsed);
        performGathering(boid, ellapsed);
        //performWandering(boid, ellapsed);
    }
}

void Flock::performAvoiding(Boid& boid, float ellapsed)
{
    auto boidsToAvoid = _quadtree.quarry(
        Rect(boid.position, _boidParams.avoidVision));

    if(debug)
    {
        draw::setColor(draw::Color(255, 0, 0));
        glLineWidth(3);

    }

    for(auto& boidToAvoid : boidsToAvoid)
    {
        boid.avoid(boidToAvoid->position, _boidParams.avoidStrength, ellapsed);
        
        if(debug)
        {
            draw::drawLine(boid.position, boidToAvoid->position);
        }
    }
}


void Flock::performAligning(Boid& boid, float ellapsed)
{
    struct Transformer
    {
        Vector2 operator()(Boid* boid)
        {
            return boid->velocity;
        }
    };

    auto boidsToAlignTo = _quadtree.quarry(Rect(boid.position, _boidParams.alignVision));
    auto velocitiesToAlignTo = 
        util::transform<Boid*, Vector2, Transformer>(boidsToAlignTo);

    boid.align(velocitiesToAlignTo, _boidParams.alignStrength, ellapsed);

    if(debug)
    {
        draw::setColor(draw::Color(255, 255, 0));
        glLineWidth(2);

        for(auto& boidToAlignTo : boidsToAlignTo)
        {
            draw::drawLine(boid.position, boidToAlignTo->position);
        }
    }

}

void Flock::performGathering(Boid& boid, float ellapsed)
{
    struct Transformer
    {
        Vector2 operator()(Boid* boid)
        {
            return boid->position;
        }
    };

    auto positionsToGatherWith =
        util::transform<Boid*, Vector2, Transformer>(
            _quadtree.quarry(Rect(boid.position, _boidParams.gatherVision)));

    boid.gather(positionsToGatherWith, _boidParams.gatherStrength, ellapsed);

    if(debug)
    {
        draw::setColor(draw::Color(0, 255, 0));
        glLineWidth(1);

        for(auto& position : positionsToGatherWith)
        {
            draw::drawLine(boid.position, position);
        }
    }
}

void Flock::performWandering(Boid& boid,float ellapsed)
{
    boid.wander(_boidParams.wanderStrength, ellapsed);
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
    draw::setColor(_color);

    switch (_drawType)
    {
        case FlockDrawType::Points:
        {
            for (const auto& boid : _boids)
            {
                draw::drawPoint(boid.position, BOID_SIZE);
            }

            break;
        }
        case FlockDrawType::Triangles:
        {
            for (const auto& boid : _boids)
            {
                Vector2 direction = boid.velocity.normalized();
                Vector2 perpDirection = direction.perp();

                Vector2 a = boid.position + direction * BOID_SIZE / 2;
                Vector2 b = boid.position - direction * BOID_SIZE / 2 + perpDirection * BOID_SIZE / 3;
                Vector2 c = boid.position - direction * BOID_SIZE / 2 - perpDirection * BOID_SIZE / 3;

                draw::drawTriangle(a, b, c);
            }

            break;
        }
    }
}