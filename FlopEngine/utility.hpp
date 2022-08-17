#pragma once
#include <vector>

namespace util
{
    template<class InitType, class ResType, class Transformer>
    std::vector<ResType> transform(const std::vector<InitType>& vec)
    {
        std::vector<ResType> result;
        result.reserve(vec.size());

        for(auto& elem : vec)
        {
            result.emplace_back(Transformer()(elem));
        }

        return result;
    }
}