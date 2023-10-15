#pragma once

#include <iterator>

namespace utils
{

template <typename Begin, typename End = Begin>
struct iterator_range
{
    iterator_range(Begin b, End e)
        : _begin{b}
        , _end{e}
    {

    }

    iterator_range(const iterator_range& other)
    {
        copy_fields(other);
    }

    iterator_range(iterator_range&& other)
    {
        move_fields(iterator_range);
    }

    iterator_range& operator=(const iterator_range& other)
    {
        copy_fields(other);

        return *this;
    }

    iterator_range& operator=(iterator_range&& other)
    {
        move_fields(other);

        return *this;
    }

    Begin begin()
    {
        return _begin;
    };

    End end()
    {
        return _end;
    }

    Begin begin() const
    {
        return _begin;
    };

    End end() const
    {
        return _end;
    }

private:
    void copy_fields(const iterator_range& other)
    {
        _begin = other._begin;
        _end = other._end;
    }

    void move_fields(iterator_range&& other)
    {
        _begin = std::move(other._begin);
        _end = std::move(other._end);
    }

private:
    Begin _begin;
    End _end;
};

auto make_iterator_range(auto begin, auto end)
{
    return iterator_range{begin, end};
}

template <typename Begin, typename End>
auto begin(iterator_range<Begin, End>& range)
{
    return range.begin();
}

template <typename Begin, typename End>
auto end(iterator_range<Begin, End>& range)
{
    return range.end();
}

template <typename Begin, typename End>
auto begin(const iterator_range<Begin, End>& range)
{
    return range.begin();
}

template <typename Begin, typename End>
auto end(const iterator_range<Begin, End>& range)
{
    return range.end();
}

} // namespace utils
