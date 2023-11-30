#pragma once

#include <concepts>

#include "remove_all.hpp"

namespace flp::concepts
{

template<typename T1, typename T2>
concept base_same_as = std::same_as<
    traits::remove_all_t<T1>,
    traits::remove_all_t<T2>
>;

} // namespace flp::concepts
