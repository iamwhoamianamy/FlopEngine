#pragma once
#include <vector>
#include "vector2.hpp"

class MarchingGrid
{
private:
    std::vector<std::vector<float>> _grid;

    size_t nodeCountX;
    size_t nodeCountY;
public:
    MarchingGrid(size_t nodeCountX = 0, size_t nodeCountY = 0);
    void clear();
    void addContribution(const Vector2& point, float maxX, float maxY);
    void draw(float screenWidth, float screenHeigh) const;
};
