#pragma once
#include <array>

#include "vector2.hpp"
#include "math.hpp"

template <typename V>
concept triangle_vertex = std::is_same_v<V, vector2> || std::is_same_v<V, vector2*>;

template <triangle_vertex V>
class triangle_base;

using triangle = triangle_base<vector2>;
using triangle_view = triangle_base<vector2*>;

template <triangle_vertex V>
class triangle_base
{
private:
    using container_t = std::array<V, 3>;
    using get_t = get_vector2<V>;

    container_t _vertices;

public:
    triangle_base(const V& v1, const V& v2, const V& v3);
    //triangle_base(std::initializer_list<V> list);

    const vector2& a() const;
    const vector2& b() const;
    const vector2& c() const;

    const V& operator[](size_t i) const;

    float a_side() const;
    float b_side() const;
    float c_side() const;

    std::pair<vector2, float> get_circumcircle() const;
    float perimeter() const;
    float area() const;

    std::pair<V, V> edge(size_t i) const;

    bool is_sharp_enough(float threshold) const;
    bool has_vertex(const vector2& v) const;

    bool has_similar_vertex(const triangle_base<V>& other) const;
};

template<triangle_vertex V>
inline triangle_base<V>::triangle_base(const V& a, const V& b, const V& c)
{
    _vertices = {a, b, c};
}

//template<triangle_vertex V>
//inline triangle_base<V>::triangle_base(std::initializer_list<V> list)
//{
//    _vertices = container_t(list.begin(), list.end());
//}

template<triangle_vertex V>
inline const vector2& triangle_base<V>::a() const
{
    return get_t{}(_vertices[0]);
}

template<triangle_vertex V>
inline const vector2& triangle_base<V>::b() const
{
    return get_t{}(_vertices[1]);
}

template<triangle_vertex V>
inline const vector2& triangle_base<V>::c() const
{
    return get_t{}(_vertices[2]);
}

template<triangle_vertex V>
inline const V& triangle_base<V>::operator[](size_t i) const
{
    return _vertices[i % 3];
}

template<triangle_vertex V>
inline float triangle_base<V>::a_side() const
{
    return (a() - b()).length();
}

template<triangle_vertex V>
inline float triangle_base<V>::b_side() const
{
    return (b() - c()).length();
}

template<triangle_vertex V>
inline float triangle_base<V>::c_side() const
{
    return (c() - a()).length();
}

inline auto calc_determinant(const std::array<float, 9>& m)
{
    return
        (m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7]) - 
        (m[2] * m[4] * m[6] + m[0] * m[7] * m[5] + m[8] * m[1] * m[3]);
}

template<triangle_vertex V>
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

template<triangle_vertex V>
inline float triangle_base<V>::perimeter() const
{
    return a_side() + b_side() + c_side();
}

template<triangle_vertex V>
inline float triangle_base<V>::area() const
{
    auto p{perimeter() / 2};
    return std::sqrtf(p * (p - a_side()) * (p - b_side()) * (p - c_side()));
}

template<triangle_vertex V>
inline std::pair<V, V> triangle_base<V>::edge(size_t i) const
{
    return {_vertices[i % 3], _vertices[(i + 1) % 3]};
}

template<triangle_vertex V>
inline bool triangle_base<V>::is_sharp_enough(float threshold) const
{
    return false;
}

template<triangle_vertex V>
inline bool triangle_base<V>::has_vertex(const vector2& v) const
{
    return v == a() || v == b() || v == c();
}

template<triangle_vertex V>
inline bool triangle_base<V>::has_similar_vertex(const triangle_base<V>& other) const
{
    return has_vertex(other.a()) || has_vertex(other.b()) || has_vertex(other.c());
}

template<triangle_vertex V>
struct std::hash<triangle_base<V>>
{
    template <triangle_vertex V>
    size_t operator()(const triangle_base<V>& triangle) const
    {
        return
            std::hash<vector2>{}(triangle.a()) ^
            std::hash<vector2>{}(triangle.b()) ^
            std::hash<vector2>{}(triangle.c());
    }
};

inline bool operator==(const triangle& t1, const triangle& t2)
{
    return 
        t1.has_vertex(t2.a()) && 
        t1.has_vertex(t2.b()) && 
        t1.has_vertex(t2.c());
}