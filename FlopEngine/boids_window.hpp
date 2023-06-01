#pragma once
#include <iostream>
#include <mutex>

#include "GL/freeglut.h"

#include "vector2.hpp"
#include "flock.hpp"
#include "quadtree.hpp"
#include "base_window.hpp"
#include "marching_grid.hpp"
#include "file_observer.hpp"

using marching_grid_t = marching_grid<500, 250>;

class boids_window : public flp::base_window
{
public:
    static const size_t flock_count = 3;
    static const size_t boid_per_flock = 200;
    const std::string boid_param_filename = "params.json";

private:
    std::array<flock_t, flock_count> _flocks;
    const float _viscosity = 1;
    std::array<marching_grid_t, flock_count> _marching_grids;
    bool _drawBoids = true;
    bool _draw_marching_squares = false;
    bool _smooth = false;

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
};