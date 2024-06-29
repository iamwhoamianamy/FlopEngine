#pragma once

#include "delaunay_triangulator.hpp"

#include "utils/agent.hpp"
#include "libs/graphics/base_window.hpp"

namespace flp
{

struct delaunay_window : public flp::base_window
{
private:
    static const size_t _agent_count;
    std::vector<utils::agent> _agents;
    triangulation_t _triangulation;
    std::unordered_map<vector2, std::pair<float, size_t>> _point_weights;

public:
    delaunay_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;

private:
    void calc_point_weights();
    void draw_with_edge_length();
    void draw_with_edge_length_gradient();
};

} // namespace flp