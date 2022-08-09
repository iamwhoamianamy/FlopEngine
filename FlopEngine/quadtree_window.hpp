#pragma once
#include <iostream>
#include <vector>

#include "GL/freeglut.h"

#include "vector2.hpp"
#include "quadtree.hpp"
#include "base_window.hpp"

template<>
class OctreePointHolder<Vector2>
{
public:
    static float x(Vector2* vec)
    {
        return vec->x;
    }

    static float y(Vector2* vec)
    {
        return vec->y;
    }
};

class QuadtreeWindow : public flp::BaseWindow
{
private:
    std::vector<Vector2> points;
public:
    QuadtreeWindow(int argc, char** argv,
        float screenWidth, float screenHeight, std::string name);

    void display();
    void keyboardLetters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void mousePassive(int x, int y);
    void exitingFunction();
private:
};