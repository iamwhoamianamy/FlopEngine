#pragma once
#include <iostream>
#include <mutex>

#include "GL/freeglut.h"

#include "vector2.hpp"
#include "quadtree.hpp"
#include "base_window.hpp"
#include "flock.hpp"
#include "marching_grid.hpp"

using marching_grid_t = marching_grid<500, 250>;

const auto boid_param_filename = "params.json";

class boids_window : public flp::base_window
{
public:
    static const size_t flock_count = 3;
    static const size_t boid_per_flock = 2000;

private:
    std::array<flock_t, flock_count> _flocks;
    const float _viscosity = 1;
    std::array<marching_grid_t, flock_count> _marching_grids;
    bool _drawBoids = true;
    bool _draw_marching_squares = false;
    bool _smooth = false;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::unique_lock<std::mutex> _lock;

    std::chrono::milliseconds _last_ellapsed = base_window::physics_interval;

public:
    boids_window(int argc, char** argv,
        float screen_width, float screen_height, std::string name);

    void display();
    void keyboard_letters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void mouse_passive(int x, int y);
    void exiting_function();

private:
    void start_watching_for_boid_param_file_change();
    void start_physics();
    void perform_flocking_physics();
    void perform_marching_physics();
    void watch_for_boid_param_file_change();
    void read_boid_params();
    void perform_physics_loop();
};