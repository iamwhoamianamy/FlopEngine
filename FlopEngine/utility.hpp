#pragma once
#include <vector>

namespace util
{
	template<class InitType, class ResType, class Transformer<ResType>>
	std::vector<T> transform(const std::vector<T*>& vec);
}

