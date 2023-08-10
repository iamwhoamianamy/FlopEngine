#pragma once

namespace utils
{

template <typename Begin, typename End>
struct iterator_range
{
    iterator_range(Begin b, End e) : _begin{b}, _end{e} { }

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

auto make_iterator_range(auto begin, auto end)
{
    return iterator_range{begin, end};
}

} // namespace utils
