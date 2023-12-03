#pragma once

#include <vector>
#include <unordered_map>

#include "libs/graphics/base_window.hpp"
#include "libs/grid/grid.hpp"
#include "libs/graphics/color.hpp"

namespace flp
{

    enum class iteration_algorithm : size_t
{
    plane_iterator = 0,
    plane_for_each_seq,
    plane_for_each_par,
    classic,

    SIZE
};

inline auto iteration_algorithm_to_str =
std::unordered_map<iteration_algorithm, std::string>{
    {iteration_algorithm::plane_iterator, "plane_iterator"},
    {iteration_algorithm::plane_for_each_seq, "plane_for_each_seq"},
    {iteration_algorithm::plane_for_each_par, "plane_for_each_par"},
    {iteration_algorithm::classic, "classic"}
};

struct pixel_grid_window : public flp::base_window
{
private:
    grid<draw::color> _pixels;
    iteration_algorithm _iteration_algorithm{};

public:
    pixel_grid_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;
    void keyboard_letters(unsigned char key, int x, int y) override;

private:
};

} // namespace flp