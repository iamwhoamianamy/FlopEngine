#pragma once

#include <iostream>
#include <vector>

#include "GL/freeglut.h"

#include "libs/geometry/vector2.hpp"
#include "libs/quadtree/quadtree.hpp"
#include "libs/graphics/base_window.hpp"

class QuadtreeWindow : public flp::base_window
{
private:
    std::vector<vector2> points;
public:
    QuadtreeWindow(int argc, char** argv,
        float _screen_width, float _screen_height, std::string name);

    void display();
    void keyboard_letters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void mouse_passive(int x, int y);
    void exiting_function();
private:
};