#pragma once
#include <vector>
#include "base_window.hpp"
#include "marching_grid.hpp"

class MarchingWindow : public flp::BaseWindow
{
private:
    Vector2 _mousePosition;
    MarchingGrid<500, 250> _marchingGrid;
public:
    MarchingWindow(
        int argc, char** argv,
        float screenWidth = 400, float screenHeight = 400,
        std::string name = "New Window");

    void display();
    void mousePassive(int x, int y);
    void keyboardLetters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void exitingFunction();
};