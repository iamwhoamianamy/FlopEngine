#include "pixel_grid_window.hpp"

#include "utils/utils.hpp"

#include "GL/freeglut.h"

pixel_grid_window::pixel_grid_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
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
        GL_RGBA,
        GL_FLOAT,
        _pixels.data());

    glFinish();
}

void pixel_grid_window::resize(float w, float h)
{
    auto grid_width  = static_cast<size_t>(w);
    auto grid_height = static_cast<size_t>(h);

    _pixels.resize(grid_width, grid_height);

    for (const auto x : utils::iota(grid_width))
    {
        for (const auto y : utils::iota(grid_height))
        {
            _pixels[x + y * grid_width].r = x / w;
        }
    }
}
