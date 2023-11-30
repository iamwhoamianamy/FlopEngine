#pragma once

#include "converter.hpp"

namespace flp::concepts
{

template <typename From, typename To>
concept trait_convertible_to = requires(From&& v)
{
    { traits::converter<From, To>::convert(std::forward<From>(v)) } -> base_same_as<To>;
};

} // namespace flp::concepts