#pragma once

#include <vector>

#include "utils/agent.hpp"
#include "libs/graphics/base_window.hpp"
#include "libs/marching/marching_grid.hpp"

class marching_window : public flp::base_window
{
private:
    marching_grid<400, 200> _marching_grid;
    std::vector<utils::agent> _agents;
public:
    marching_window(
        int argc, char** argv,
        float _screen_width = 400, float _screen_height = 400,
        std::string name = "Marching Window");

    void physics_loop() override;
    void display() override;
    void mouse_passive(int x, int y);
    void keyboard_letters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void exiting_function();
    void resize(float w, float h) override;
}; 