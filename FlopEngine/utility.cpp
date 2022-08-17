#include "utility.hpp"

namespace util
{
	template<class T>
	std::vector<T> transform(const std::vector<T*>& vec)
	{
		std::vector<T> result;
		result.reserve(vec.size());

		for (auto elem : vec)
		{
			result.emplace_back(*elem);
		}

		return elem;
	}
}
