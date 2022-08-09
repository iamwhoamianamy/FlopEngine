#include <iostream>
#include "flop_engine.hpp"
#include "octree_window.hpp"

int main(int argc, char** argv)
{
   window = new OctreeWindow(argc, argv, 800, 800, "Octree");
   window->run(30);
}
