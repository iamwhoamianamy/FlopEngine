#include "examples/marching/marching_window.hpp"
#include "libs/graphics/flop_engine.hpp"

int main(int argc, char** argv)
{
    flp::window = new flp::marching_window({argc, argv, 1600, 800, "Marching", 144, false});
    flp::window->run();
}
