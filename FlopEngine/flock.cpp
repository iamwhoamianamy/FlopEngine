#include <algorithm>
#include <limits>
#include <fstream>

#include "json.hpp"
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
    _drawType(FlockDrawType::TrianglesFilled),
    _color(255, 255, 255),
    _quadtree(Rect(Vector2(), Vector2()))
{
}

BoidParameters BoidParameters::readParamsFromFile(const std::string& filename)
{
    auto json = nlohmann::json::parse(std::ifstream(filename.data()));

    return {
        .avoidVision = json["avoidVision"],
        .avoidStrength = json["avoidStrength"],
        .alignVision = json["alignVision"],
        .alignStrength = json["alignStrength"],
        .gatherVision = json["gatherVision"],
        .gatherStrength = json["gatherStrength"],
        .fleeVision = json["fleeVision"],
        .fleeStrength = json["fleeStrength"],
        .wanderStrength = json["wanderStrength"],
        .size = json["size"],
        .maxSpeed = json["maxSpeed"],
        .minSpeed = json["minSpeed"]
    };
}

void Flock::initRandomOnScreen(
    float screenWidth,
    float screenHeight,
    size_t boidsCount)
{
    _boids.clear();
    _boids.reserve(boidsCount);

    for(size_t i = 0; i < boidsCount; i++)
    {
        Vector2 position = {
            math::randomInRange(0, screenWidth),
            math::randomInRange(0, screenHeight) };
        _boids.emplace_back(position, math::generateRandomVector() * _boidParams.maxSpeed);
    }
}

void Flock::updateBoidPositions(float viscosity, float ellapsed)
{
    for(auto& boid : _boids)
    {
        boid.updatePosition(viscosity, ellapsed);

        float speed = boid.velocity.length();

        if(speed < _boidParams.minSpeed)
        {
            boid.velocity.setLength(_boidParams.minSpeed);
        }
        else
        {
            if(_boidParams.maxSpeed < speed)
            {
                boid.velocity.limit(_boidParams.maxSpeed);
            }
        }
    }
}

void Flock::formQuadtree(const Rect& boidFieldBorders)
{
    _quadtree = quadtree_t(boidFieldBorders);
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

void Flock::performFleeing(const Flock& flock, float ellapsed)
{
    for(auto& boid : _boids)
    {
        auto boidsToFleeFrom = flock.quadtree().quarry(
            Rect(boid.position, _boidParams.fleeVision));

        for(const auto& boidToFleeFrom : boidsToFleeFrom)
        {
            boid.avoid(boidToFleeFrom->position, _boidParams.fleeStrength, ellapsed);
        }
    }
}

draw::Color& Flock::color()
{
    return _color;
}

const std::vector<Boid>& Flock::boids() const
{
    return _boids;
}

FlockDrawType& Flock::drawType()
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
                draw::drawPoint(boid.position, _boidParams.size);
            }

            break;
        }
        case FlockDrawType::Triangles:
        case FlockDrawType::TrianglesFilled:
        {
            for (const auto& boid : _boids)
            {
                Vector2 direction = boid.velocity.normalized();
                Vector2 perpDirection = direction.perp();

                Vector2 a = boid.position + direction * _boidParams.size / 2;
                Vector2 b = boid.position - direction * _boidParams.size / 2 + 
                    perpDirection * _boidParams.size / 3;
                Vector2 c = boid.position - direction * _boidParams.size / 2 - 
                    perpDirection * _boidParams.size / 3;

                draw::drawTriangle(a, b, c, _drawType == FlockDrawType::TrianglesFilled);
            }

            break;
        }
        case FlockDrawType::Letter:
        {
            char code = _color.r ^ _color.g ^ _color.b;
            char letter = math::map(
                code,
                std::numeric_limits<char>::min(),
                std::numeric_limits<char>::max(),
                '0', '9');

            for(const auto& boid : _boids)
            {
                draw::renderLetter(boid.position, _boidParams.size, letter);
            }

            break;
        }
    }
}

const quadtree_t& Flock::quadtree() const
{
    return _quadtree;
}

void Flock::setParams(const BoidParameters& newParams)
{
    _boidParams = newParams;
}
