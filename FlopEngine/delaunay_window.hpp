#pragma once

#include "agent.hpp"
#include "base_window.hpp"

struct delaunay_window : public flp::base_window
{
private:
    static const size_t _agent_count;
    std::vector<utils::agent> _agents;

public:
    delaunay_window(int argc, char** argv,
        float screen_width, float screen_height, std::string name);

    void physics_loop() override;
    void display() override;
};