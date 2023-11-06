#include "pixel_grid_window.hpp"

#include "utils/utils.hpp"

#include "GL/freeglut.h"

pixel_grid_window::pixel_grid_window(int argc, char** argv,
    float screen_width, float screen_height, std::string name)
    : base_window{argc, argv, screen_width, screen_height, name}
{
}

void pixel_grid_window::physics_loop()
{

}

void pixel_grid_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    glDrawPixels(
        static_cast<size_t>(_screen_w),
        static_cast<size_t>(_screen_h),
        GL_RGB,
        GL_FLOAT,
        _pixels.data());

    glFinish();
}

void pixel_grid_window::resize(float w, float h)
{
    auto grid_width  = static_cast<size_t>(w);
    auto grid_height = static_cast<size_t>(h);

    auto pixel_count = grid_width * grid_height * 3;

    _pixels.resize(pixel_count);

    for (const auto x : utils::iota(grid_width))
    {
        for (const auto y : utils::iota(grid_height))
        {
            _pixels[x * 3 + y * grid_width * 3] = x / w;
        }
    }
}
