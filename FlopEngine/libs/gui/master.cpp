#include <algorithm>

#include "master.hpp"
#include "object.hpp"

void gui::master::hover(const vector2& mouse_position)
{
    auto candidates = get_all_under_cursor(mouse_position);

    for (auto object : _objects)
    {
        bool hovered_over = candidates.contains(object);
        object->set_hovered_over(hovered_over);
    }
}

void gui::master::register_mouse_click_status_change(const vector2& mouse_position)
{
    auto candidates = get_all_under_cursor(mouse_position);

    if (_pressed_objects.empty())
    {
        for (auto candidate : candidates)
        {
            candidate->set_pressed_status(true);
            _pressed_objects.insert(candidate);
        }
    }
    else
    {
        for (auto candidate : candidates)
        {
            if (_pressed_objects.contains(candidate))
            {
                candidate->set_pressed_status(false);
            }
        }

        _pressed_objects.clear();
    }
}

void gui::master::draw()
{
    for (auto object : _objects)
    {
        object->draw();
    }
}

auto gui::master::get_all_under_cursor(const vector2& mouse_position) -> objects_t
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
