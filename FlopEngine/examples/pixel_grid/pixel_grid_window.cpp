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

    for (auto& [val, x, y] : _pixels.as_plain_range())
    {
        val.r = math::random_normed();
    }

    //_pixels.for_each_plane(
    //    [=](auto& val, size_t x, size_t y, [[maybe_unused]] size_t i)
    //    {
    //        val.r = math::random_normed();
    //    });

    //for (const auto y : utils::iota(_pixels.height()))
    //{
    //    for (const auto x : utils::iota(_pixels.width()))
    //    {
    //        _pixels[x + y * _pixels.width()].r = math::random_normed();
    //    }
    //}

    glFinish();
}

void pixel_grid_window::resize(float w, float h)
{
    auto grid_width  = static_cast<size_t>(w);
    auto grid_height = static_cast<size_t>(h);

    _pixels.resize(grid_width, grid_height);
}
