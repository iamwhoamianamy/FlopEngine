#pragma once

#include <ranges>
#include <chrono>

#include "libs/geometry/rectangle.hpp"
#include "libs/meta/concepts.hpp"

namespace flp::utils
{

constexpr const float ellapsed_normalization_factor{2.0e6f};

inline auto iota(size_t start, size_t end)
{
    return std::views::iota(start, end);
}

inline auto iota(size_t n)
{
    return iota(size_t{}, n);
}

template<class InitType, class ResType, class Transformer>
auto transform(const std::vector<InitType>& vec) -> std::vector<ResType>
{
    std::vector<ResType> result;
    result.reserve(vec.size());

    for (auto& elem : vec)
    {
        result.emplace_back(Transformer()(elem));
    }

    return result;
}

inline auto true_ellapsed(concepts::duration auto ellapsed)
{
    auto casted{std::chrono::duration_cast<std::chrono::microseconds>(ellapsed)};
    return casted.count() / ellapsed_normalization_factor;
}

inline auto generate_random(const geo::rectangle& range, size_t count)
{
    std::vector<vector2> result(count);

    std::ranges::generate_n(result.begin(), count,
        [&range]
        {
            return vector2{
                math::random_in_range(0, range.width()),
                math::random_in_range(0, range.height())
            };
        });

    return result;
}

template <typename T>
concept EnumWithSize = requires(T t)
{
    T::SIZE;
};

template <EnumWithSize E>
inline E next_enum(const E& e)
{
    using enum_t = std::remove_cvref_t<decltype(e)>;
    using underlying_t = std::underlying_type_t<enum_t>;

    return static_cast<enum_t>(
        (static_cast<underlying_t>(e) + 1) %
        static_cast<underlying_t>(enum_t::SIZE));
}

} // namespace flp::utils