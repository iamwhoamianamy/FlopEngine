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
    float cellWidth = maxX / (nodeCountX - 1);
    float cellHeight = maxY / (nodeCountY - 1);

    for(size_t y = 0; y < nodeCountY; y++)
    {
        for(size_t x = 0; x < nodeCountX; x++)
        {
            float saturation = 50 / sqrt(
                Vector2::distanceSquared(point, { x * cellWidth, y * cellHeight }));

            _grid[y][x] = std::min(_grid[y][x] + saturation, 1.0f);
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

            UCHAR intencity = math::map(_grid[y][x], 0.0f, 1.0f, 0, 255);
            draw::setColor(intencity, 0, 0);

            float saturation = math::map(_grid[y][x], 0.0f, 1.0f, 0, 10);
            draw::drawPoint({ currentX, currentY }, saturation);
        }
    }
}

void MarchingGrid::marchAllCells(float screenWidth, float screenHeigh)
{
    float cellWidth = screenWidth / (nodeCountX - 1);
    float cellHeight = screenHeigh / (nodeCountY - 1);
    const float treshold = 0.5f;
    draw::setColor();

    struct Corner
    {
        float weight;
        Vector2 loc;
    };

    for(int i = 0; i < nodeCountY - 1; i++)
    {
        float currentY = cellHeight * i;

        for(int j = 0; j < nodeCountX - 1; j++)
        {
            float currentX = cellWidth * j;

            Corner c1 = { _grid[i][j], Vector2(currentX, currentY) };
            Corner c2 = { _grid[i][j + 1], Vector2(currentX + cellWidth, currentY) };
            Corner c3 = { _grid[i + 1][j + 1], Vector2(currentX + cellWidth, currentY + cellHeight) };
            Corner c4 = { _grid[i + 1][j], Vector2(currentX, currentY + cellHeight) };

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
