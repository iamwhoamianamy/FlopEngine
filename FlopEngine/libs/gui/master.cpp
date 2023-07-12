#include <algorithm>

#include "master.hpp"
#include "object.hpp"

void gui::master::hover(const vector2& mouse_position)
{
    objects_t candidates;

    std::copy_if(
        _objects.begin(), _objects.end(),
        std::inserter(candidates, candidates.end()),
        [&](std::shared_ptr<object> object)
        {
            return object->boundary_rectangle().contains(mouse_position);
        }
    );

    for (auto object : _objects)
    {
        bool hovered_over = candidates.contains(object);
        object->set_hovered_over(hovered_over);
    }
}

void gui::master::draw()
{
    for (auto object : _objects)
    {
        object->draw();
    }
}

void gui::master::add_new_object(std::shared_ptr<gui::object> object)
{
    _objects.insert(object);
}
