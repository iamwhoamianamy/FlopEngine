#pragma once
#include <iostream>
#include <vector>

#include "GL/freeglut.h"

#include "vector2.hpp"
#include "quadtree.hpp"
#include "base_window.hpp"
#include "flock.hpp"

class BoidsWindow : public flp::BaseWindow
{
private:
    const size_t _flockCount = 3;
    std::vector<Flock> _flocks;
    const float _viscosity = 1;
    const size_t _boidPerFlock = 1000;
public:
    BoidsWindow(int argc, char** argv,
        float screenWidth, float screenHeight, std::string name);

    void display();
    void keyboardLetters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void mousePassive(int x, int y);
    void exitingFunction();
private:
};