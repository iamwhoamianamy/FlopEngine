#pragma once

#include "marching_grid.hpp"

template<size_t NodeCountX, size_t NodeCountY>
inline void marching_grid<NodeCountX, NodeCountY>::clear()
{
    std::fill(_grid.begin(), _grid.end(), 0.0f);
}

template<size_t NodeCountX, size_t NodeCountY>
inline size_t marching_grid<NodeCountX, NodeCountY>::plain_id(size_t x, size_t y) const
{
    return y * NodeCountX + x;
}

template<size_t NodeCountX, size_t NodeCountY>
inline void marching_grid<NodeCountX, NodeCountY>::set(size_t x, size_t y, float val)
{
    _grid[plain_id(x, y)] = val;
}

template<size_t NodeCountX, size_t NodeCountY>
inline float marching_grid<NodeCountX, NodeCountY>::get(size_t x, size_t y) const
{
    return _grid[plain_id(x, y)];
}

template<size_t NodeCountX, size_t NodeCountY>
inline void marching_grid<NodeCountX, NodeCountY>::add_contribution_cone(
    const vector2& point, float contribution, float max_x, float max_y)
{
    auto [center_x, center_y] = point_to_id(point, max_x, max_y);

    float cell_width  = max_x / (NodeCountX - 1);
    float cell_height = max_y / (NodeCountY - 1);

    float low_x = math::limit(point.x - contribution - cell_width,  0.0f, max_x);
    float low_y = math::limit(point.y - contribution - cell_height, 0.0f, max_y);

    float high_x = math::limit(point.x + contribution + cell_width,  0.0f, max_x);
    float high_y = math::limit(point.y + contribution + cell_height, 0.0f, max_y);

    auto [low_x_id, low_y_id]   = point_to_id({low_x, low_y},   max_x, max_y);
    auto [high_x_id, high_y_id] = point_to_id({high_x, high_y}, max_x, max_y);

    for (size_t y_id = low_y_id; y_id < high_y_id; y_id++)
    {
        float y = cell_height * y_id;

        for (size_t x_id = low_x_id; x_id < high_x_id; x_id++)
        {
            float x = cell_width * x_id;

            float d = vector2::distance({x, y}, point);
            float f = std::max(0.0f, 1 - std::abs(d * (1 / contribution)));

            float current = get(x_id, y_id);
            float new_value = f + current;

            set(x_id, y_id, new_value);
        }
    }
}

template<size_t NodeCountX, size_t NodeCountY>
inline void marching_grid<NodeCountX, NodeCountY>::draw(float screen_width, float screen_heigh) const
{
    float cell_w = screen_width / (NodeCountX - 1);
    float cell_h = screen_heigh / (NodeCountY - 1);

    for (size_t y_id = 0; y_id < NodeCountY; y_id++)
    {
        float y = cell_h * y_id;

        for (size_t x_id = 0; x_id < NodeCountX; x_id++)
        {
            float x = cell_w * x_id;

            auto saturation = get(x_id, y_id) * 4.0f;

            draw::set_color(saturation, 0, 0);
            draw::draw_point({x, y}, saturation);

            draw::set_color(draw::color::white());
            draw::render_string({x, y}, 5, std::format("{:.1}", get(x_id, y_id)));
        }
    }
}

template<size_t NodeCountX, size_t NodeCountY>
inline void marching_grid<NodeCountX, NodeCountY>::march_all_cells(
    float screen_width, float screen_heigh, float threshold)
{
    float cellWidth = screen_width / (NodeCountX - 1);
    float cellHeight = screen_heigh / (NodeCountY - 1);

    struct Corner
    {
        float weight;
        vector2 loc;
    };

    for (int y_id = 0; y_id < NodeCountY - 1; y_id++)
    {
        float currentY = cellHeight * y_id;

        for (int x_id = 0; x_id < NodeCountX - 1; x_id++)
        {
            float currentX = cellWidth * x_id;

            Corner c1{get(x_id, y_id), vector2(currentX, currentY)};
            Corner c2{get(x_id + 1, y_id), vector2(currentX + cellWidth, currentY)};
            Corner c3{get(x_id + 1, y_id + 1), vector2(currentX + cellWidth, currentY + cellHeight)};
            Corner c4{get(x_id, y_id + 1), vector2(currentX, currentY + cellHeight)};

            float x, y;

            x = c1.loc.x + (c2.loc.x - c1.loc.x) * ((threshold - c1.weight) / (c2.weight - c1.weight));
            y = c1.loc.y;
            vector2 a(x, y);

            x = c2.loc.x;
            y = c2.loc.y + (c3.loc.y - c2.loc.y) * ((threshold - c2.weight) / (c3.weight - c2.weight));
            vector2 b(x, y);

            x = c4.loc.x + (c3.loc.x - c4.loc.x) * ((threshold - c4.weight) / (c3.weight - c4.weight));
            y = c4.loc.y;
            vector2 c(x, y);

            x = c1.loc.x;
            y = c1.loc.y + (c4.loc.y - c1.loc.y) * ((threshold - c1.weight) / (c4.weight - c1.weight));
            vector2 d(x, y);

            int var =
                (threshold < c1.weight) * 1 +
                (threshold < c2.weight) * 2 +
                (threshold < c3.weight) * 4 +
                (threshold < c4.weight) * 8;

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

template<size_t NodeCountX, size_t NodeCountY>
const auto& marching_grid<NodeCountX, NodeCountY>::grid() const
{
    return _grid;
}

template<size_t NodeCountX, size_t NodeCountY>
inline auto marching_grid<NodeCountX, NodeCountY>::point_to_id(
    const vector2& p, float max_x, float max_y) const
    -> std::pair<size_t, size_t>
{
    size_t x = math::map(p.x, 0.0f, max_x, (size_t)0, NodeCountX);
    size_t y = math::map(p.y, 0.0f, max_y, (size_t)0, NodeCountY);

    return std::pair{x, y};
}
