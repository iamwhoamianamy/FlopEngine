#include "marching_window.hpp"
#include "drawing.hpp"

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

    draw::setColor();
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
