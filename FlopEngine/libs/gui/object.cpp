#include "object.hpp"
#include "master.hpp"

#include "utils/singleton.hpp"

using namespace flp::gui;

object_ptr object::create(const frame_t& frame)
{
    return object_ptr(new object{frame});
}

object::object(const frame_t& frame)
    : _frame{frame}
{
    init();
}

void object::resize(const frame_t& frame)
{
    _frame = frame;
}

void object::init()
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

void object::on_press(callback_t&& callback)
{
    _on_press = callback;
}

void object::on_release(callback_t&& callback)
{
    _on_release = callback;
}

void object::on_key_pressed(keyboard_callback_t&& callback)
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

bool object::pressed() const
{
    return _pressed;
}

bool object::active() const
{
    return _active;
}

void object::set_hovered_over(bool hovered_over)
{
    _hovered_over = hovered_over;
}

void object::set_pressed(bool pressed)
{
    _pressed = pressed;

    if (pressed && _on_press)
        (*_on_press)();
}

void object::set_active(bool active)
{
    _active = active;
}

void object::press_key(char key)
{
    if (_on_key_pressed)
        (*_on_key_pressed)(key);
}

void object::release()
{
    _pressed = false;

    if (_on_release)
        (*_on_release)();
}