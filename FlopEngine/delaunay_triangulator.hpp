#pragma once
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <algorithm>

#include "triangle.hpp"
#include "utils.hpp"

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
    triangles_t triangles{encompassing_triangle};
    std::set<vector2> points_arranged;

    std::transform(
        points.begin(),
        points.end(),
        std::inserter(points_arranged, points_arranged.begin()),
        [](const auto& point)
        {
            using point_t = std::remove_const_t<std::remove_reference_t<decltype(point)>>;
            return get_vector2<point_t>{}(point);
        });

    for (const auto& point : points_arranged)
    {
        triangles_t to_remove;
        edges_t edges;

        for (const auto& tr : triangles)
        {
            auto [center, radius] = tr.get_circumcircle();

            if (vector2::is_close_enough(point, center, radius))
            {
                for (auto i : utils::iota(3))
                {
                    edges[tr.edge(i)]++;
                }

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
            return tr.has_similar_vertex(encompassing_triangle);
        });

    return triangles;
}
