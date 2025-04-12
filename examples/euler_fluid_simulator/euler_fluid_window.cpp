#include "euler_fluid_window.hpp"

#include <random>

#include "libs/graphics/drawing.hpp"
#include "utils/utils.hpp"

#include "GL/freeglut.h"

using namespace flp;

euler_fluid_window::euler_fluid_window(
    flp::window_settings&& settings)
    : base_window{std::move(settings)}
{
}

void euler_fluid_window::physics_loop()
{
    _simulator.update(_last_ellapsed);
}

void euler_fluid_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    draw::draw_value_grid(_simulator.as_saturation_grid());

    glFinish();
}

void euler_fluid_window::resize(float w, float h)
{
    auto grid_width  = static_cast<size_t>(w);
    auto grid_height = static_cast<size_t>(h);

    _simulator.resize(grid_width, grid_height);
}

void euler_fluid_window::keyboard_letters(unsigned char key, int x, int y)
{

}
