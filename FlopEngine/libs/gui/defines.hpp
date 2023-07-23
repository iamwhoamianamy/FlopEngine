#pragma once

#include <memory>
#include <functional>

namespace gui
{

struct object;
struct layout;
struct split_layout;

using object_ptr = std::shared_ptr<object>;
using callback_t = std::function<void()>;

} // namespace gui