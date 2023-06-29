#pragma once

#include "libs/geometry/vector2.hpp"
#include "libs/math/math.hpp"
#include "geometry_figure.hpp"
#include "libs/geometry/edge.hpp"

template <geo_figure_vertex V>
struct triangle_base;

using triangle = triangle_base<vector2>;
using triangle_view = triangle_base<vector2*>;

template <geo_figure_vertex V>
struct triangle_base : geometry_figure<V, 3>
{
private:
    using base_t = geometry_figure<V, 3>;

public:
    triangle_base(std::initializer_list<V> list);
    triangle_base(const V& v1, const V& v2, const V& v3);

    const vector2& a() const;
    const vector2& b() const;
    const vector2& c() const;

    float a_side() const;
    float b_side() const;
    float c_side() const;

    std::pair<vector2, float> get_circumcircle() const;
    float area() const;

    edge_base<V> get_edge(size_t i) const;

    bool is_sharp_enough(float threshold) const;
};

template<geo_figure_vertex V>
inline triangle_base<V>::triangle_base(std::initializer_list<V> list)
    : base_t{list}
{
}

template<geo_figure_vertex V>
inline triangle_base<V>::triangle_base(const V& a, const V& b, const V& c)
    : triangle_base{a, b, c}
{
}

template<geo_figure_vertex V>
inline const vector2& triangle_base<V>::a() const
{
    return base_t::get_t::get(base_t::_vertices[0]);
}

template<geo_figure_vertex V>
inline const vector2& triangle_base<V>::b() const
{
    return base_t::get_t::get(base_t::_vertices[1]);
}

template<geo_figure_vertex V>
inline const vector2& triangle_base<V>::c() const
{
    return base_t::get_t::get(base_t::_vertices[2]);
}

template<geo_figure_vertex V>
inline float triangle_base<V>::a_side() const
{
    return get_edge(0).perimeter();
}

template<geo_figure_vertex V>
inline float triangle_base<V>::b_side() const
{
    return get_edge(1).perimeter();
}

template<geo_figure_vertex V>
inline float triangle_base<V>::c_side() const
{
    return get_edge(2).perimeter();
}

// lazy ass
inline auto calc_determinant(const std::array<float, 9>& m)
{
    return
        (m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7]) - 
        (m[2] * m[4] * m[6] + m[0] * m[7] * m[5] + m[8] * m[1] * m[3]);
}

template<geo_figure_vertex V>
inline std::pair<vector2, float> triangle_base<V>::get_circumcircle() const
{
    vector2 center;

    auto a_norm{a().length_squared()};
    auto b_norm{b().length_squared()};
    auto c_norm{c().length_squared()};

    auto sx{calc_determinant({
        a_norm, a().y, 1,
        b_norm, b().y, 1, 
        c_norm, c().y, 1
    }) / 2};

    auto sy{calc_determinant({
        a().x, a_norm, 1,
        b().x, b_norm, 1,
        c().x, c_norm, 1
    }) / 2};

    auto a_det{calc_determinant({
        a().x, a().y, 1,
        b().x, b().y, 1,
        c().x, c().y, 1
    })};

    center.x = sx / a_det;
    center.y = sy / a_det;

    auto ar{area()};

    math::limit(ar, 1e-6f, ar);

    auto radius{(a_side() * b_side() * c_side()) / (4 * ar)};

    return {center, radius};
}

template<geo_figure_vertex V>
inline float triangle_base<V>::area() const
{
    auto p{base_t::perimeter() / 2};
    return std::sqrtf(p * (p - a_side()) * (p - b_side()) * (p - c_side()));
}

template<geo_figure_vertex V>
inline edge_base<V> triangle_base<V>::get_edge(size_t i) const
{
    return {base_t::_vertices[i % 3], base_t::_vertices[(i + 1) % 3]};
}

template<geo_figure_vertex V>
inline bool triangle_base<V>::is_sharp_enough(float threshold) const
{
    return false;
}

template<geo_figure_vertex V>
struct std::hash<triangle_base<V>>
{
    size_t operator()(const auto& triangle) const
    {
        return triangle.hash();
    }
};

inline bool operator==(const triangle& t1, const triangle& t2)
{
    return t1.equal(t2);
}