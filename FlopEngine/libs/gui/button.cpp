#include "button.hpp"
#include "libs/graphics/drawing.hpp"

std::shared_ptr<gui::button>
gui::button::create(const frame_t& frame)
{
    return std::shared_ptr<button>{new button{frame}};
}

void gui::button::draw()
{
    if (active())
        draw::set_line_stripple(1);

    if (object::pressed())
        draw::set_line_width(4);
    else
        draw::set_line_width(2);

    if (object::hovered_over())
        draw::set_color(draw::color::red());
    else
        draw::set_color(draw::color::white());
    
    draw::draw_rect(object::frame());

    draw::disable_line_stripple();
}

gui::button::button(const frame_t& frame)
    : object{frame}
{
}
