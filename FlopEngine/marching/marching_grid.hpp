#pragma once

#include <array>
#include <algorithm>
#include <cmath>

#include "libs/geometry/vector2.hpp"
#include "libs/math/math.hpp"
#include "libs/graphics/drawing.hpp"

template <size_t node_count_x, size_t node_count_y>
class marching_grid
{
private:
    std::array<float, node_count_x * node_count_y> _grid;

public:
    marching_grid() = default;

    void clear();

    size_t plain_id(size_t x, size_t y) const;
    void set(size_t x, size_t y, float val);
    float get(size_t x, size_t y) const;

    void add_contribution_bump(
        const vector2& point, float contribution, float max_x, float max_y);

    void draw(float screen_width, float screen_heigh) const;
    void march_all_cells(float screen_width, float screen_heigh);

    auto& grid() const
    {
        return _grid;
    }

private:
};

template<size_t node_count_x, size_t node_count_y>
inline void marching_grid<node_count_x, node_count_y>::clear()
{
    std::fill(_grid.begin(), _grid.end(), 0.0f);
}

template<size_t node_count_x, size_t node_count_y>
inline size_t marching_grid<node_count_x, node_count_y>::plain_id(size_t x, size_t y) const
{
    return y * node_count_x + x;
}

template<size_t node_count_x, size_t node_count_y>
inline void marching_grid<node_count_x, node_count_y>::set(size_t x, size_t y, float val)
{
    _grid[plain_id(x, y)] = val;
}

template<size_t node_count_x, size_t node_count_y>
inline float marching_grid<node_count_x, node_count_y>::get(size_t x, size_t y) const
{
    return _grid[plain_id(x, y)];
}

template<size_t node_count_x, size_t node_count_y>
inline void marching_grid<node_count_x, node_count_y>::add_contribution_bump(
    const vector2& point, float contribution, float max_x, float max_y)
{
    size_t centerX = math::map(point.x, 0.0f, max_x, (size_t)0, node_count_x);
    size_t centerY = math::map(point.y, 0.0f, max_y, (size_t)0, node_count_y);

    float cell_width = max_x / (node_count_x - 1);
    float cell_height = max_y / (node_count_y - 1);

    size_t radius_x = contribution / cell_width;
    size_t radius_y = contribution / cell_height;

    size_t low_x = math::limit<long long>(
        (long long)centerX - radius_x - 1,
        0,
        (node_count_x));

    size_t high_x = math::limit<long long>(
        centerX + radius_x + 1,
        0,
        (node_count_x));

    size_t low_y = math::limit<long long>(
        (long long)centerY - radius_y - 1,
        0,
        (node_count_y));

    size_t high_y = math::limit<long long>(
        centerY + radius_y + 1,
        0,
        (node_count_y));

    for (size_t y_id = low_y; y_id < high_y; y_id++)
    {
        float y = cell_height * y_id;

        for (size_t x_id = low_x; x_id < high_x; x_id++)
        {
            float x = cell_width * x_id;
            float r = std::sqrt(std::pow(point.x - x, 2) + std::pow(point.y - y, 2));

            if (std::abs(r) < contribution)
            {
                float contr_squared = contribution * contribution;
                float f = std::exp(-0.5 * contr_squared / (contr_squared - r * r));
                set(x_id, y_id, std::min(get(x_id, y_id) + f, 1.0f));
            }
        }
    }
}

template<size_t node_count_x, size_t node_count_y>
inline void marching_grid<node_count_x, node_count_y>::draw(float screen_width, float screen_heigh) const
{
    float cellWidth = screen_width / (node_count_x - 1);
    float cellHeight = screen_heigh / (node_count_y - 1);

    for (size_t yId = 0; yId < node_count_y; yId++)
    {
        float y = cellHeight * yId;

        for (size_t xId = 0; xId < node_count_x; xId++)
        {
            float x = cellWidth * xId;

            UCHAR intencity = math::map(get(xId, yId), 0.0f, 1.0f, 0, 255);
            draw::set_color(intencity, 0, 0);

            float saturation = math::map(get(xId, yId), 0.0f, 1.0f, 1, 10);
            draw::draw_point({x, y}, saturation);
        }
    }
}

template<size_t node_count_x, size_t node_count_y>
inline void marching_grid<node_count_x, node_count_y>::march_all_cells(
    float screen_width, float screen_heigh)
{
    float cellWidth = screen_width / (node_count_x - 1);
    float cellHeight = screen_heigh / (node_count_y - 1);
    const float treshold = 0.5f;

    struct Corner
    {
        float weight;
        vector2 loc;
    };

    for (int yId = 0; yId < node_count_y - 1; yId++)
    {
        float currentY = cellHeight * yId;

        for (int xId = 0; xId < node_count_x - 1; xId++)
        {
            float currentX = cellWidth * xId;

            Corner c1{get(xId, yId), vector2(currentX, currentY)};
            Corner c2{get(xId + 1, yId), vector2(currentX + cellWidth, currentY)};
            Corner c3{get(xId + 1, yId + 1), vector2(currentX + cellWidth, currentY + cellHeight)};
            Corner c4{get(xId, yId + 1), vector2(currentX, currentY + cellHeight)};

            float x, y;

            x = c1.loc.x + (c2.loc.x - c1.loc.x) * ((treshold - c1.weight) / (c2.weight - c1.weight));
            y = c1.loc.y;
            vector2 a(x, y);

            x = c2.loc.x;
            y = c2.loc.y + (c3.loc.y - c2.loc.y) * ((treshold - c2.weight) / (c3.weight - c2.weight));
            vector2 b(x, y);

            x = c4.loc.x + (c3.loc.x - c4.loc.x) * ((treshold - c4.weight) / (c3.weight - c4.weight));
            y = c4.loc.y;
            vector2 c(x, y);

            x = c1.loc.x;
            y = c1.loc.y + (c4.loc.y - c1.loc.y) * ((treshold - c1.weight) / (c4.weight - c1.weight));
            vector2 d(x, y);

            int var =
                (treshold < c1.weight) * 1 +
                (treshold < c2.weight) * 2 +
                (treshold < c3.weight) * 4 +
                (treshold < c4.weight) * 8;

            switch (var)
            {
                case 1: draw::draw_line(d, a); break;
                case 2: draw::draw_line(a, b); break;
                case 3: draw::draw_line(d, b); break;
                case 4: draw::draw_line(c, b); break;
                case 5: draw::draw_line(d, c); draw::draw_line(a, b); break;
                case 6: draw::draw_line(a, c); break;
                case 7: draw::draw_line(d, c); break;
                case 8: draw::draw_line(d, c); break;
                case 9: draw::draw_line(a, c); break;
                case 10: draw::draw_line(d, a); draw::draw_line(c, b); break;
                case 11: draw::draw_line(c, b); break;
                case 12: draw::draw_line(d, b); break;
                case 13: draw::draw_line(a, b); break;
                case 14: draw::draw_line(d, a); break;
            }
        }
    }
}
