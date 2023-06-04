#include <ranges>

namespace utils
{

auto iota(size_t n)
{
    return std::views::iota(size_t{}, n);
}

}