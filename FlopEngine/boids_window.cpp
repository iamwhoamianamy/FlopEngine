#include <thread>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <execution>

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
        flock.initRandomOnScreen(screenWidth, screenHeight, boidPerFlock);
        flock.color() = draw::generateRandomColor();
    }

    readBoidParams();
    startWatchingForBoidParamFileChange();
    startPhysics();
}

void BoidsWindow::startWatchingForBoidParamFileChange()
{
    _lock = std::unique_lock(_mutex, std::defer_lock);
    std::jthread([this]()
        {
            watchForBoidParamFileChange();
        }).detach();
}

void BoidsWindow::startPhysics()
{
    std::jthread([this]
        {
            while (true)
            {
                performPhysicsLoop();
            }
        }).detach();
}

void BoidsWindow::performFlockingPhysics()
{
    std::for_each(std::execution::par, _flocks.begin(), _flocks.end(),
        [=](Flock& flock)
        {
            flock.formQuadtree(
                Rect(
                    {screenWidth / 2, screenHeight / 2},
                    {screenWidth / 2, screenHeight / 2})
            );
            flock.performFlockingBehaviour(_lastEllapsed);
        });

    for (size_t i = 0; i < _flocks.size(); i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            _flocks[i].performFleeing(_flocks[j], _lastEllapsed);
            _flocks[j].performFleeing(_flocks[i], _lastEllapsed);
        }
    }
}

void BoidsWindow::performMarchingPhysics()
{
    for (size_t i = 0; i < BoidsWindow::flockCount; i++)
    {
        for (const auto& boid : _flocks[i].boids())
        {
            _marchingGrids[i].addContributionBump(boid.position, 15, screenWidth, screenHeight);
        }
    }
}

void BoidsWindow::performPhysicsLoop()
{
    auto start = std::chrono::steady_clock::now();
    auto desirable_end = start + physics_interval;
    performFlockingPhysics();

    for (auto& flock : _flocks)
    {
        flock.updateBoidPositions(_viscosity, _lastEllapsed);
        flock.goThroughWindowBorders(screenWidth, screenHeight);
    }

    std::this_thread::sleep_until(desirable_end);
    _lastEllapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
}

void BoidsWindow::watchForBoidParamFileChange()
{
    while (true)
    {
        auto lastModified = std::filesystem::last_write_time(boidParamFilename);
        auto now = std::chrono::file_clock::now();
        auto deb = decltype(lastModified)::clock::time_point(lastModified);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - deb);

        if (seconds.count() < 5)
        {
            if (_lock.try_lock())
            {
                readBoidParams();

                _lock.unlock();
                _cv.notify_all();
            }
            else
            {
                _cv.wait(_lock);

                _lock.lock();
                readBoidParams();
                _lock.unlock();
                _cv.notify_all();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void BoidsWindow::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    
    try
    {
        if (_lock.try_lock())
        {
            _lock.unlock();
            _cv.notify_all();
        }
        else
        {
            _cv.wait(_lock);
        }
    }
    catch(...)
    {

    }

    //if (_drawMarchingSquares)
    //{
    //    std::jthread flockPhysicsHandler(
    //        flockPhysics, std::ref(_flocks), screenWidth, screenHeight, FPS);

    //    std::jthread marchingPhysicsHandler(
    //        marchingPhysics, std::ref(_flocks), std::ref(_marchingGrid), screenWidth, screenHeight);
    //}
    //else
    //{
    //    flockPhysics(_flocks, screenWidth, screenHeight, FPS);
    //}

    //for(auto& flock : _flocks)
    //{
    //    flock.updateBoidPositions(_viscosity, 1.0 / FPS);
    //    flock.goThroughWindowBorders(screenWidth, screenHeight);
    //}

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
            _marchingGrids[i].marchAllCells(screenWidth, screenHeight);
            _marchingGrids[i].clear();
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