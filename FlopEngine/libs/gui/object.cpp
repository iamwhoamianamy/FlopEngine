#include "object.hpp"
#include "master.hpp"

#include "utils/singleton.hpp"

using namespace gui;

object_ptr object::create(const rectangle& boundary_rectangle)
{
    return object_ptr(new object{boundary_rectangle});
}

object::object(const rectangle& boundary_rectangle)
    : _boundary_rectangle{boundary_rectangle}
{
    init();
}

void gui::object::resize(const rectangle& boundary_rectangle)
{
    _boundary_rectangle = boundary_rectangle;
}

void gui::object::init()
{
    utils::singleton<master>::get().add_new_object(this);
}

void object::draw()
{

}

const rectangle& object::boundary_rectangle() const
{
    return _boundary_rectangle;
}

void gui::object::on_press(callback_t&& callback)
{
    _on_press = callback;
}

void gui::object::on_release(callback_t&& callback)
{
    _on_release = callback;
}

bool object::hidden() const
{
    return _hidden;
}

bool object::hovered_over() const
{
    return _hovered_over;
}

bool gui::object::pressed() const
{
    return _pressed;
}

bool gui::object::active() const
{
    return _active;
}

void gui::object::set_hovered_over(bool hovered_over)
{
    _hovered_over = hovered_over;
}

void gui::object::set_pressed(bool pressed)
{
    _pressed = pressed;

    if (pressed && _on_press)
        (*_on_press)();
}

void gui::object::set_active(bool active)
{
    _active = active;
}

void gui::object::release()
{
    _pressed = false;

    if (_on_release)
        (*_on_release)();
}