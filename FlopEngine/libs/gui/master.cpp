#include <algorithm>

#include "master.hpp"
#include "object.hpp"

void gui::master::hover(const vector2& mouse_position)
{
    auto candidates = get_objects_under_cursor(mouse_position);

    for (auto object : _objects)
    {
        bool hovered_over = candidates.contains(object);
        object->set_hovered_over(hovered_over);
    }
}

void gui::master::register_mouse_click_status_change(const vector2& mouse_position)
{
    auto objects_under_cursor = get_objects_under_cursor(mouse_position);

    if (_last_mouse_status_is_press)
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
    }
    else
    {
        _last_mouse_status_is_press = true;

        for (auto object_under_cursor : objects_under_cursor)
        {
            object_under_cursor->set_pressed(true);
            _pressed_objects.insert(object_under_cursor);
        }
    }
}

void gui::master::draw()
{
    for (auto object : _objects)
    {
        object->draw();
    }
}

auto gui::master::get_objects_under_cursor(const vector2& mouse_position) -> objects_t
{
    objects_t result;

    std::copy_if(
        _objects.begin(), _objects.end(),
        std::inserter(result, result.end()),
        [&](std::shared_ptr<object> object)
        {
            return object->boundary_rectangle().contains(mouse_position);
        }
    );

    return result;
}

void gui::master::add_new_object(std::shared_ptr<gui::object> object)
{
    _objects.insert(object);
}
