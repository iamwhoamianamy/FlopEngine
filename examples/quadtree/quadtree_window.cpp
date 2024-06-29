#include "quadtree_window.hpp"

#include <format>

#include "libs/graphics/drawing.hpp"
#include "libs/quadtree/quadtree_help.hpp"
#include "utils/utils.hpp"

using namespace flp;

quadtree_window::quadtree_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
{
    /*size_t point_count = 10;
    float step = 360.0f / point_count;
    float radius = 300;
    vector2 center = screen_rectangle().center;

    for (auto i : utils::iota(point_count))
    {
        auto x = center.x + radius * std::cosf(math::deg_to_rad(i * step));
        auto y = center.y + radius * std::sinf(math::deg_to_rad(i * step));

        _points.emplace_back(x, y);
    }

    _points.append_range(utils::generate_random(screen_rectangle(), 10));*/

    _mouse_rectangle = {{}, {40, 40}};
}

void quadtree_window::keyboard_letters(unsigned char key, int x, int y)
{
    switch (key)
    {
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
            if (state != 0)
                return;

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

template<typename Point, typename Node>
void draw_qtree(const quadtree<Point, Node>& qtree)
{
    draw::set_line_width(2.0f);

    qtree.traverse_by_width(
        [](const auto* node)
        {
            if (!node->empty())
            {
                draw::set_color(draw::color::red());
                size_t level = node->level();

                geo::rectangle boundary = node->boundary();
                boundary.half_dimensions -= vector2{1.0f, 1.0f} * level;

                draw::draw_rect(boundary);
            }
            else
            {
                draw::set_color(draw::color::white());
                draw::draw_rect(node->boundary());
            }
        });

    draw::set_line_width(1.0f);
    draw::set_color(draw::color::green());

    qtree.traverse_by_width_reverse(
        [](const auto* node)
        {
            if (!node->subdivided())
                node->data = node->points().size();
            else
            {
                for (const auto& child : node->children())
                    node->data += child.get()->data;
            }

            draw::render_string(
                node->boundary().center,
                15.0f,
                std::to_string(node->data));
        });
}

void quadtree_window::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    _fps_smother.push(_last_ellapsed.count() / 1e6f);

    auto point_color = draw::color(255, 255, 255);

    draw::set_color(draw::color::blue());
    draw::draw_rect(_mouse_pos, _mouse_rectangle.half_dimensions.x, _mouse_rectangle.half_dimensions.y);

    quadtree<vector2, quadtree_node_with_data<vector2, size_t>> qtree{
        geo::rectangle{
            vector2(_screen_w / 2, _screen_h / 2),
            vector2(_screen_w / 2, _screen_h / 2)
        }
    };

    qtree.insert(_points);

    if (_commit_qtree)
    {
        qtree.commit();
    }

    draw_qtree(qtree);

    for (auto& point : _points)
    {
        draw::set_color(point_color);
        draw::draw_point(point, 5);
    }

    draw::set_color(draw::color::blue());

    size_t point_count = 0;

    for (const auto& point : qtree.quarry(_mouse_rectangle))
    {
        draw::draw_point(point, 5);
        point_count++;
    }

    float fps = 0.0f;

    for (const auto& val : _fps_smother.values())
    {
        fps += val;
    }

    fps = 1.0f / (fps / _fps_smother.size());
    
    draw::set_color(draw::color::black());
    draw::draw_filled_rect(geo::rectangle{{135, 50}, 135, 50});

    draw::set_color(draw::color::blue());
    draw::render_string({0, 15}, 15, std::format("fps: {:.3}", fps));
    draw::render_string({0, 35}, 15, std::format("point in range count: {}", point_count));
    draw::render_string({0, 75}, 15, std::format("commit qtree: {}", _commit_qtree ? "enabled" : "disabled"));

    glFinish();
}
