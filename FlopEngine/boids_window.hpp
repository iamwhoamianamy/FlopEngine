#pragma once
#include <iostream>
#include <mutex>

#include "GL/freeglut.h"

#include "vector2.hpp"
#include "quadtree.hpp"
#include "base_window.hpp"
#include "flock.hpp"
#include "marching_grid.hpp"

using marching_grid_t = MarchingGrid<500, 250>;

const auto boidParamFilename = "params.json";

class BoidsWindow : public flp::BaseWindow
{
public:
    static const size_t flockCount = 3;
    static const size_t boidPerFlock = 2000;

private:
    std::array<Flock, flockCount> _flocks;
    const float _viscosity = 1;
    std::array<marching_grid_t, flockCount> _marchingGrid;
    bool _drawBoids = true;
    bool _drawMarchingSquares = false;
    bool _smooth = false;

    std::mutex _mutex;
    std::condition_variable _cv;
    std::unique_lock<std::mutex> _lock;

public:
    BoidsWindow(int argc, char** argv,
        float screenWidth, float screenHeight, std::string name);

    void display();
    void keyboardLetters(unsigned char key, int x, int y);
    void mouse(int button, int state, int x, int y);
    void mousePassive(int x, int y);
    void exitingFunction();
private:
    void watchForBoidParamFileChange();

    void readBoidParams();
};