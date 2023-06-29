#pragma once

#include <array>

#include "libs/geometry/vector2.hpp"
#include "libs/math/math.hpp"
#include "geometry_figure.hpp"

template <geo_figure_vertex V>
class edge_base;

using edge = edge_base<vector2>;
using edge_view = edge_base<vector2*>;

template <geo_figure_vertex V>
struct edge_base : geometry_figure<V, 2>
{
private:
    using base_t = geometry_figure<V, 2>;

public:
    edge_base(std::initializer_list<V> list);
    edge_base(const V& v1, const V& v2);

    const vector2& a() const;
    const vector2& b() const;
};

template<geo_figure_vertex V>
inline edge_base<V>::edge_base(std::initializer_list<V> list)
    : base_t{list}
{
}

template<geo_figure_vertex V>
inline edge_base<V>::edge_base(const V& a, const V& b)
    : edge_base{a, b}
{
}

template<geo_figure_vertex V>
inline const vector2& edge_base<V>::a() const
{
    return base_t::get_t::get(base_t::_vertices[0]);
}

template<geo_figure_vertex V>
inline const vector2& edge_base<V>::b() const
{
    return base_t::get_t::get(base_t::_vertices[1]);
}

template<geo_figure_vertex V>
struct std::hash<edge_base<V>>
{
    size_t operator()(const auto& edge) const
    {
        return edge.hash();
    }
};

inline bool operator==(const edge& e1, const edge& e2)
{
    return e1.equal(e2);
}