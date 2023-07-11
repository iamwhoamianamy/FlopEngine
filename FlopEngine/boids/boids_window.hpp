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

//using marching_grid_t = marching_grid<500, 250>;
using marching_grid_t = marching_grid<200, 100>;

class boids_window : public flp::base_window
{
public:
    static const size_t flock_count = 3;
    static const size_t boid_per_flock = 1200;
    const std::string boid_param_filename = "boids/params.json";

private:
    std::array<flock_t, flock_count> _flocks;
    const float _viscosity = 0.9f;
    std::array<marching_grid_t, flock_count> _marching_grids;
    bool _drawBoids = true;
    bool _draw_marching_squares = false;
    bool _smooth = false;
    bool _bounce = false;

    utils::file_observer _boid_param_file_observer;

public:
    boids_window(int argc, char** argv,
        float screen_width, float screen_height, std::string name);

    void physics_loop() override;
    void display() override;
    void keyboard_letters(unsigned char key, int x, int y) override;

private:
    void perform_flocking_physics();
    void perform_marching_physics();
    void read_boid_params();
    void block_on_param_file_update();
    void draw_focks();
    void draw_marching_squares();
};