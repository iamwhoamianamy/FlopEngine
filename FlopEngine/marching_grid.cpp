#include <cmath>
#include "marching_grid.hpp"
#include "math.hpp"
#include "drawing.hpp"

MarchingGrid::MarchingGrid(size_t nodeCountX, size_t nodeCountY) :
    _nodeCountX(nodeCountX), _nodeCountY(nodeCountY)
{
    _grid.resize(nodeCountY, std::vector<float>(nodeCountX));
}

void MarchingGrid::clear()
{
    for(size_t y = 0; y < _nodeCountY; y++)
    {
        for(size_t x = 0; x < _nodeCountX; x++)
        {
            _grid[y][x] = 0;
        }
    }
}

void MarchingGrid::addContributionReverseSquare(
    const Vector2& point, float contribution, float maxX, float maxY)
{
    float cellWidth = maxX / (_nodeCountX - 1);
    float cellHeight = maxY / (_nodeCountY - 1);

    for(size_t yId = 0; yId < _nodeCountY; yId++)
    {
        for(size_t xId = 0; xId < _nodeCountX; xId++)
        {
            float saturation = contribution / sqrt(
                Vector2::distanceSquared(point, { xId * cellWidth, yId * cellHeight }));

            _grid[yId][xId] = std::min(_grid[yId][xId] + saturation, 1.0f);
        }
    }
}

void MarchingGrid::addContributionBump(
    const Vector2& point, float contribution, float maxX, float maxY)
{
    size_t centerX = math::map(point.x, 0.0f, maxX, (size_t)0, _nodeCountX);
    size_t centerY = math::map(point.y, 0.0f, maxY, (size_t)0, _nodeCountY);

    float cellWidth = maxX / (_nodeCountX - 1);
    float cellHeight = maxY / (_nodeCountY - 1);

    size_t radiusX = contribution / cellWidth;
    size_t radiusY = contribution / cellHeight;

    size_t lowX = math::limit(
        (long long)centerX - (long long)radiusX - 1,
        (long long)0,
        (long long)(_nodeCountX));

    size_t highX = math::limit(
        (long long)centerX + (long long)radiusX + 1,
        (long long)0,
        (long long)(_nodeCountX));

    size_t lowY = math::limit(
        (long long)centerY - (long long)radiusY - 1,
        (long long)0,
        (long long)(_nodeCountY));

    size_t highY = math::limit(
        (long long)centerY + (long long)radiusY + 1,
        (long long)0,
        (long long)(_nodeCountY));

    for(size_t yId = lowY; yId < highY; yId++)
    //for(size_t yId = 0; yId < _nodeCountY; yId++)
    {
        float y = cellHeight * yId;

        for(size_t xId = lowX; xId < highX; xId++)
        //for(size_t xId = 0; xId < _nodeCountX; xId++)
        {
            float x = cellWidth * xId;
            float r = std::sqrt(std::pow(point.x - x, 2) + std::pow(point.y - y, 2));

            if(std::abs(r) < contribution)
            {
                float contrSquared = contribution * contribution;
                float f = std::exp(-0.5 * contrSquared / (contrSquared - r * r));
                _grid[yId][xId] = std::min(_grid[yId][xId] + f, 1.0f);
            }
        }
    }
}

void MarchingGrid::draw(float screenWidth, float screenHeigh) const
{
    float cellWidth = screenWidth / (_nodeCountX - 1);
    float cellHeight = screenHeigh / (_nodeCountY - 1);

    for(size_t yId = 0; yId < _nodeCountY; yId++)
    {
        float y = cellHeight * yId;

        for(size_t xId = 0; xId < _nodeCountX; xId++)
        {
            float x = cellWidth * xId;

            UCHAR intencity = math::map(_grid[yId][xId], 0.0f, 1.0f, 0, 255);
            draw::setColor(intencity, 0, 0);

            float saturation = math::map(_grid[yId][xId], 0.0f, 1.0f, 1, 10);
            draw::drawPoint({ x, y }, saturation);
        }
    }
}

void MarchingGrid::marchAllCells(float screenWidth, float screenHeigh)
{
    float cellWidth = screenWidth / (_nodeCountX - 1);
    float cellHeight = screenHeigh / (_nodeCountY - 1);
    const float treshold = 0.5f;
    draw::setColor();

    struct Corner
    {
        float weight;
        Vector2 loc;
    };

    for(int yId = 0; yId < _nodeCountY - 1; yId++)
    {
        float currentY = cellHeight * yId;

        for(int xId = 0; xId < _nodeCountX - 1; xId++)
        {
            float currentX = cellWidth * xId;

            Corner c1 = { _grid[yId][xId], Vector2(currentX, currentY) };
            Corner c2 = { _grid[yId][xId + 1], Vector2(currentX + cellWidth, currentY) };
            Corner c3 = { _grid[yId + 1][xId + 1], Vector2(currentX + cellWidth, currentY + cellHeight) };
            Corner c4 = { _grid[yId + 1][xId], Vector2(currentX, currentY + cellHeight) };

            float x, y;

            x = c1.loc.x + (c2.loc.x - c1.loc.x) * ((treshold - c1.weight) / (c2.weight - c1.weight));
            y = c1.loc.y;
            Vector2 a (x, y);

            x = c2.loc.x;
            y = c2.loc.y + (c3.loc.y - c2.loc.y) * ((treshold - c2.weight) / (c3.weight - c2.weight));
            Vector2 b(x, y);

            x = c4.loc.x + (c3.loc.x - c4.loc.x) * ((treshold - c4.weight) / (c3.weight - c4.weight));
            y = c4.loc.y;
            Vector2 c(x, y);

            x = c1.loc.x;
            y = c1.loc.y + (c4.loc.y - c1.loc.y) * ((treshold - c1.weight) / (c4.weight - c1.weight));
            Vector2 d(x, y);

            int var = 
                (treshold < c1.weight) * 1 +
                (treshold < c2.weight) * 2 +
                (treshold < c3.weight) * 4 +
                (treshold < c4.weight) * 8;

            switch(var)
            {
                case 1: draw::drawLine(d, a); break;
                case 2: draw::drawLine(a, b); break;
                case 3: draw::drawLine(d, b); break;
                case 4: draw::drawLine(c, b); break;
                case 5: draw::drawLine(d, c); draw::drawLine(a, b); break;
                case 6: draw::drawLine(a, c); break;
                case 7: draw::drawLine(d, c); break;
                case 8: draw::drawLine(d, c); break;
                case 9: draw::drawLine(a, c); break;
                case 10: draw::drawLine(d, a); draw::drawLine(c, b); break;
                case 11: draw::drawLine(c, b); break;
                case 12: draw::drawLine(d, b); break;
                case 13: draw::drawLine(a, b); break;
                case 14: draw::drawLine(d, a); break;
            }
        }
    }
}

const std::vector<std::vector<float>>& MarchingGrid::grid() const
{
    return _grid;
}
