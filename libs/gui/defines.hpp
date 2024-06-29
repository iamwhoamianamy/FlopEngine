#pragma once

#include <memory>
#include <functional>

#include "libs/geometry/rectangle.hpp"

namespace flp::gui
{

struct object;
struct layout;
struct split_layout;

using object_ptr = std::shared_ptr<object>;
using callback_t = std::function<void()>;

using keyboard_key_t = char;
using keyboard_callback_t = std::function<void(keyboard_key_t)>;

using frame_t = geo::rectangle;

} // namespace gui