#include "examples/empty/empty_window.hpp"
#include "libs/graphics/flop_engine.hpp"

int main(int argc, char** argv)
{
    flp::window = new flp::empty_window({argc, argv, 1600, 800, "Empty", 60, true});
    flp::window->run();
}
