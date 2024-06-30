#include "gui_window.hpp"

#include <libs/graphics/drawing.hpp>

#include "GL/freeglut.h"

using namespace flp;

gui_window::gui_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
{
    setup_gui();
}

void gui_window::physics_loop()
{

}

void gui_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    draw::set_color(draw::color::white());
    draw::draw_points<vector2>(_points, 10.0f);

    glFinish();
}

void gui_window::resize(float w, float h)
{

}

void gui_window::mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
        {
            _points.emplace_back(x, y);
        }
    }
}

void gui_window::setup_gui()
{
    for (auto i : utils::iota(10))
        _labels.emplace_back(gui::label::create("label number " + std::to_string(i)));

    _left_vertical_split_layout = gui::split_layout::create(
        gui::split_layout::orientation::vertical,
        gui::split_layout::init_container{
            {_labels[0], gui::split_layout::init_container::markup::absolute, 100},
            {_labels[1], gui::split_layout::init_container::markup::absolute, 100},
            {_labels[2], gui::split_layout::init_container::markup::absolute, 100},
            {_labels[3], gui::split_layout::init_container::markup::absolute, 100},
            {_labels[4], gui::split_layout::init_container::markup::absolute, 100},
            {_labels[5], gui::split_layout::init_container::markup::absolute, 100},
        }
    );

    // button
    _button = gui::button::create("Le button");

    // split layout
    _main_split_layout = gui::split_layout::create(
        gui::split_layout::orientation::horizontal,
        gui::split_layout::init_container{
            {_left_vertical_split_layout, gui::split_layout::init_container::markup::relative, 0.3f},
            {_button, gui::split_layout::init_container::markup::relative, 0.7f}
        }
    );

    // finilize
    utils::singleton<gui::master>::get().set_layout(_main_split_layout.get());
}




