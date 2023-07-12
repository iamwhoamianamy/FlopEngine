#include "button.hpp"
#include "libs/graphics/drawing.hpp"

std::shared_ptr<gui::button>
gui::button::create(const rectangle& boundary_rectangle)
{
    return std::shared_ptr<button>(new button{boundary_rectangle});
}

gui::button::~button()
{

}

void gui::button::draw()
{
    if (object::hovered_over())
        draw::set_color(draw::color::red());
    else
        draw::set_color(draw::color::white());
    
    draw::set_line_width(4);
    draw::draw_rect(object::boundary_rectangle());
}

gui::button::button(const rectangle& boundary_rectangle)
    : object{boundary_rectangle}
{
}
