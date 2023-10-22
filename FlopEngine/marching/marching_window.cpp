#include "marching_window.hpp"
#include "libs/graphics/drawing.hpp"
#include "GL/freeglut.h"

marching_window::marching_window(
    int argc, char** argv,
    float _screen_width, float _screen_height,
    std::string name) :
    base_window(argc, argv, _screen_width, _screen_height, name)
{
    _agents = utils::agent::generate_random(screen_rectangle(), 200, 20.0f);
}

void marching_window::physics_loop()
{
    for (auto& agent : _agents)
    {
        agent.bounce_from_borders(screen_rectangle());
        agent.update_position(1.0f, _last_ellapsed);

        _marching_grid.add_contribution_bump(agent.position, 20, _screen_w, _screen_h);
    }

    _marching_grid.add_contribution_bump(_mouse_pos, 20, _screen_w, _screen_h);
}

void marching_window::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    draw::set_color(draw::color::orange());

    for (auto& agent : _agents)
    {
        draw::draw_point(agent.position, 10.0f);
    }

    draw::set_color(draw::color::white());
    _marching_grid.draw(_screen_w, _screen_h);


    draw::set_color(draw::color::orange());
    _marching_grid.march_all_cells(_screen_w, _screen_h);
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
}

void marching_window::exiting_function()
{
}
