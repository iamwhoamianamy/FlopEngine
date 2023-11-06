#pragma once

#include <ranges>
#include <chrono>

#include "libs/geometry/rectangle.hpp"
#include "utils/concepts.hpp"

namespace utils
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

inline auto true_ellapsed(flp::duration auto ellapsed)
{
    auto casted{std::chrono::duration_cast<std::chrono::microseconds>(ellapsed)};
    return casted.count() / ellapsed_normalization_factor;
}

inline auto generate_random(const rectangle& range, size_t count)
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

} // namespace utils