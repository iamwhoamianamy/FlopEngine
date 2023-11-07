#pragma once

#include <iostream>
#include <vector>

#include "GL/freeglut.h"

#include "libs/geometry/vector2.hpp"
#include "libs/quadtree/quadtree.hpp"
#include "libs/graphics/base_window.hpp"
#include "utils/fixed_size_queue.hpp"

class quadtree_window : public flp::base_window
{
private:
    std::vector<vector2> _points;
    bool _range_based_query = false;
    bool _commit_qtree = false;
    rectangle _mouse_rectangle;
    utils::fixed_size_queue<float, 10> _fps_smother;

public:
    quadtree_window(flp::window_settings&& settings);

    void display();
    void keyboard_letters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void mouse_passive(int x, int y);
    void exiting_function();
private:
};