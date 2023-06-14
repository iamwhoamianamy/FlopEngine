#include "GL/freeglut.h"

#include "delaunay_window.hpp"
#include "drawing.hpp"
#include "delaunay_triangulator.hpp"

const size_t delaunay_window::_agent_count{15};

delaunay_window::delaunay_window(int argc, char** argv,
    float screen_width, float screen_height, std::string name)
    : base_window{argc, argv, screen_width, screen_height, name}
    , _agents{utils::agent::generate_random(screen_rectangle(), _agent_count, 400)}
{
}

void delaunay_window::physics_loop()
{
    for (auto& agent : _agents)
    {
        agent.update_position(1.0f, _last_ellapsed);
        agent.bounce_from_borders(screen_rectangle());
    }

    delaunay_triangulator tor;
}

void delaunay_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    draw::set_color(255, 200, 100);
    for (const auto& agent : _agents)
    {
        draw::draw_point(agent.position, 5);
    }

    triangle tr(_agents[0].position, _agents[1].position, _agents[2].position);

    draw::set_color(255, 255, 255);

    for (const auto& triangle : delaunay_triangulator{}.triangulate(_agents))
    {
        draw::draw_triangle(triangle[0], triangle[1], triangle[2]);
    }

    glFinish();
}