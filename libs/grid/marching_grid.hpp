#pragma once

#include <array>
#include <algorithm>
#include <cmath>

#include "libs/geometry/vector2.hpp"
#include "libs/math/math.hpp"
#include "libs/graphics/drawing.hpp"

namespace flp
{

template <size_t NodeCountX, size_t NodeCountY>
class marching_grid
{
private:
    std::array<float, NodeCountX* NodeCountY> _grid;

public:
    marching_grid(float width = 0, float height = 0);

    void resize(float width, float height);
    void clear();

    void add_contribution_cone(const vector2& point, float contribution);

    void draw() const;

    void march_all_cells(float threshold = 0.5f);

    const auto& grid() const;

private:
    size_t plain_id(size_t x, size_t y) const;
    void set(size_t x, size_t y, float val);
    float get(size_t x, size_t y) const;

    auto point_to_id(const vector2& p) const->std::pair<size_t, size_t>;

private:
    float _width;
    float _height;

    float _cell_width;
    float _cell_height;
};

} // namespace flp::grid

#include "marching_grid_impl.hpp"