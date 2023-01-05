#include <thread>
#include <type_traits>
#include <filesystem>

#include "boids_window.hpp"
#include "drawing.hpp"

BoidsWindow::BoidsWindow(
    int argc, char** argv,
    float screenWidth, float screenHeight,
    std::string name) :
    BaseWindow(argc, argv, screenWidth, screenHeight, name)
{
    for(auto& flock : _flocks)
    {
        flock.initRandomOnScreen(screenWidth, screenHeight, _boidPerFlock);
        flock.color() = draw::generateRandomColor();
    }
}

void flockPhysics(
    std::array<Flock, flockCount>& flocks,
    float screenWidth, float screenHeight,
    float FPS)
{
    for(auto& flock : flocks)
    {
        flock.formQuadtree(
            Rect(
                { screenWidth / 2, screenHeight / 2 },
                { screenWidth / 2, screenHeight / 2 })
        );
        flock.performFlockingBehaviour(1.0 / FPS);
    }

    for(size_t i = 0; i < flocks.size(); i++)
    {
        for(size_t j = 0; j < i; j++)
        {
            flocks[i].performFleeing(flocks[j], 1.0 / FPS);
            flocks[j].performFleeing(flocks[i], 1.0 / FPS);
        }
    }
}

void marchingPhysics(
    std::array<Flock, flockCount>& flocks, 
    std::array<marching_grid_t, flockCount>& marchingGrids,
    float screenWidth, float screenHeight)
{
    for (size_t i = 0; i < flockCount; i++)
    {
        for (const auto& boid : flocks[i].boids())
        {
            marchingGrids[i].addContributionBump(boid.position, 15, screenWidth, screenHeight);
        }
    }
}

void BoidsWindow::watchForBoidParamFileChange()
{
    auto lastModified = std::filesystem::last_write_time(boidParamFilename);
    auto now = std::chrono::file_clock::now();
    auto deb = decltype(lastModified)::clock::time_point(lastModified);
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - deb);

    if (seconds.count() < 5)
    {
        readBoidParams();
    }
}

void BoidsWindow::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    watchForBoidParamFileChange();

    if (_drawMarchingSquares)
    {
        std::jthread flockPhysicsHandler(
            flockPhysics, std::ref(_flocks), screenWidth, screenHeight, FPS);

        std::jthread marchingPhysicsHandler(
            marchingPhysics, std::ref(_flocks), std::ref(_marchingGrid), screenWidth, screenHeight);
    }
    else
    {
        flockPhysics(_flocks, screenWidth, screenHeight, FPS);
    }

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
        glLineWidth(1);

        for (size_t i = 0; i < flockCount; i++)
        {
            draw::setColor(_flocks[i].color());
            _marchingGrid[i].marchAllCells(screenWidth, screenHeight);
            _marchingGrid[i].clear();
        }
    }

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
        case 's':
        {
            if (_smooth)
            {
                glDisable(GL_LINE_SMOOTH);
                glDisable(GL_POLYGON_SMOOTH);
            }
            else
            {
                glEnable(GL_LINE_SMOOTH);
                glEnable(GL_POLYGON_SMOOTH);
                glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
                glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
            }

            _smooth = !_smooth;

            break;
        }
        case 'o':
        {
            readBoidParams();
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

void BoidsWindow::readBoidParams()
{
    try
    {
        auto newParams = BoidParameters::readParamsFromFile(boidParamFilename);

        for (auto& flock : _flocks)
        {
            flock.setParams(newParams);
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
