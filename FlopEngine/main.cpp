#include <iostream>

#include "flop_engine.hpp"
#include "quadtree_window.hpp"
#include "boids_window.hpp"

int main(int argc, char** argv)
{
    //flp::window = new QuadtreeWindow(argc, argv, 800, 800, "Octree");
    flp::window = new BoidsWindow(argc, argv, 1800, 900, "Boids");
    flp::window->run(60);
}
