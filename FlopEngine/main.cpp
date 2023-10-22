#include <iostream>

#include "libs/graphics/flop_engine.hpp"
#include "quadtree/quadtree_window.hpp"
#include "boids/boids_window.hpp"
#include "marching/marching_window.hpp"
#include "delaunay/delaunay_window.hpp"

int main(int argc, char** argv)
{
    //flp::window = new quadtree_window(argc, argv, 800, 800, "Octree");
    flp::window = new boids_window(argc, argv, 1600, 800, "Boids");
    //flp::window = new marching_window(argc, argv, 1600, 800, "Marching");

    //flp::window = new delaunay_window(argc, argv, 1600, 800, "Delaunay");
    flp::window->run();
}
