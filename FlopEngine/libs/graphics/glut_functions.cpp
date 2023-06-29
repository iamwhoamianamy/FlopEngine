#pragma once

#include "glut_functions.hpp"
#include "flop_engine.hpp"

namespace flp
{
    base_window* window;

    void registerFunctions()
    {
        glutDisplayFunc(glutDisplay);
        glutReshapeFunc(glutReshape);
        glutKeyboardFunc(glutKeyboardLetters);
        glutMouseFunc(glutMouse);
        glutPassiveMotionFunc(glutMousePassive);
        atexit(glutExitingFunction);
        glutTimerFunc(0, glutOnTimer, 0);
    }

    void glutOnTimer(int millisec)
    {
        if(window)
            window->base_on_timer(millisec);
    }

    void glutExitingFunction()
    {
        if(window)
            window->base_exiting_function();
    }

    void glutDisplay()
    {
        if(window)
            window->base_display();
    }

    void glutReshape(int w, int h)
    {
        if(window)
            window->base_reshape(w, h);
    }

    void glutKeyboardLetters(unsigned char key, int x, int y)
    {
        if(window)
            window->base_keyboard_letters(key, x, y);
    }

    void glutMouse(int button, int state, int x, int y)
    {
        if(window)
            window->base_mouse(button, state, x, y);
    }

    void glutMousePassive(int x, int y)
    {
        if(window)
            window->base_mouse_passive(x, y);
    }
}