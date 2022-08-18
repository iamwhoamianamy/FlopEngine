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

    void addContributionReverseSquare(
        const Vector2& point, float contribution, float maxX, float maxY);

    void addContributionBump(
        const Vector2& point, float contribution, float maxX, float maxY);

    void draw(float screenWidth, float screenHeigh) const;
    void marchAllCells(float screenWidth, float screenHeigh);

    const std::vector<std::vector<float>>& grid() const;
private:
};
