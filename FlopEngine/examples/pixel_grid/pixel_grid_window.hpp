#pragma once

#include <vector>

#include "libs/graphics/base_window.hpp"

struct pixel_grid_window : public flp::base_window
{
private:
    std::vector<float> _pixels;

public:
    pixel_grid_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;

private:
};