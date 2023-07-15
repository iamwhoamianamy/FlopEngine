#pragma once

#include <unordered_set>
#include <string>

#include "libs/geometry/vector2.hpp"
#include "libs/gui/defines.hpp"

namespace gui
{

struct master final
{
public:
    using objects_t = std::unordered_set<object_ptr>;

public:
    void hover(const vector2& mouse_position);
    void register_mouse_click_status_change(const vector2& mouse_position);
    void draw();

private:
    auto get_objects_under_cursor(const vector2& mouse_position) -> objects_t;
    void add_new_object(object_ptr object);

private:
    friend object;

    objects_t _objects;

    bool _last_mouse_status_is_press = false;
    objects_t _pressed_objects;
};

} // namespace gui