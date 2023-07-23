#pragma once

#include <unordered_set>
#include <string>

#include "libs/geometry/vector2.hpp"
#include "libs/geometry/rectangle.hpp"
#include "libs/gui/defines.hpp"
#include "libs/gui/layout.hpp"

namespace gui
{

struct master final
{
public:
    using objects_t = std::unordered_set<object*>;

public:
    master() = default;

public:
    void resize(const rectangle& screen_rectangle);
    void hover(const vector2& mouse_position);
    void register_mouse_click_status_change(const vector2& mouse_position);
    void draw();

public:
    auto screen_layout() -> std::shared_ptr<layout>;

private:
    auto get_objects_under_cursor(const vector2& mouse_position) -> objects_t;
    void add_new_object(object* object);

private:
    friend object;

    objects_t _objects;

    bool _last_mouse_status_is_press = false;
    objects_t _pressed_objects;

    std::shared_ptr<layout> _screen_layout;
};

} // namespace gui