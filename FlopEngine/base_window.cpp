#include "base_window.hpp"
#include "glut_functions.hpp"

using namespace flp;

base_window::base_window(
    int argc, char** argv,
    float screen_width, float screen_height,
    std::string name) :
    _screen_w(screen_width), _screen_h(screen_height), _name(name)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(_screen_w, _screen_h);
    glutCreateWindow(name.c_str());

    registerFunctions();
}

void base_window::run()
{
    glutMainLoop();
}

void base_window::base_on_timer(int millisec)
{
    glutPostRedisplay();

    auto left_in_loop{
        _last_ellapsed < _drawing_interval
        ? _drawing_interval - _last_ellapsed
        : std::chrono::milliseconds{0}};

    glutTimerFunc(left_in_loop.count(), glutOnTimer, 0);
}

void base_window::base_exiting_function()
{
    exiting_function();
}

void base_window::base_reshape(int w, int h)
{
    _screen_w = w;
    _screen_h = h;

    glViewport(0, 0, _screen_w, _screen_h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, _screen_w, _screen_h, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void base_window::base_keyboard_letters(unsigned char key, int x, int y)
{
    keyboard_letters(key, x, y);
}

void base_window::base_mouse(int button, int state, int x, int y)
{
    mouse(button, state, x, y);
}

void base_window::base_mouse_passive(int x, int y)
{
    mouse_passive(x, y);
}

auto flp::base_window::screen_rectangle() const -> rectangle
{
    return rectangle{
        vector2{_screen_w / 2, _screen_h / 2},
        vector2{_screen_w / 2, _screen_h / 2}
    };
}

void base_window::base_display()
{
    auto start{std::chrono::steady_clock::now()};

    physics_loop();
    display();

    _last_ellapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
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