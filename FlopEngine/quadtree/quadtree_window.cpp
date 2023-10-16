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

quadtree_window::quadtree_window(
    int argc, char** argv,
    float screen_width, float screen_height,
    std::string name) :
    base_window{argc, argv, screen_width, screen_height, name}
{

}

void quadtree_window::keyboard_letters(unsigned char key, int x, int y)
{

}

void quadtree_window::mouse(int button, int state, int x, int y)
{
    if(state == 0)
    {
        points.emplace_back(x, y);
    }
}

void quadtree_window::mouse_passive(int x, int y)
{
    _mouse_pos.x = x;
    _mouse_pos.y = y;
}

void quadtree_window::exiting_function()
{
    std::cout << "DONE!";
}

void test_f();

void quadtree_window::display()
{
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    auto point_color = draw::color(255, 255, 255);

    glEnable(GL_POINT_SMOOTH);

    for(auto& point : points)
    {
        draw::set_color(point_color);
        draw::draw_point(point, 5);
    }

    rectangle mouse_rectangle(_mouse_pos, vector2(40, 40));
    draw::set_color(point_color);
    draw::draw_rect(_mouse_pos, mouse_rectangle.half_dimensions.x, mouse_rectangle.half_dimensions.y);

    quadtree<vector2, 2> qtree{
        rectangle{
            vector2(_screen_w / 2, _screen_h / 2),
            vector2(_screen_w / 2, _screen_h / 2)
        }
    };

    qtree.insert(points);
    //auto found_points = qtree.quarry(mouse_rectangle);
    auto found_color = draw::color(255, 0, 0);

    for (auto point : qtree.quarry_as_range(mouse_rectangle))
    {
        draw::set_color(found_color);
        draw::draw_point(point, 5);
    }

    test_f();

    drawOctree(qtree);

    glFinish();
}

void test_f()
{
    //int arr[3];

    //auto range = utils::make_iterator_range(arr[0], arr[1]);

    //auto i1 = begin(range);

    auto it = quadtree<vector2, 2>::const_iterator();

    auto range = utils::make_iterator_range(it, it);

    auto b = begin(range);
}