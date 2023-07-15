#pragma once

#include <numeric>
#include <array>
#include "libs/geometry/vector2.hpp"

template<typename V>
concept geo_figure_vertex = std::is_same_v<V, vector2> || std::is_same_v<V, vector2*>;

template<geo_figure_vertex V, size_t VertexCount>
struct geometry_figure
{
protected:
    using container_t = std::array<V, VertexCount>;
    using get_t = vector2_traits<V>;

    container_t _vertices;

public:
    geometry_figure(std::initializer_list<V> list);

    const V& operator[](size_t i) const;
    bool has_vertex(const vector2& v) const;
    size_t hash() const;
    float perimeter() const;

    template<geo_figure_vertex OtherV, size_t OtherVertexCount>
    bool has_similar_vertex(const geometry_figure<OtherV, OtherVertexCount>& other) const;

    template<geo_figure_vertex OtherV, size_t OtherVertexCount>
    bool equal(const geometry_figure<OtherV, OtherVertexCount>& other) const;

protected:
    const vector2& get_ref(size_t i) const;
};

template<geo_figure_vertex V, size_t VertexCount>
inline geometry_figure<V, VertexCount>::geometry_figure(std::initializer_list<V> list)
{
    std::copy(list.begin(), list.end(), _vertices.begin());
}

template<geo_figure_vertex V, size_t VertexCount>
inline const V& geometry_figure<V, VertexCount>::operator[](size_t i) const
{
    return _vertices[i % VertexCount];
}

template<geo_figure_vertex V, size_t VertexCount>
inline size_t geometry_figure<V, VertexCount>::hash() const
{
    size_t result{0};

    result = std::reduce(_vertices.begin(), _vertices.end(), result,
        [](const auto& sum, const auto& vertex)
        {
            using type_t = std::remove_cvref_t<decltype(vertex)>;
            return sum ^ std::hash<type_t>{}(vertex);
        });

    return result;
}

template<geo_figure_vertex V, size_t VertexCount>
inline float geometry_figure<V, VertexCount>::perimeter() const
{
    if constexpr (VertexCount == 2)
    {
        return (_vertices[0] - _vertices[1]).length();
    }
    else
    {
        float result{0};

        // TODO rewrite with iteration over edges somehow???
        for (size_t i{0}; i < VertexCount; i++)
        {
            result += vector2::distance(
                get_t::get(_vertices[i]),
                get_t::get(_vertices[(i + 1) % VertexCount]));
        }

        return result;
    }
}

template<geo_figure_vertex V, size_t VertexCount>
template<geo_figure_vertex OtherV, size_t OtherVertexCount>
inline bool geometry_figure<V, VertexCount>::equal(
    const geometry_figure<OtherV, OtherVertexCount>& other) const
{
    return 
        VertexCount == OtherVertexCount &&
        std::all_of(_vertices.begin(), _vertices.end(),
            [&other](const auto& vertex)
            {
                return other.has_vertex(get_t::get(vertex));
            });
}

template<geo_figure_vertex V, size_t VertexCount>
inline bool geometry_figure<V, VertexCount>::has_vertex(const vector2& v) const
{
    return std::any_of(_vertices.begin(), _vertices.end(),
        [&v](const auto& vertex)
        {
            return v == get_t::get(vertex);
        });
}

template<geo_figure_vertex V, size_t VertexCount>
template<geo_figure_vertex OtherV, size_t OtherVertexCount>
inline bool geometry_figure<V, VertexCount>::has_similar_vertex(
    const geometry_figure<OtherV, OtherVertexCount>& other) const
{
    return std::any_of(_vertices.begin(), _vertices.end(),
        [&other](const auto& vertex)
        {
            return other.has_vertex(get_t::get(vertex));
        });
}

template<geo_figure_vertex V, size_t VertexCount>
inline const vector2& geometry_figure<V, VertexCount>::get_ref(size_t i) const
{
    return get_t::get(_vertices[i % VertexCount]);
}
