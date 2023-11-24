#pragma once

#include "libs/geometry/vector2.hpp"

namespace flp
{

namespace concepts
{

template<typename T>
concept physics_object = requires(T t)
{
    { t.position     } -> std::convertible_to<vector2>;
    { t.velocity     } -> std::convertible_to<vector2>;
    { t.acceleration } -> std::convertible_to<vector2>;
};

} // namespace concepts

namespace traits
{

template<flp::concepts::physics_object Obj>
struct physics_object_traits
{
    static const vector2 position    (const Obj& obj) { return obj.position;     }
    static const vector2 velocity    (const Obj& obj) { return obj.velocity;     }
    static const vector2 acceleration(const Obj& obj) { return obj.acceleration; }
};

} // namespace traits

} // namespace flp