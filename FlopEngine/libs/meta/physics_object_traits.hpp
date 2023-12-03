#pragma once

#include "libs/geometry/vector2.hpp"

namespace flp::traits
{

template<typename T>
struct physics_object
{
    constexpr static vector2& position(T& obj);
    constexpr static vector2& velocity(T& obj);
    constexpr static vector2& acceleration(T& obj);
};

} // namespace flp::traits

namespace flp::concepts
{

template<typename T>
concept physics_object = requires(T&& t)
{
    { traits::physics_object<T>::position(t)     } -> std::convertible_to<vector2>;
    { traits::physics_object<T>::velocity(t)     } -> std::convertible_to<vector2>;
    { traits::physics_object<T>::acceleration(t) } -> std::convertible_to<vector2>;
};

} // namespace flp::concepts
