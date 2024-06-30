#include "label.hpp"

#include "libs/graphics/drawing.hpp"

using namespace flp::gui;

std::shared_ptr<label>
label::create(const std::string& text, const frame_t& frame)
{
    return std::shared_ptr<label>{new label{text, frame}};
}

void label::draw()
{
    if (active())
        draw::set_line_stripple(1);

    draw::set_line_width(2);

    if (object::hovered_over())
        draw::set_color(draw::color::red());
    else
        draw::set_color(draw::color::white());

    draw::draw_rect(object::frame());

    draw::disable_line_stripple();

    draw::render_string(frame().center, 20, _text);
}

label::label(const std::string& text, const frame_t& frame)
    : object{frame}
    , _text{text}
{

}
