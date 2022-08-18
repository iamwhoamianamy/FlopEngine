#include "marching_grid.hpp"
#include "math.hpp"
#include "drawing.hpp"

MarchingGrid::MarchingGrid(size_t nodeCountX, size_t nodeCountY) :
    nodeCountX(nodeCountX), nodeCountY(nodeCountY)
{
    _grid.resize(nodeCountY, std::vector<float>(nodeCountX));
}

void MarchingGrid::clear()
{
    for(size_t y = 0; y < nodeCountY; y++)
    {
        for(size_t x = 0; x < nodeCountX; x++)
        {
            _grid[y][x] = 0;
        }
    }
}

void MarchingGrid::addContribution(
    const Vector2& point, float maxX, float maxY)
{
    //float x = math::limit(point.x, 0.0f, maxX);
    //float y = math::limit(point.x, 0.0f, maxY);

    //x = math::map(x, 0.0f, maxX, 0.0f, (float)nodeCountX);
    //y = math::map(y, 0.0f, maxY, 0.0f, (float)nodeCountY);

    //float currentVal = _grid[y][x];
    float cellWidth = maxX / (nodeCountX - 1);
    float cellHeight = maxY / (nodeCountY - 1);

    for(size_t y = 0; y < nodeCountY; y++)
    {
        for(size_t x = 0; x < nodeCountX; x++)
        {
            float saturation = 200.0 / sqrt(
                Vector2::distanceSquared(point, { x * cellWidth, y * cellHeight }));

            _grid[y][x] += saturation;
        }
    }
}

void MarchingGrid::draw(float screenWidth, float screenHeigh) const
{
    float cellWidth = screenWidth / (nodeCountX - 1);
    float cellHeight = screenHeigh / (nodeCountY - 1);

    for(size_t y = 0; y < nodeCountY; y++)
    {
        float currentY = cellHeight * y;

        for(size_t x = 0; x < nodeCountX; x++)
        {
            float currentX = cellWidth * x;
            float saturation = math::limit(_grid[y][x], 0.0f, 10.0f);
            draw::setColor(math::map(saturation, 0.0f, 10.0f, 0, 255), 0, 0);
            draw::drawPoint({ currentX, currentY }, saturation);
        }
    }
}
