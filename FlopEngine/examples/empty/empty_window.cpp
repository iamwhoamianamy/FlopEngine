#include "empty_window.hpp"

#include "GL/freeglut.h"

using namespace flp;

empty_window::empty_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
{
}

void empty_window::physics_loop()
{

}

void empty_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glFinish();
}

void empty_window::resize(float w, float h)
{

}
