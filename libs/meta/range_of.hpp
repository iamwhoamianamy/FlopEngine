#pragma once

#include <ranges>

#include "trait_convertible_to.hpp"

namespace flp::concepts
{

template<typename Range, typename T>
concept range_of = 
    std::ranges::range<Range> &&
    std::same_as<typename traits::remove_all_t<Range>::value_type, T>;

} // namespace flp::concepts