#include <iostream>

#include "flop_engine.hpp"
#include "quadtree_window.hpp"
#include "boids_window.hpp"
#include "marching_window.hpp"
#include "delaunay_window.hpp"

int main(int argc, char** argv)
{
    //flp::window = new QuadtreeWindow(argc, argv, 800, 800, "Octree");
    //flp::window = new boids_window(argc, argv, 1600, 800, "Boids");
    //flp::window = new MarchingWindow(argc, argv, 1600, 800, "Boids");

    flp::window = new delaunay_window(argc, argv, 1600, 800, "Delaunay");
    flp::window->run();
}
