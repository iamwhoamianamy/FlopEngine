#pragma once
#include <iostream>
#include <vector>
#include <chrono>

#include "vector2.hpp"

namespace flp
{

class base_window
{
protected:
    std::chrono::milliseconds _drawing_interval{1000 / 120};
    std::chrono::milliseconds _last_ellapsed;

    float _screen_w;
    float _screen_h;
    std::string _name;
    vector2 _mouse_pos;

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

    void run();



    friend void glutOnTimer(int millisec);
    friend void glutExitingFunction();
    friend void glutDisplay();
    friend void glutReshape(int w, int h);
    friend void glutKeyboardLetters(unsigned char key, int x, int y);
    friend void glutMouse(int button, int state, int x, int y);
    friend void glutMousePassive(int x, int y);
private:
    void base_display();
    void base_on_timer(int millisec);
    void base_exiting_function();
    void base_reshape(int w, int h);

    void base_keyboard_letters(unsigned char key, int x, int y);
    void base_mouse(int button, int state, int x, int y);
    void base_mouse_passive(int x, int y);
};

}