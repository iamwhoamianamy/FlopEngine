#include "marching_window.hpp"
#include "drawing.hpp"
#include "GL/freeglut.h"

MarchingWindow::MarchingWindow(
    int argc, char** argv,
    float screenWidth, float screenHeight,
    std::string name) :
    BaseWindow(argc, argv, screenWidth, screenHeight, name)
{
}

void MarchingWindow::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    _marchingGrid.addContributionBump(_mousePosition, 100, screenWidth, screenHeight);
    //_marchingGrid.addContributionReverseSquare(_mousePosition, 100, screenWidth, screenHeight);
    _marchingGrid.draw(screenWidth, screenHeight);
    _marchingGrid.marchAllCells(screenWidth, screenHeight);
    _marchingGrid.clear();

    draw::setColor(255);
    draw::drawPoint(_mousePosition, 10);

    glFinish();
}

void MarchingWindow::mousePassive(int x, int y)
{
    _mousePosition.x = x;
    _mousePosition.y = y;
}

void MarchingWindow::keyboardLetters(unsigned char key, int x, int y)
{
}

void MarchingWindow::mouse(int button, int state, int x, int y)
{
}

void MarchingWindow::exitingFunction()
{
}
