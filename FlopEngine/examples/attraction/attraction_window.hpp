#pragma once

#include "libs/graphics/base_window.hpp"

#include "utils/agent.hpp"
#include "libs/quadtree/quadtree.hpp"


struct attraction_window : public flp::base_window
{
private:
    std::vector<utils::agent> _agents;
    rectangle _agent_center;

public:
    attraction_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;

private:
    void attract(utils::agent& a, utils::agent& b);
    auto calc_agent_center() -> rectangle;
};