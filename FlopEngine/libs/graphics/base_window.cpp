#include "base_window.hpp"

#include <format>

#include "glut_functions.hpp"

#include "utils/singleton.hpp"
#include "libs/gui/master.hpp"
#include "libs/logger/logger.hpp"
#include "libs/graphics/drawing.hpp"

using namespace flp;

base_window::base_window(window_settings&& settings)
    : _screen_w{settings.screen_width}
    , _screen_h{settings.screen_height}
    , _drawing_interval{static_cast<int>(1'000'000 / settings.fps)}
    , _fixed_timestamp{settings.fixed_timestep}
{
    glutInit(&settings.argc, settings.argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_ALPHA);
    glutInitWindowSize(static_cast<int>(_screen_w), static_cast<int>(_screen_h));
    glutCreateWindow(settings.name.c_str());

    registerFunctions();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    utils::singleton<gui::master>::get();
}

void base_window::run()
{
    glutMainLoop();
}

void base_window::base_on_timer(int millisec)
{
    glutPostRedisplay();

    auto left_in_loop = std::chrono::duration_cast<std::chrono::milliseconds>(
        _spent_on_iteration < _drawing_interval
        ? _drawing_interval - _spent_on_iteration
        : std::chrono::microseconds{0});

    if (_fixed_timestamp)
    {
        _last_ellapsed = _drawing_interval;
    }
    else
    {
        _last_ellapsed = std::max(_drawing_interval, _spent_on_iteration);
    }

    _fps_smother.push(_last_ellapsed.count() / 1e6f);

    //logger::log_trace(
    //    "spent_on_iteration: {}, left in loop: {}, last ellapsed: {}, ",
    //    _spent_on_iteration
    //    , left_in_loop,
    //    _last_ellapsed);

    glutTimerFunc(
        static_cast<unsigned int>(left_in_loop.count()),
        glutOnTimer, 0);
}

void base_window::base_exiting_function()
{
    exiting_function();
}

void base_window::base_reshape(int w, int h)
{
    _screen_w = static_cast<float>(w);
    _screen_h = static_cast<float>(h);

    utils::singleton<gui::master>::get().resize(screen_rectangle());

    glViewport(0, 0, static_cast<GLsizei>(_screen_w), static_cast<GLsizei>(_screen_h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, _screen_w, _screen_h, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    resize(static_cast<float>(w), static_cast<float>(h));
}

void flp::base_window::base_idle()
{
    idle();
}

void base_window::base_keyboard_letters(unsigned char key, int x, int y)
{
    utils::singleton<gui::master>::get().react_on_keyboard_key_press(key);
    keyboard_letters(key, x, y);
}

void base_window::base_mouse(int button, int state, int x, int y)
{
    _mouse_pos = {static_cast<float>(x), static_cast<float>(y)};
    mouse(button, state, x, y);
    utils::singleton<gui::master>::get().register_mouse_click_status_change(_mouse_pos);
}

void base_window::base_mouse_passive(int x, int y)
{
    _mouse_pos = {static_cast<float>(x), static_cast<float>(y)};
    mouse_passive(x, y);
    utils::singleton<gui::master>::get().hover(_mouse_pos);
}

void flp::base_window::base_special(int key, int x, int y)
{
    switch (key)
    {
        case 3:
        {
            _debug_mode = !_debug_mode;
            break;
        }
    }
}

auto flp::base_window::screen_rectangle() const -> rectangle
{
    return rectangle{
        vector2{_screen_w / 2, _screen_h / 2},
        vector2{_screen_w / 2, _screen_h / 2}
    };
}

auto flp::base_window::get_smooth_fps() const -> float
{
    float fps = 0.0f;

    for (const auto& val : _fps_smother.values())
        fps += val;

    return 1.0f / (fps / _fps_smother.size());
}

void base_window::base_display()
{
    auto start = std::chrono::steady_clock::now();

    physics_loop();
    display();
    utils::singleton<gui::master>::get().draw();

    _spent_on_iteration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start);

    if (_debug_mode)
    {
        draw::set_color(draw::color::black());
        draw::draw_filled_rect(rectangle{{135, 10}, 135, 10});

        draw::set_color(draw::color::blue());
        draw::render_string({0, 15}, 15, std::format("fps: {:.3}", get_smooth_fps()));
    }

    glutSwapBuffers();
}

void base_window::physics_loop()
{

}

void base_window::keyboard_letters(unsigned char key, int x, int y)
{

}

void base_window::mouse(int button, int state, int x, int y)
{

}

void base_window::mouse_passive(int x, int y)
{

}

void base_window::exiting_function()
{

}

void flp::base_window::idle()
{

}

void flp::base_window::resize(float x, float y)
{

}
