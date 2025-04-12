#include "examples/boids/boids_window.hpp"
#include "libs/graphics/flop_engine.hpp"

int main(int argc, char** argv)
{
    flp::window = new flp::boids_window({argc, argv, 1600, 800, "Boids", 60, true});
    flp::window->run();
}
