#pragma once

#include "libs/graphics/base_window.hpp"

struct empty_window : public flp::base_window
{
private:

public:
    empty_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;
};