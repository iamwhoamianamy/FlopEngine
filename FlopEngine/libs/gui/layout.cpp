#include "layout.hpp"

#include "libs/graphics/drawing.hpp"

using namespace gui;

std::shared_ptr<layout> gui::layout::create(const frame_t& frame)
{
    return std::shared_ptr<layout>{new layout{frame}};
}

void layout::draw()
{
    draw::set_line_width(object::border_width);
    draw::set_color(draw::color::blue());
    draw::draw_rect(object::frame());

    for (auto& child : _children)
    {
        child->draw();
    }
}

void layout::add_child(object* child)
{
    _children.insert(child);
}

void layout::resize(const frame_t& new_frame)
{
    object::resize(fix_frame(new_frame));

    for (auto child : _children)
        child->resize(frame());
}

layout::layout(const frame_t& frame)
    : object{fix_frame(frame)}
{

}

auto gui::layout::fix_frame(const frame_t& frame) -> frame_t
{
    auto result = frame;

    result.half_dimensions.x -= object::border_width + 1.0f;
    result.half_dimensions.y -= object::border_width + 1.0f;

    return result;
}
