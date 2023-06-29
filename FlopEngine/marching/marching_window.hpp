#pragma once

#include <vector>
#include "libs/graphics/base_window.hpp"
#include "marching_grid.hpp"

class MarchingWindow : public flp::base_window
{
private:
    vector2 _mousePosition;
    marching_grid<500, 250> _marchingGrid;
public:
    MarchingWindow(
        int argc, char** argv,
        float _screen_width = 400, float _screen_height = 400,
        std::string name = "New Window");

    void display();
    void mouse_passive(int x, int y);
    void keyboard_letters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void exiting_function();
};