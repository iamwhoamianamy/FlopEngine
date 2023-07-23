#include "layout.hpp"

#include "libs/graphics/drawing.hpp"

using namespace gui;

std::shared_ptr<layout> gui::layout::create(const rectangle& boundary_rectangle)
{
    return std::shared_ptr<layout>(new layout{boundary_rectangle});
}

std::shared_ptr<layout> layout::create(layout* parent)
{
    return std::shared_ptr<layout>(new layout{parent});
}

void layout::draw()
{
    draw::set_line_width(object::border_width);
    draw::set_color(draw::color::blue());
    draw::draw_rect(object::boundary_rectangle());

    for (auto& child : _children)
    {
        child->draw();
    }
}

void layout::add_child(object* child)
{
    _children.insert(child);
}

void layout::resize(const rectangle& screen_rectangle)
{
    object::resize(fix_boundary_rectangle(screen_rectangle));

    for (auto child : _children)
        child->resize(boundary_rectangle());
}

layout::layout(const rectangle& boundary_rectangle)
    : object{fix_boundary_rectangle(boundary_rectangle)}
{

}

gui::layout::layout(layout* parent)
    : object{}
    , _parent{parent}
{

}

void gui::layout::resize(object* object, rectangle boundary_rectangle)
{
    object->resize(boundary_rectangle);
}

auto gui::layout::fix_boundary_rectangle(const rectangle& boundary_rectangle) -> rectangle
{
    auto result = boundary_rectangle;

    result.half_dimensions.x -= object::border_width + 1.0f;
    result.half_dimensions.y -= object::border_width + 1.0f;

    return result;
}
