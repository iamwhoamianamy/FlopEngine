#pragma once

#include "marching_grid.hpp"

namespace flp
{

template<size_t NodeCountX, size_t NodeCountY>
marching_grid<NodeCountX, NodeCountY>::marching_grid(float width, float height)
{
    resize(width, height);
}

template<size_t NodeCountX, size_t NodeCountY>
void marching_grid<NodeCountX, NodeCountY>::resize(float width, float height)
{
    _width  = width;
    _height = height;

    _cell_width  = _width  / (NodeCountX - 1);
    _cell_height = _height / (NodeCountY - 1);
}

template<size_t NodeCountX, size_t NodeCountY>
void marching_grid<NodeCountX, NodeCountY>::clear()
{
    std::fill(_grid.begin(), _grid.end(), 0.0f);
}

template<size_t NodeCountX, size_t NodeCountY>
size_t marching_grid<NodeCountX, NodeCountY>::plain_id(size_t x, size_t y) const
{
    return y * NodeCountX + x;
}

template<size_t NodeCountX, size_t NodeCountY>
void marching_grid<NodeCountX, NodeCountY>::set(size_t x, size_t y, float val)
{
    _grid[plain_id(x, y)] = val;
}

template<size_t NodeCountX, size_t NodeCountY>
float marching_grid<NodeCountX, NodeCountY>::get(size_t x, size_t y) const
{
    return _grid[plain_id(x, y)];
}

template<size_t NodeCountX, size_t NodeCountY>
void marching_grid<NodeCountX, NodeCountY>::add_contribution_cone(
    const vector2& point, float contribution)
{
    const float low_x = math::limit(point.x - contribution - _cell_width, 0.0f, _width);
    const float low_y = math::limit(point.y - contribution - _cell_height, 0.0f, _height);

    const float high_x = math::limit(point.x + contribution + _cell_width, 0.0f, _width);
    const float high_y = math::limit(point.y + contribution + _cell_height, 0.0f, _height);

    const auto [low_x_id, low_y_id] = point_to_id(vector2{low_x, low_y});
    const auto [high_x_id, high_y_id] = point_to_id(vector2{high_x, high_y});

    for (size_t y_id = low_y_id; y_id < high_y_id; ++y_id)
    {
        const float y = _cell_height * y_id;

        for (size_t x_id = low_x_id; x_id < high_x_id; ++x_id)
        {
            const float x = _cell_width * x_id;

            const float d = vector2::distance(vector2{x, y}, point);
            const float f = std::max(0.0f, 1 - std::abs(d * (1 / contribution)));

            const float current = get(x_id, y_id);
            const float new_value = f + current;

            set(x_id, y_id, new_value);
        }
    }
}

template<size_t NodeCountX, size_t NodeCountY>
void marching_grid<NodeCountX, NodeCountY>::draw() const
{
    for (size_t y_id = 0; y_id < NodeCountY; y_id++)
    {
        const float y = _cell_height * y_id;

        for (size_t x_id = 0; x_id < NodeCountX; x_id++)
        {
            const float x = _cell_width * x_id;

            const float saturation = get(x_id, y_id) * 4.0f;

            draw::set_color(saturation, 0, 0);
            draw::draw_point(vector2{x, y}, saturation);
        }
    }
}

template<size_t NodeCountX, size_t NodeCountY>
void marching_grid<NodeCountX, NodeCountY>::march_all_cells(float threshold)
{
    struct Corner
    {
        float weight;
        vector2 loc;
    };

    for (int y_id = 0; y_id < NodeCountY - 1; y_id++)
    {
        const float cell_y = _cell_height * y_id;

        for (int x_id = 0; x_id < NodeCountX - 1; x_id++)
        {
            const float cell_x = _cell_width * x_id;

            Corner c1{get(x_id, y_id), vector2(cell_x, cell_y)};
            Corner c2{get(x_id + 1, y_id), vector2(cell_x + _cell_width, cell_y)};
            Corner c3{get(x_id + 1, y_id + 1), vector2(cell_x + _cell_width, cell_y + _cell_height)};
            Corner c4{get(x_id, y_id + 1), vector2(cell_x, cell_y + _cell_height)};

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

            const int var =
                (threshold < c1.weight) * 1 +
                (threshold < c2.weight) * 2 +
                (threshold < c3.weight) * 4 +
                (threshold < c4.weight) * 8;

            switch (var)
            {
                case 1:  draw::draw_line(d, a); break;
                case 2:  draw::draw_line(a, b); break;
                case 3:  draw::draw_line(d, b); break;
                case 4:  draw::draw_line(c, b); break;
                case 5:  draw::draw_line(d, c); draw::draw_line(a, b); break;
                case 6:  draw::draw_line(a, c); break;
                case 7:  draw::draw_line(d, c); break;
                case 8:  draw::draw_line(d, c); break;
                case 9:  draw::draw_line(a, c); break;
                case 10: draw::draw_line(d, a); draw::draw_line(c, b); break;
                case 11: draw::draw_line(c, b); break;
                case 12: draw::draw_line(d, b); break;
                case 13: draw::draw_line(a, b); break;
                case 14: draw::draw_line(d, a); break;
                default: break;
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
std::pair<size_t, size_t> marching_grid<NodeCountX, NodeCountY>::point_to_id(
    const vector2& p) const
{
    const size_t x = math::map(p.x, 0.0f, _width,  size_t{}, NodeCountX);
    const size_t y = math::map(p.y, 0.0f, _height, size_t{}, NodeCountY);

    return std::pair{x, y};
}

} // namespace flp::grid