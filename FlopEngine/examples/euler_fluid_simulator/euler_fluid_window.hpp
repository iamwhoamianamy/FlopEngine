#pragma once

#include <vector>
#include <unordered_map>

#include "euler_fluid_simulator.hpp"
#include "libs/graphics/base_window.hpp"

struct euler_fluid_window : public flp::base_window
{
private:
    euler_fluid_simulator _simulator;

public:
    euler_fluid_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;
    void keyboard_letters(unsigned char key, int x, int y) override;

private:
};