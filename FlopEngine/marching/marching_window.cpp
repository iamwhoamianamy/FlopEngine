#include "marching_window.hpp"
#include "libs/graphics/drawing.hpp"
#include "GL/freeglut.h"

marching_window::marching_window(
    int argc, char** argv,
    float screen_width, float screen_height,
    std::string name)
    : base_window(argc, argv, screen_width, screen_height, name)
    , _marching_grid(_screen_w, _screen_h)
{
    _agents = utils::agent::generate_random(screen_rectangle(), 200, 30.0f);
}

void marching_window::physics_loop()
{
    for (auto& agent : _agents)
    {
        agent.bounce_from_borders(screen_rectangle());
        agent.update_position(1.0f, _last_ellapsed);

        _marching_grid.add_contribution_cone(agent.position, 30.0f);
    }

    _marching_grid.add_contribution_cone(_mouse_pos, 100.0f);
}

void marching_window::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    //draw::set_color(draw::color::purple());

    //for (auto& agent : _agents)
    //{
    //    draw::draw_point(agent.position, 10.0f);
    //}

    //draw::set_color(draw::color::white());
    //_marching_grid.draw(_screen_w, _screen_h);

    draw::set_color(draw::color::purple());
    _marching_grid.march_all_cells(0.1f);

    draw::set_color(draw::color::blue());
    _marching_grid.march_all_cells(0.2f);

    draw::set_color(0.0f, 1.0f, 1.0f);
    _marching_grid.march_all_cells(0.3f);

    draw::set_color(draw::color::green());
    _marching_grid.march_all_cells(0.4f);

    draw::set_color(0.75f, 1.0f, 0.75f);
    _marching_grid.march_all_cells(0.5f);

    draw::set_color(draw::color::yellow());
    _marching_grid.march_all_cells(0.6f);

    draw::set_color(draw::color::orange());
    _marching_grid.march_all_cells(0.7f);

    draw::set_color(1.0f, 0.75f, 0.0f);
    _marching_grid.march_all_cells(0.8f);

    draw::set_color(draw::color::red());
    _marching_grid.march_all_cells(0.9f);


    _marching_grid.clear();

    glFinish();
}

void marching_window::mouse_passive(int x, int y)
{

}

void marching_window::keyboard_letters(unsigned char key, int x, int y)
{
}

void marching_window::mouse(int button, int state, int x, int y)
{
    _agents.emplace_back(_mouse_pos, math::generate_random_vector() * _agents.back().velocity.length());
}

void marching_window::exiting_function()
{
}

void marching_window::resize(float w, float h)
{
    _marching_grid.resize(w, h);
}
