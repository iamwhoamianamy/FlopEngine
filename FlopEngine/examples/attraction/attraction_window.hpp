#pragma once

#include "libs/graphics/base_window.hpp"
#include "utils/agent.hpp"
#include "libs/quadtree/quadtree.hpp"

namespace flp
{

struct body : public utils::agent
{

};

struct attraction_window : public flp::base_window
{
private:
    std::vector<utils::agent> _agents;
    geo::rectangle _agent_center;
    std::vector<std::pair<vector2, vector2>> _edges;
    quadtree<utils::agent> _qtree;

public:
    attraction_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;

private:
    void attract(utils::agent& a, utils::agent& b);
    auto calc_agent_center() -> geo::rectangle;
};

} // namespace flp