#pragma once

#include <array>
#include <algorithm>
#include <cmath>

#include "libs/geometry/vector2.hpp"
#include "libs/math/math.hpp"
#include "libs/graphics/drawing.hpp"

template <size_t NodeCountX, size_t NodeCountY>
class marching_grid
{
private:
    std::array<float, NodeCountX * NodeCountY> _grid;

public:
    marching_grid() = default;

    void clear();

    size_t plain_id(size_t x, size_t y) const;
    void set(size_t x, size_t y, float val);
    float get(size_t x, size_t y) const;

    void add_contribution_cone(
        const vector2& point, float contribution, float max_x, float max_y);

    void draw(float screen_width, float screen_heigh) const;

    void march_all_cells(
        float screen_width, float screen_heigh, float threshold = 0.5f);

    const auto& grid() const;

private:
    auto point_to_id(const vector2& p, float max_x, float max_y) const
        -> std::pair<size_t, size_t>;
};

#include "marching_grid_impl.hpp"