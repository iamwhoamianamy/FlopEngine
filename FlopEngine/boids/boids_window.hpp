#pragma once

#include <iostream>
#include <mutex>

#include "GL/freeglut.h"

#include "libs/geometry/vector2.hpp"
#include "flock.hpp"
#include "libs/quadtree/quadtree.hpp"
#include "libs/graphics/base_window.hpp"
#include "marching/marching_grid.hpp"
#include "utils/file_observer.hpp"
#include "libs/gui/button.hpp"
#include "libs/gui/split_layout.hpp"
#include "libs/gui/input_param_box.hpp"

//using marching_grid_t = marching_grid<500, 250>;
using marching_grid_t = marching_grid<200, 100>;

class boids_window : public flp::base_window
{
public:
    static const size_t flock_count = 3;
    static const size_t boid_per_flock = 200;
    const std::string boid_param_filename = "boids/params.json";

private:
    std::array<flock, flock_count> _flocks;
    const float _viscosity = 1.0f;
    std::array<marching_grid_t, flock_count> _marching_grids;
    bool _draw_boids = true;
    bool _perform_marching_physics = false;
    bool _smooth = false;
    bool _bounce = false;

    utils::file_observer _boid_param_file_observer;

    std::shared_ptr<gui::input_param_box> _pmf_param_box;
    std::shared_ptr<gui::button> _right_button;
    std::shared_ptr<gui::split_layout> _main_split_layout;

public:
    boids_window(int argc, char** argv,
        float screen_width, float screen_height, std::string name);

    void physics_loop() override;
    void display() override;
    void keyboard_letters(unsigned char key, int x, int y) override;

private:
    void perform_flocking_physics();
    void perform_marching_physics();
    void update_boid_positions();
    void read_boid_params();
    void block_on_param_file_update();
    void draw_focks();
    void draw_marching_squares();
    void setup_gui();
};