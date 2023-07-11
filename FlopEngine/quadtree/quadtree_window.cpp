#include "quadtree_window.hpp"
#include "libs/graphics/drawing.hpp"
#include "libs/quadtree/quadtree_help.hpp"

using namespace flp;

namespace traits
{

template<>
class access<vector2>
{
public:
    static vector2 position(vector2* vec)
    {
        return *vec;
    }
};

}

QuadtreeWindow::QuadtreeWindow(
    int argc, char** argv,
    float _screen_width, float _screen_height,
    std::string name) :
    base_window(argc, argv, _screen_width, _screen_height, name)
{

}

void QuadtreeWindow::keyboard_letters(unsigned char key, int x, int y)
{

}

void QuadtreeWindow::mouse(int button, int state, int x, int y)
{
    if(state == 0)
    {
        points.emplace_back(x, y);
    }
}

void QuadtreeWindow::mouse_passive(int x, int y)
{
    _mouse_pos.x = x;
    _mouse_pos.y = y;
}

void QuadtreeWindow::exiting_function()
{
    std::cout << "DONE!";
}

void QuadtreeWindow::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    auto pointColor = draw::color(255, 255, 255);

    glEnable(GL_POINT_SMOOTH);

    for(auto& point : points)
    {
        draw::set_color(pointColor);
        draw::draw_point(point, 5);
    }

    rectangle mouseBox(_mouse_pos, vector2(40, 40));
    draw::set_color(pointColor);
    draw::draw_rect(_mouse_pos, mouseBox.half_dimensions.x, mouseBox.half_dimensions.y);

    quadtree<vector2, 8> octree(rectangle(
            vector2(_screen_w / 2, _screen_h / 2),
            vector2(_screen_w / 2, _screen_h / 2)));

    octree.insert(points);
    auto foundPoints = octree.quarry(mouseBox);
    auto foundColor = draw::color(255, 0, 0);

    for(auto point : foundPoints)
    {
        draw::set_color(foundColor);
        draw::draw_point(*point, 5);
    }

    drawOctree(octree);

    glFinish();
}