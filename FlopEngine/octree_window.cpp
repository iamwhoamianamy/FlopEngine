#include "octree_window.hpp"
#include "drawing.hpp"

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
        points.push_back(Vector3(x, y, 0));
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

    auto pointColor = drawing::Color(255, 255, 255);

    glEnable(GL_POINT_SMOOTH);

    for(auto& point : points)
    {
        drawing::drawPoint(point, pointColor, 5);
    }

    Box mouseBox(mousePos, Vector3(40, 40, 40));
    drawing::drawRectangle(mousePos, mouseBox.halfDimensions.x, mouseBox.halfDimensions.y, pointColor);

    Octree octree(Box(Vector3(screenWidth / 2, screenHeight / 2, 0),
        Vector3(screenWidth / 2, screenHeight / 2, 50)), 1);
    std::vector<Vector3*> foundPoints;

    octree.insert(points);
    octree.quarry(mouseBox, foundPoints);

    auto foundColor = drawing::Color(255, 0, 0);

    for(auto point : foundPoints)
    {
        drawing::drawPoint(*point, foundColor, 5);
    }

    drawing::drawOctree(octree);

    glFinish();
}