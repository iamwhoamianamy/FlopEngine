#include "quadtree_window.hpp"

#include <format>

#include "libs/graphics/drawing.hpp"
#include "libs/quadtree/quadtree_help.hpp"
#include "utils/utils.hpp"

using namespace flp;

quadtree_window::quadtree_window(
    int argc, char** argv,
    float screen_width, float screen_height,
    std::string name) :
    base_window{argc, argv, screen_width, screen_height, name}
{
    size_t point_count = 10000;
    float step = 360.0f / point_count;
    float radius = 300;
    vector2 center = screen_rectangle().center;

    for (auto i : utils::iota(point_count))
    {
        auto x = center.x + radius * std::cosf(math::deg_to_rad(i * step));
        auto y = center.y + radius * std::sinf(math::deg_to_rad(i * step));

        _points.emplace_back(x, y);
    }

    _points.append_range(utils::generate_random(screen_rectangle(), 10000));

    _mouse_rectangle = {{}, {40, 40}};
}

void quadtree_window::keyboard_letters(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q':
        {
            _range_based_query = !_range_based_query;
            break;
        }
        case 'c':
        {
            _commit_qtree = !_commit_qtree;
            break;
        }
    }
}

void quadtree_window::mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case 0:
        {
            _points.emplace_back(static_cast<float>(x), static_cast<float>(y));
            break;
        }
        case 3:
        {
            _mouse_rectangle.half_dimensions += {5, 5};
            break;
        }
        case 4:
        {
            _mouse_rectangle.half_dimensions -= {5, 5};
            break;
        }
    }
}

void quadtree_window::mouse_passive(int x, int y)
{
    _mouse_pos = {static_cast<float>(x), static_cast<float>(y)};
    _mouse_rectangle.center = _mouse_pos;
}

void quadtree_window::exiting_function()
{
    std::cout << "DONE!";
}

void quadtree_window::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    _fps_smother.push(_last_ellapsed.count() / 1e6f);

    auto point_color = draw::color(255, 255, 255);

    draw::set_color(draw::color::red());
    draw::draw_rect(_mouse_pos, _mouse_rectangle.half_dimensions.x, _mouse_rectangle.half_dimensions.y);

    quadtree<vector2> qtree{
        rectangle{
            vector2(_screen_w / 2, _screen_h / 2),
            vector2(_screen_w / 2, _screen_h / 2)
        }
    };

    qtree.insert(_points);

    if (_commit_qtree)
    {
        qtree.commit();
    }

    draw_quadtree(qtree);

    for (auto& point : _points)
    {
        draw::set_color(point_color);
        draw::draw_point(point, 5);
    }

    auto found_point_color = draw::color(255, 0, 0);
    draw::set_color(found_point_color);

    size_t point_count = 0;

    if (_range_based_query)
    {
        for (const auto& point : qtree.quarry_as_range(_mouse_rectangle))
        {
            draw::draw_point(point, 5);
            point_count++;
        }
    }
    else
    {
        auto points = qtree.quarry(_mouse_rectangle);

        for (auto point : points)
        {
            draw::draw_point(*point, 5);
            point_count++;
        }
    }

    float fps = 0.0f;

    for (const auto& val : _fps_smother.values())
    {
        fps += val;
    }

    fps = 1.0f / (fps / _fps_smother.size());
    
    draw::set_color(draw::color::black());
    draw::draw_filled_rect(rectangle{{135, 50}, 135, 50});

    draw::set_color(draw::color::blue());
    draw::render_string({0, 15}, 15, std::format("fps: {:.3}", fps));
    draw::render_string({0, 35}, 15, std::format("point in range count: {}", point_count));
    draw::render_string({0, 55}, 15, std::format("range base query: {}", _range_based_query ? "enabled" : "disabled"));
    draw::render_string({0, 75}, 15, std::format("commit qtree: {}", _commit_qtree ? "enabled" : "disabled"));

    glFinish();
}
