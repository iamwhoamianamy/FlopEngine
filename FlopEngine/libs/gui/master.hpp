#pragma once

#include <unordered_set>
#include <memory>
#include <string>

#include "libs/geometry/vector2.hpp"

namespace gui
{

struct object;

struct master
{
public:
    using objects_t = std::unordered_set<std::shared_ptr<object>>;

public:
    void hover(const vector2& mouse_position);
    void register_mouse_click_status_change(const vector2& mouse_position);
    void draw();

private:
    auto get_all_under_cursor(const vector2& mouse_position) -> objects_t;
    void add_new_object(std::shared_ptr<object> object);

private:
    friend object;

    objects_t _objects;
    objects_t _pressed_objects;
};

} // namespace gui