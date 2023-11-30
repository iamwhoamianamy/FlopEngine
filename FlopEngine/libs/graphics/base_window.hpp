#pragma once

#include <iostream>
#include <vector>
#include <chrono>

#include "libs/geometry/vector2.hpp"
#include "libs/geometry/rectangle.hpp"
#include "utils/fixed_size_queue.hpp"

namespace flp
{

struct window_settings
{
    int argc;
    char** argv;

    float screen_width = 400;
    float screen_height = 400;

    std::string name = "New Window";
    float fps = 60.0f;
    bool fixed_timestep = false;
};

class base_window
{
protected:
    const std::chrono::microseconds _drawing_interval;

    std::chrono::microseconds _spent_on_iteration{};
    std::chrono::microseconds _last_ellapsed{};

    float _screen_w;
    float _screen_h;
    vector2 _mouse_pos;

    utils::fixed_size_queue<float> _fps_smother;

    bool _debug_mode = false;
    bool _fixed_timestamp;

protected:
    auto screen_rectangle() const -> rectangle;
    auto get_smooth_fps() const -> float;

public:
    base_window(window_settings&& settings);

    virtual void physics_loop();
    virtual void display() = 0;
    virtual void keyboard_letters(unsigned char key, int x, int y);
    virtual void mouse(int button, int state, int x, int y);
    virtual void mouse_passive(int x, int y);
    virtual void exiting_function();
    virtual void idle();
    virtual void resize(float w, float h);

    void run();

    friend void glutOnTimer(int millisec);
    friend void glutExitingFunction();
    friend void glutDisplay();
    friend void glutReshape(int w, int h);
    friend void glutKeyboardLetters(unsigned char key, int x, int y);
    friend void glutMouse(int button, int state, int x, int y);
    friend void glutMousePassive(int x, int y);
    friend void glutIdle();
    friend void glutSpecial(int key, int x, int y);

private:
    void base_display();
    void base_on_timer(int millisec);
    void base_exiting_function();
    void base_reshape(int w, int h);
    void base_idle();

    void base_keyboard_letters(unsigned char key, int x, int y);
    void base_mouse(int button, int state, int x, int y);
    void base_mouse_passive(int x, int y);
    void base_special(int key, int x, int y);
};

}