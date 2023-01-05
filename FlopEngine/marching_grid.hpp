#pragma once
#include <array>
#include <algorithm>
#include <cmath>

#include "vector2.hpp"
#include "math.hpp"
#include "drawing.hpp"

template <size_t nodeCountX, size_t nodeCountY>
class MarchingGrid
{
private:
    std::array<float, nodeCountX * nodeCountY> _grid;

public:
    MarchingGrid() = default;

    void clear();

    size_t plainId(size_t x, size_t y) const;
    void set(size_t x, size_t y, float val);
    float get(size_t x, size_t y) const;

    void addContributionBump(
        const Vector2& point, float contribution, float maxX, float maxY);

    void draw(float screenWidth, float screenHeigh) const;
    void marchAllCells(float screenWidth, float screenHeigh);

    auto& grid() const
    {
        return _grid;
    }

private:
};

template<size_t nodeCountX, size_t nodeCountY>
inline void MarchingGrid<nodeCountX, nodeCountY>::clear()
{
    std::fill(_grid.begin(), _grid.end(), 0.0f);
}

template<size_t nodeCountX, size_t nodeCountY>
inline size_t MarchingGrid<nodeCountX, nodeCountY>::plainId(size_t x, size_t y) const
{
    return y * nodeCountX + x;
}

template<size_t nodeCountX, size_t nodeCountY>
inline void MarchingGrid<nodeCountX, nodeCountY>::set(size_t x, size_t y, float val)
{
    _grid[plainId(x, y)] = val;
}

template<size_t nodeCountX, size_t nodeCountY>
inline float MarchingGrid<nodeCountX, nodeCountY>::get(size_t x, size_t y) const
{
    return _grid[plainId(x, y)];
}

template<size_t nodeCountX, size_t nodeCountY>
inline void MarchingGrid<nodeCountX, nodeCountY>::addContributionBump(const Vector2& point, float contribution, float maxX, float maxY)
{
    size_t centerX = math::map(point.x, 0.0f, maxX, (size_t)0, nodeCountX);
    size_t centerY = math::map(point.y, 0.0f, maxY, (size_t)0, nodeCountY);

    float cellWidth = maxX / (nodeCountX - 1);
    float cellHeight = maxY / (nodeCountY - 1);

    size_t radiusX = contribution / cellWidth;
    size_t radiusY = contribution / cellHeight;

    size_t lowX = math::limit<long long>(
        (long long)centerX - radiusX - 1,
        0,
        (nodeCountX));

    size_t highX = math::limit<long long>(
        centerX + radiusX + 1,
        0,
        (nodeCountX));

    size_t lowY = math::limit<long long>(
        (long long)centerY - radiusY - 1,
        0,
        (nodeCountY));

    size_t highY = math::limit<long long>(
        centerY + radiusY + 1,
        0,
        (nodeCountY));

    for (size_t yId = lowY; yId < highY; yId++)
    {
        float y = cellHeight * yId;

        for (size_t xId = lowX; xId < highX; xId++)
        {
            float x = cellWidth * xId;
            float r = std::sqrt(std::pow(point.x - x, 2) + std::pow(point.y - y, 2));

            if (std::abs(r) < contribution)
            {
                float contrSquared = contribution * contribution;
                float f = std::exp(-0.5 * contrSquared / (contrSquared - r * r));
                set(xId, yId, std::min(get(xId, yId) + f, 1.0f));
            }
        }
    }
}

template<size_t nodeCountX, size_t nodeCountY>
inline void MarchingGrid<nodeCountX, nodeCountY>::draw(float screenWidth, float screenHeigh) const
{
    float cellWidth = screenWidth / (nodeCountX - 1);
    float cellHeight = screenHeigh / (nodeCountY - 1);

    for (size_t yId = 0; yId < nodeCountY; yId++)
    {
        float y = cellHeight * yId;

        for (size_t xId = 0; xId < nodeCountX; xId++)
        {
            float x = cellWidth * xId;

            UCHAR intencity = math::map(get(xId, yId), 0.0f, 1.0f, 0, 255);
            draw::setColor(intencity, 0, 0);

            float saturation = math::map(get(xId, yId), 0.0f, 1.0f, 1, 10);
            draw::drawPoint({x, y}, saturation);
        }
    }
}

template<size_t nodeCountX, size_t nodeCountY>
inline void MarchingGrid<nodeCountX, nodeCountY>::marchAllCells(float screenWidth, float screenHeigh)
{
    float cellWidth = screenWidth / (nodeCountX - 1);
    float cellHeight = screenHeigh / (nodeCountY - 1);
    const float treshold = 0.5f;

    struct Corner
    {
        float weight;
        Vector2 loc;
    };

    for (int yId = 0; yId < nodeCountY - 1; yId++)
    {
        float currentY = cellHeight * yId;

        for (int xId = 0; xId < nodeCountX - 1; xId++)
        {
            float currentX = cellWidth * xId;

            Corner c1{get(xId, yId), Vector2(currentX, currentY)};
            Corner c2{get(xId + 1, yId), Vector2(currentX + cellWidth, currentY)};
            Corner c3{get(xId + 1, yId + 1), Vector2(currentX + cellWidth, currentY + cellHeight)};
            Corner c4{get(xId, yId + 1), Vector2(currentX, currentY + cellHeight)};

            float x, y;

            x = c1.loc.x + (c2.loc.x - c1.loc.x) * ((treshold - c1.weight) / (c2.weight - c1.weight));
            y = c1.loc.y;
            Vector2 a(x, y);

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

            switch (var)
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
