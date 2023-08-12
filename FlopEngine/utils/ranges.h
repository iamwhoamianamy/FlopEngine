#pragma once

#include <iterator>

namespace utils
{

namespace detail
{

template <std::forward_iterator Begin, std::forward_iterator End>
struct iterator_range
{
    iterator_range(Begin b, End e)
        : _begin{b}
        , _end{e}
    {

    }

    auto begin()
    {
        return _begin;
    };

    auto end()
    {
        return _end;
    }

private:
    Begin _begin;
    End _end;
};

} // namespace detail

auto make_iterator_range(
    std::forward_iterator auto begin,
    std::forward_iterator auto end)
{
    return detail::iterator_range{begin, end};
}

} // namespace utils
