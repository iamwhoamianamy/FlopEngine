#pragma once
#include <utility>

namespace flp
{

template<typename T1, typename T2>
concept one_side_addable = requires(T1 t1, T2 t2)
{
    { t1 + t2 } -> std::convertible_to<T1>;
};

template<typename T1, typename T2>
concept addable = one_side_addable<T1, T2> || one_side_addable<T2, T1>;

template<typename T>
concept duration = requires(T t)
{
    { t.count() } -> std::integral;
};

}