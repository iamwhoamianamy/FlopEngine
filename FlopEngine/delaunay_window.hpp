#pragma once

#include "agent.hpp"
#include "base_window.hpp"
#include "delaunay_triangulator.hpp"

struct delaunay_window : public flp::base_window
{
private:
    static const size_t _agent_count;
    std::vector<utils::agent> _agents;
    triangulation_t _triangulation;

public:
    delaunay_window(int argc, char** argv,
        float screen_width, float screen_height, std::string name);

    void physics_loop() override;
    void display() override;
};