#include "boids_window.hpp"
#include "drawing.hpp"

BoidsWindow::BoidsWindow(
    int argc, char** argv,
    float screenWidth, float screenHeight,
    std::string name) :
    BaseWindow(argc, argv, screenWidth, screenHeight, name)
{
    _flocks.resize(_flockCount);

    for(auto& flock : _flocks)
    {
        flock.initRandomOnScreen(_boidsPerFlock, screenWidth, screenHeight);
    }
}

void BoidsWindow::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    for(auto& flock : _flocks)
    {
        flock.formQuadtree(Rect({ screenWidth / 2, screenHeight / 2 }, { screenWidth / 2, screenHeight / 2 }), 4);
        flock.performFlockingBehaviour(1.0 / FPS);

        flock.updateBoidPositions(_viscosity, 1.0 / FPS);
        flock.goThroughWindowBorders(screenWidth, screenHeight);

        flock.draw();
    }

    glFinish();
}

void BoidsWindow::keyboardLetters(unsigned char key, int x, int y)
{
}

void BoidsWindow::mouse(int button, int state, int x, int y)
{
}

void BoidsWindow::mousePassive(int x, int y)
{
}

void BoidsWindow::exitingFunction()
{
}
