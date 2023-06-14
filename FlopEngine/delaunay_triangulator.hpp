#pragma once
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "triangle.hpp"

class delaunay_triangulator
{
public:
    using triangles_t = std::unordered_set<triangle>;
    using edge_t      = std::pair<vector2, vector2>;
    using edges_t     = std::unordered_map<edge_t, size_t>;

public:
    auto triangulate(const std::ranges::range auto& points) -> triangles_t;

private:
    static const triangle encompassing_triangle;
};

auto delaunay_triangulator::triangulate(const std::ranges::range auto& points) -> triangles_t
{
    triangles_t triangles;

    triangles.insert(encompassing_triangle);

    for (const auto& _point : points)
    {
        using point_t = std::remove_const_t<std::remove_reference_t<decltype(_point)>>;
        auto point{get_vector2<point_t>{}(_point)};

        triangles_t to_remove;
        edges_t edges;

        for (const auto& tr : triangles)
        {
            auto [center, radius] = tr.get_circumcircle();

            if (vector2::is_close_enough(point, center, radius))
            {
                edges[edge_t(tr.a(), tr.b())]++;
                edges[edge_t(tr.b(), tr.c())]++;
                edges[edge_t(tr.c(), tr.a())]++;

                to_remove.insert(triangle{tr.a(), tr.b(), tr.c()});
            }
        }

        std::erase_if(edges,
            [](auto& item)
            {
                return item.second != 1;
            });

        for (const auto& tr : to_remove)
        {
            triangles.erase(tr);
        }

        for (const auto& [key, value] : edges)
        {
            auto& [a, b] = key;
            triangle new_tr{a, b, point};
            triangles.insert(new_tr);
        }
    }

    std::erase_if(triangles,
        [](const triangle& tr)
        {
            return
                tr.a() == encompassing_triangle.a() ||
                tr.a() == encompassing_triangle.b() ||
                tr.a() == encompassing_triangle.c() ||
                tr.b() == encompassing_triangle.a() ||
                tr.b() == encompassing_triangle.b() ||
                tr.b() == encompassing_triangle.c() ||
                tr.c() == encompassing_triangle.a() ||
                tr.c() == encompassing_triangle.b() ||
                tr.c() == encompassing_triangle.c();
        });

    return triangles;
}
