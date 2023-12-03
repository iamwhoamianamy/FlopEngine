#pragma once

#include <vector>

#include "utils/agent.hpp"
#include "libs/graphics/base_window.hpp"
#include "libs/grid/marching_grid.hpp"

namespace flp
{

class marching_window : public flp::base_window
{
private:
    marching_grid<800, 400> _marching_grid;
    std::vector<utils::agent> _agents;
public:
    marching_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void mouse_passive(int x, int y);
    void keyboard_letters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void exiting_function();
    void resize(float w, float h) override;
};

} // namespace flp