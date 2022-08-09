#include "quadtree_window.hpp"
#include "drawing.hpp"
#include "quadtree_help.hpp"

using namespace flp;

template<>
class QuadtreePointHolder<Vector2>
{
public:
    static float x(Vector2* vec)
    {
        return vec->x;
    }

    static float y(Vector2* vec)
    {
        return vec->y;
    }
};

QuadtreeWindow::QuadtreeWindow(
    int argc, char** argv,
    float screenWidth, float screenHeight,
    std::string name) :
    BaseWindow(argc, argv, screenWidth, screenHeight, name)
{

}

void QuadtreeWindow::keyboardLetters(unsigned char key, int x, int y)
{

}

void QuadtreeWindow::mouse(int button, int state, int x, int y)
{
    if(state == 0)
    {
        points.emplace_back(x, y);
    }
}

void QuadtreeWindow::mousePassive(int x, int y)
{
    mousePos.x = x;
    mousePos.y = y;
}

void QuadtreeWindow::exitingFunction()
{
    std::cout << "DONE!";
}

void QuadtreeWindow::display()
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

    Quadtree<Vector2> octree(Rect(
            Vector2(screenWidth / 2, screenHeight / 2),
            Vector2(screenWidth / 2, screenHeight / 2)));

    octree.insert(points);
    auto foundPoints = octree.quarry(mouseBox);
    auto foundColor = draw::Color(255, 0, 0);

    for(auto point : foundPoints)
    {
        draw::drawPoint(*point, foundColor, 5);
    }

    drawOctree(octree);

    glFinish();
}