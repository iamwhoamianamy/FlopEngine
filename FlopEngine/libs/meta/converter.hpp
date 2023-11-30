#pragma once

#include "base_same_as.hpp"

namespace flp::traits
{

template<typename From, typename To>
struct converter
{
    static concepts::base_same_as<To> auto&& convert(concepts::base_same_as<From> auto&&);
};

template<typename T>
struct converter<T, T>
{
    static concepts::base_same_as<T> auto&& convert(concepts::base_same_as<T> auto&& val)
    {
        return val;
    }
};

template<typename T>
struct converter<T, T*>
{
    static concepts::base_same_as<T*> auto&& convert(concepts::base_same_as<T> auto&& val)
    {
        return &val;
    }
};

template<typename T>
struct converter<T*, T>
{
    static concepts::base_same_as<T> auto&& convert(concepts::base_same_as<T*> auto&& val)
    {
        return *val;
    }
};

} // namespace flp::traits