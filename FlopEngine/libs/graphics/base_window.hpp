#pragma once

#include <iostream>
#include <vector>
#include <chrono>

#include "libs/geometry/vector2.hpp"
#include "libs/geometry/rectangle.hpp"

namespace flp
{

class base_window
{
protected:
    std::chrono::microseconds _drawing_interval{1'000'000 / 120};
    std::chrono::microseconds _last_ellapsed{0};

    float _screen_w;
    float _screen_h;
    std::string _name;
    vector2 _mouse_pos;

    auto screen_rectangle() const -> rectangle;

public:
    base_window(
        int argc,
        char** argv,
        float screen_width = 400,
        float screen_height = 400,
        std::string name = "New Window");

    virtual void physics_loop();
    virtual void display() = 0;
    virtual void keyboard_letters(unsigned char key, int x, int y);
    virtual void mouse(int button, int state, int x, int y);
    virtual void mouse_passive(int x, int y);
    virtual void exiting_function();
    virtual void idle();

    void run();

    friend void glutOnTimer(int millisec);
    friend void glutExitingFunction();
    friend void glutDisplay();
    friend void glutReshape(int w, int h);
    friend void glutKeyboardLetters(unsigned char key, int x, int y);
    friend void glutMouse(int button, int state, int x, int y);
    friend void glutMousePassive(int x, int y);
    friend void glutIdle(int x, int y);

private:
    void base_display();
    void base_on_timer(int millisec);
    void base_exiting_function();
    void base_reshape(int w, int h);
    void base_idle();

    void base_keyboard_letters(unsigned char key, int x, int y);
    void base_mouse(int button, int state, int x, int y);
    void base_mouse_passive(int x, int y);
};

}