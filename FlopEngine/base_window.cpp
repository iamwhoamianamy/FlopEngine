#include "base_window.hpp"
#include "glut_functions.hpp"

using namespace flp;

base_window::base_window(
    int argc, char** argv,
    float screen_width, float screen_height,
    std::string name) :
    _screen_width(screen_width), _screen_height(screen_height), name(name)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(_screen_width, _screen_height);
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
        _last_ellapsed < drawing_interval
        ? drawing_interval - _last_ellapsed
        : std::chrono::milliseconds{0}};

    glutTimerFunc(left_in_loop.count(), glutOnTimer, 0);
}

void base_window::base_exiting_function()
{
    exiting_function();
}

void base_window::base_reshape(int w, int h)
{
    _screen_width = w;
    _screen_height = h;

    glViewport(0, 0, _screen_width, _screen_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, _screen_width, _screen_height, 0, -1.0, 1.0);
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

void base_window::base_display()
{
    auto start{std::chrono::steady_clock::now()};

    physics_loop();
    display();

    _last_ellapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
}