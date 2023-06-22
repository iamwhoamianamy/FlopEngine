#include "GL/freeglut.h"

#include "delaunay_window.hpp"
#include "drawing.hpp"
#include "delaunay_triangulator.hpp"

const size_t delaunay_window::_agent_count{100};

delaunay_window::delaunay_window(int argc, char** argv,
    float screen_width, float screen_height, std::string name)
    : base_window{argc, argv, screen_width, screen_height, name}
    , _agents{utils::agent::generate_random(screen_rectangle(), _agent_count, 200)}
{
}

void delaunay_window::physics_loop()
{
    for (auto& agent : _agents)
    {
        agent.update_position(1.0f, _last_ellapsed);
        agent.bounce_from_borders(screen_rectangle());
    }

    _triangulation.clear();

    auto triangulation{triangulate(_agents)};
    _triangulation = {triangulation.begin(), triangulation.end()};
}

void delaunay_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    draw::color color{255, 200, 100};

    float max_true_length{screen_rectangle().diagonal() / 8};

    for (const auto& edge : _triangulation)
    {
        float width     {math::map(edge.perimeter(), 0.0f, max_true_length, 6.0f, 2.0f)};
        float saturation{math::map(edge.perimeter(), 0.0f, max_true_length, 100.0f, 20.0f)};

        draw::color color_fixed(
            2.5f * saturation,
            2.0 * saturation,
            1.0 * saturation);

        draw::set_color(color_fixed);
        draw::set_line_width(width);
        draw::draw_line(edge[0], edge[1]);
    }

    draw::set_color(color);
    for (const auto& agent : _agents)
    {
        draw::draw_point(agent.position, 12);
    }

    glFinish();
}
