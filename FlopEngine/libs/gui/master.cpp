#include <algorithm>

#include "master.hpp"
#include "object.hpp"

void gui::master::set_layout(layout* layout)
{
    _screen_layout = layout;
}

void gui::master::resize(const rectangle& screen_rectangle)
{
    if (_screen_layout)
        _screen_layout->resize(screen_rectangle);
}

void gui::master::hover(const vector2& mouse_position)
{
    if (_hidden)
        return;

    auto candidates = get_objects_under_cursor(mouse_position);

    for (auto object : _objects)
    {
        bool hovered_over = candidates.contains(object);
        object->set_hovered_over(hovered_over);
    }
}

void gui::master::register_mouse_click_status_change(const vector2& mouse_position)
{
    if (_hidden)
        return;

    auto objects_under_cursor = get_objects_under_cursor(mouse_position);

    auto handle_press = [&]
    {
        _last_mouse_status_is_press = true;

        for (auto object_under_cursor : objects_under_cursor)
        {
            object_under_cursor->set_pressed(true);
            _pressed_objects.insert(object_under_cursor);
        }

        for (auto active_object : _active_objects)
            active_object->set_active(false);

        _active_objects.clear();
        _active_objects.insert(objects_under_cursor.begin(), objects_under_cursor.end());

        for (auto active_object : _active_objects)
            active_object->set_active(true);
    };

    auto handle_release = [&]
    {
        _last_mouse_status_is_press = false;

        for (auto object_under_cursor : objects_under_cursor)
        {
            if (_pressed_objects.contains(object_under_cursor))
            {
                object_under_cursor->release();
            }
        }

        for (auto pressed_object : _pressed_objects)
        {
            pressed_object->set_pressed(false);
        }

        _pressed_objects.clear();
    };


    if (_last_mouse_status_is_press)
        handle_release();
    else
        handle_press();
}

void gui::master::draw()
{
    if (_hidden)
        return;

    for (auto object : _objects)
    {
        object->draw();
    }
}

void gui::master::react_on_keyboard_key_press(keyboard_key_t key)
{
    if (_hidden)
        return;

    for (auto object : _active_objects)
    {
        object->press_key(key);
    }
}

void gui::master::set_hidden(bool hidden)
{
    _hidden = hidden;
}

auto gui::master::screen_layout() -> layout*
{
    if (_screen_layout)
        return _screen_layout;
}

auto gui::master::get_objects_under_cursor(const vector2& mouse_position) -> objects_t
{
    objects_t result;

    std::copy_if(
        _objects.begin(), _objects.end(),
        std::inserter(result, result.end()),
        [&](object* object)
        {
            return object->boundary_rectangle().contains(mouse_position);
        }
    );

    return result;
}

void gui::master::add_new_object(object* object)
{
    _objects.insert(object);
}
