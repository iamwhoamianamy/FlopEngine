#pragma once
#include <ranges>
#include <chrono>

#include "concepts.hpp"

namespace utils
{

constexpr const float ellapsed_normalization_factor{2.0e6f};

inline auto iota(size_t n)
{
    return std::views::iota(size_t{}, n);
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

}