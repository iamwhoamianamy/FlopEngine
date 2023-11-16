#include "pixel_grid_window.hpp"

#include <random>

#include "libs/graphics/drawing.hpp"
#include "utils/utils.hpp"

#include "GL/freeglut.h"

pixel_grid_window::pixel_grid_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
{
}

void pixel_grid_window::physics_loop()
{
    auto random_func =
        []()
        {
            static std::mt19937 engine;
            static std::uniform_real_distribution<float> dist{0.0, 1.0};

            return dist(engine);
        };

    switch (_iteration_algorithm)
    {
        case iteration_algorithm::plane_iterator:
        {
            for (auto& [val, x, y] : _pixels.as_plain_range())
            {
                val.r = random_func();
            }

            break;
        }
        case iteration_algorithm::plane_for_each_seq:
        {
            _pixels.for_each_plane(
                [=](auto& val, size_t x, size_t y, [[maybe_unused]] size_t i)
                {
                    val.r = random_func();
                });

            break;
        }
        case iteration_algorithm::plane_for_each_par:
        {
            _pixels.for_each_plane(std::move(std::execution::par),
                [=](auto& val, size_t x, size_t y, [[maybe_unused]] size_t i)
                {
                    val.r = random_func();
                });

            break;
        }
        case iteration_algorithm::classic:
        {
            for (const auto y : utils::iota(_pixels.height()))
            {
                for (const auto x : utils::iota(_pixels.width()))
                {
                    _pixels[x + y * _pixels.width()].r = random_func();
                }
            }

            break;
        }
    }
}

void pixel_grid_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    draw::draw_color_grid(_pixels);

    draw::set_color(draw::color::white());
    draw::render_string({0.0f, _screen_h}, 15, iteration_algorithm_to_str[_iteration_algorithm]);

    glFinish();
}

void pixel_grid_window::resize(float w, float h)
{
    auto grid_width  = static_cast<size_t>(w);
    auto grid_height = static_cast<size_t>(h);

    _pixels.resize(grid_width, grid_height);
}

void pixel_grid_window::keyboard_letters(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'n':
        {
            _iteration_algorithm = utils::next_enum(_iteration_algorithm);
        }
    }
}
