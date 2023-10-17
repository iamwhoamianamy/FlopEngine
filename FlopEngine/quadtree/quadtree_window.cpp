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

    auto found_color = draw::color(255, 0, 0);

    for (auto point : qtree.quarry_as_range(mouse_rectangle))
    {
        draw::set_color(found_color);
        draw::draw_point(point, 5);
    }

    //test_f();

    drawOctree(qtree);

    glFinish();
}

template<std::ranges::range R>
void f(R r)
{
    
}

template<std::indirectly_readable I>
void f_indirectly_readable(I i)
{

}

template<std::weakly_incrementable I>
void f_weakly_incrementable(I i)
{

}

template<std::incrementable I>
void f_incrementable(I i)
{

}

template<std::input_iterator I>
void f_input_iterator(I i)
{

}

template<std::forward_iterator I>
void f_forward_iterator(I i)
{

}

void range_my()
{
    auto it = quadtree<vector2, 2>::const_iterator();

    auto range = utils::make_iterator_range(it, it);

    auto i1 = begin(range);
    auto i2 = end(range);

    auto val = *begin(range);

    f_indirectly_readable(i1);
    f_weakly_incrementable(i1);
    f_incrementable(i1);
    f_input_iterator(i1);

    f(range);

    auto b = begin(range);
}

void test_f()
{
    range_my();
}