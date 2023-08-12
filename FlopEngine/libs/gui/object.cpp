#include "object.hpp"
#include "master.hpp"

#include "utils/singleton.hpp"

using namespace gui;

object_ptr object::create(const frame_t& frame)
{
    return object_ptr(new object{frame});
}

object::object(const frame_t& frame)
    : _frame{frame}
{
    init();
}

void gui::object::resize(const frame_t& frame)
{
    _frame = frame;
}

void gui::object::init()
{
    utils::singleton<master>::get().add_new_object(this);
}

void object::draw()
{

}

const frame_t& object::frame() const
{
    return _frame;
}

void gui::object::on_press(callback_t&& callback)
{
    _on_press = callback;
}

void gui::object::on_release(callback_t&& callback)
{
    _on_release = callback;
}

void gui::object::on_key_pressed(keyboard_callback_t&& callback)
{
    _on_key_pressed = callback;
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

void gui::object::press_key(char key)
{
    if (_on_key_pressed)
        (*_on_key_pressed)(key);
}

void gui::object::release()
{
    _pressed = false;

    if (_on_release)
        (*_on_release)();
}