#pragma once

#include "libs/graphics/base_window.hpp"

#include "libs/gui/master.hpp"
#include "libs/gui/button.hpp"
#include "libs/gui/split_layout.hpp"
#include "libs/gui/label.hpp"

#include "utils/singleton.hpp"

namespace flp
{

struct gui_window : public flp::base_window
{
private:
    std::vector<vector2> _points;

private:
    std::shared_ptr<gui::button> _button;
    std::shared_ptr<gui::split_layout> _main_split_layout;
    std::shared_ptr<gui::split_layout> _left_vertical_split_layout;

    std::vector<std::shared_ptr<gui::label>> _labels;

public:
    explicit gui_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;

    void mouse(int button, int state, int x, int y) override;

private:
    void setup_gui();
};

} // namespace flp