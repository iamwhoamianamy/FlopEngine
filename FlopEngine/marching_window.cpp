#include "marching_window.hpp"
#include "drawing.hpp"
#include "GL/freeglut.h"

MarchingWindow::MarchingWindow(
    int argc, char** argv,
    float _screen_width, float _screen_height,
    std::string name) :
    base_window(argc, argv, _screen_width, _screen_height, name)
{
}

void MarchingWindow::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    _marchingGrid.add_contribution_bump(_mousePosition, 100, _screen_width, _screen_height);
    //_marchingGrid.addContributionReverseSquare(_mousePosition, 100, screenWidth, screenHeight);
    _marchingGrid.draw(_screen_width, _screen_height);
    _marchingGrid.march_all_cells(_screen_width, _screen_height);
    _marchingGrid.clear();

    draw::set_color(255);
    draw::draw_point(_mousePosition, 10);

    glFinish();
}

void MarchingWindow::mouse_passive(int x, int y)
{
    _mousePosition.x = x;
    _mousePosition.y = y;
}

void MarchingWindow::keyboard_letters(unsigned char key, int x, int y)
{
}

void MarchingWindow::mouse(int button, int state, int x, int y)
{
}

void MarchingWindow::exiting_function()
{
}
