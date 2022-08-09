#include "octree_window.hpp"
#include "drawing.hpp"
#include "octree_help.hpp"

using namespace flp;

OctreeWindow::OctreeWindow(
    int argc, char** argv,
    float screenWidth, float screenHeight,
    std::string name) :
    BaseWindow(argc, argv, screenWidth, screenHeight, name)
{

}

void OctreeWindow::keyboardLetters(unsigned char key, int x, int y)
{

}

void OctreeWindow::mouse(int button, int state, int x, int y)
{
    if(state == 0)
    {
        points.emplace_back(x, y);
    }
}

void OctreeWindow::mousePassive(int x, int y)
{
    mousePos.x = x;
    mousePos.y = y;
}

void OctreeWindow::exitingFunction()
{
    std::cout << "DONE!";
}

void OctreeWindow::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    auto pointColor = draw::Color(255, 255, 255);

    glEnable(GL_POINT_SMOOTH);

    for(auto& point : points)
    {
        draw::drawPoint(point, pointColor, 5);
    }

    Rect mouseBox(mousePos, Vector2(40, 40));
    draw::drawRect(mousePos, mouseBox.halfDimensions.x, mouseBox.halfDimensions.y, pointColor);

    Octree<Vector2> octree(
        Rect(
            Vector2(screenWidth / 2, screenHeight / 2),
            Vector2(screenWidth / 2, screenHeight / 2)),
        1);
    std::vector<Vector2*> foundPoints;

    octree.insert(points);
    octree.quarry(mouseBox, foundPoints);

    auto foundColor = draw::Color(255, 0, 0);

    for(auto point : foundPoints)
    {
        draw::drawPoint(*point, foundColor, 5);
    }

    drawOctree(octree);

    glFinish();
}