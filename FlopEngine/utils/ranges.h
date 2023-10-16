#pragma once

#include <iterator>

namespace utils
{

template <typename Begin, typename End = Begin>
struct iterator_range
{
    iterator_range(Begin b, End e);
    iterator_range(const iterator_range& other);
    iterator_range(iterator_range&& other);

    iterator_range& operator=(const iterator_range& other);
    iterator_range& operator=(iterator_range&& other);

    Begin begin() noexcept;
    End end() noexcept;

    Begin begin() const noexcept;
    End end() const noexcept;

private:
    void copy_fields(const iterator_range& other);
    void move_fields(iterator_range&& other);

private:
    Begin _begin;
    End _end;
};

template<typename Begin, typename End>
inline iterator_range<Begin, End>::iterator_range(Begin b, End e)
    : _begin{b}
    , _end{e}
{

}

template<typename Begin, typename End>
inline iterator_range<Begin, End>::iterator_range(const iterator_range& other)
{
    copy_fields(other);
}

template<typename Begin, typename End>
inline iterator_range<Begin, End>::iterator_range(iterator_range&& other)
{
    move_fields(iterator_range);
}

template<typename Begin, typename End>
inline iterator_range<Begin, End>& iterator_range<Begin, End>::operator=(const iterator_range& other)
{
    copy_fields(other);

    return *this;
}

template<typename Begin, typename End>
inline iterator_range<Begin, End>& iterator_range<Begin, End>::operator=(iterator_range&& other)
{
    move_fields(other);

    return *this;
}

template<typename Begin, typename End>
inline Begin iterator_range<Begin, End>::begin() noexcept
{
    return _begin;
}
template<typename Begin, typename End>
inline End iterator_range<Begin, End>::end() noexcept
{
    return _end;
}
template<typename Begin, typename End>
inline Begin iterator_range<Begin, End>::begin() const noexcept
{
    return _begin;
}
template<typename Begin, typename End>
inline End iterator_range<Begin, End>::end() const noexcept
{
    return _end;
}
template<typename Begin, typename End>
inline void iterator_range<Begin, End>::copy_fields(const iterator_range& other)
{
    _begin = other._begin;
    _end = other._end;
}
template<typename Begin, typename End>
inline void iterator_range<Begin, End>::move_fields(iterator_range&& other)
{
    _begin = std::move(other._begin);
    _end = std::move(other._end);
}
;

inline auto make_iterator_range(auto begin, auto end)
{
    return iterator_range{begin, end};
}

template <typename Begin, typename End>
inline auto begin(iterator_range<Begin, End>& range) noexcept
{
    return range.begin();
}

template <typename Begin, typename End>
inline auto end(iterator_range<Begin, End>& range) noexcept
{
    return range.end();
}

template <typename Begin, typename End>
inline auto begin(const iterator_range<Begin, End>& range) noexcept
{
    return range.begin();
}

template <typename Begin, typename End>
inline auto end(const iterator_range<Begin, End>& range) noexcept
{
    return range.end();
}

} // namespace utils
