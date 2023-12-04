#include <iostream>

#include "libs/graphics/flop_engine.hpp"
#include "examples/examples.hpp"

using namespace flp;

int main(int argc, char** argv)
{
    flp::window = new quadtree_window({argc, argv, 800, 800, "Quadtree"});
    //flp::window = new boids_window({argc, argv, 1600, 800, "Boids"});
    //flp::window = new marching_window({argc, argv, 1600, 800, "Marching"});
    //flp::window = new delaunay_window({argc, argv, 1600, 800, "Delaunay"});
    //flp::window = new pixel_grid_window({argc, argv, 1600, 800, "Pixel Grid"});
    //flp::window = new euler_fluid_window({argc, argv, 800, 800, "Pixel Grid"});
    //flp::window = new attraction_window({argc, argv, 1600, 800, "Attraction", 30, true});

    flp::window->run();
}
