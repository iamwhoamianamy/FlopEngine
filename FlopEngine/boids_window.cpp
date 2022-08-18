#include <thread>
#include <type_traits>
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
        flock.initRandomOnScreen(screenWidth, screenHeight, _boidPerFlock);
        flock.color() = draw::generateRandomColor();
    }

    _marchingGrid = MarchingGrid(500, 250);
}

void flockPhysics(
    std::vector<Flock>& _flocks, float screenWidth, float screenHeight, float FPS)
{
    for(auto& flock : _flocks)
    {
        flock.formQuadtree(
            Rect(
                { screenWidth / 2, screenHeight / 2 },
                { screenWidth / 2, screenHeight / 2 }),
            8);
        flock.performFlockingBehaviour(1.0 / FPS);
    }

    for(size_t i = 0; i < _flocks.size(); i++)
    {
        for(size_t j = 0; j < i; j++)
        {
            _flocks[i].performFleeing(_flocks[j], 1.0 / FPS);
            _flocks[j].performFleeing(_flocks[i], 1.0 / FPS);
        }
    }
}

void marchingPhysics(
    std::vector<Flock>& _flocks, MarchingGrid& _marchingGrid,
    float screenWidth, float screenHeight)
{
    for(const auto& flock : _flocks)
    {
        for(const auto& boid : flock.boids())
        {
            _marchingGrid.addContributionBump(boid.position, 15, screenWidth, screenHeight);
        }
    }
}

void BoidsWindow::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    std::thread flockPhysicsHandler(
        flockPhysics, std::ref(_flocks), screenWidth, screenHeight, FPS);

    std::thread marchingPhysicsHandler(
        marchingPhysics, std::ref(_flocks), std::ref(_marchingGrid), screenWidth, screenHeight);

    flockPhysicsHandler.join();
    marchingPhysicsHandler.join();

    for(auto& flock : _flocks)
    {
        flock.updateBoidPositions(_viscosity, 1.0 / FPS);
        flock.goThroughWindowBorders(screenWidth, screenHeight);
    }

    if(_drawBoids)
    {
        for(auto& flock : _flocks)
        {
            flock.draw();
        }
    }

    if(_drawMarchingSquares)
    {
        glLineWidth(2);
        draw::setColor(_flocks[0].color());
        _marchingGrid.marchAllCells(screenWidth, screenHeight);
    }

    _marchingGrid.clear();

    glFinish();
}

void BoidsWindow::keyboardLetters(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'c':
        {
            for(auto& flock : _flocks)
            {
                flock.color() = draw::generateRandomColor();
            }

            break;
        }
        case 'f':
        {
            glutFullScreenToggle();
            break;
        }
        case 'r':
        {
            for(auto& flock : _flocks)
            {
                flock.drawType() = (FlockDrawType)(
                    ((int)flock.drawType() + 1) % FLOCK_DRAW_TYPE_COUNT);
            }

            break;
        }
        case 'b':
        {
            _drawBoids = !_drawBoids;

            break;
        }
        case 'm':
        {
            _drawMarchingSquares = !_drawMarchingSquares;

            break;
        }
    }
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
