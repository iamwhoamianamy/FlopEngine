#pragma once
#include <iostream>
#include <vector>

#include "GL/freeglut.h"

#include "vector2.hpp"
#include "octree.hpp"
#include "base_window.hpp"

class OctreeWindow : public flp::BaseWindow
{
private:
    std::vector<Vector2> points;
public:
    OctreeWindow(int argc, char** argv,
        float screenWidth, float screenHeight, std::string name);

    void display();
    void keyboardLetters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void mousePassive(int x, int y);
    void exitingFunction();
private:
};