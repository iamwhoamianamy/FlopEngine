#pragma once

#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <algorithm>

#include "libs/geometry/triangle.hpp"
#include "libs/geometry/edge.hpp"
#include "utils/utils.hpp"

namespace detail
{

class delaunay_triangulator
{
public:
    using edge_t          = edge;
    using edges_t         = std::unordered_map<edge_t, size_t>;
    using triangulation_t = std::unordered_set<edge_t>;
    using triangles_t     = std::unordered_set<triangle>;

public:
    auto triangulate(const std::ranges::range auto& points) -> triangulation_t;

private:
    static const triangle encompassing_triangle;
};

auto delaunay_triangulator::triangulate(const std::ranges::range auto& points) -> triangulation_t
{
    triangles_t triangles{encompassing_triangle};
    std::set<vector2> points_arranged;

    std::transform(
        points.begin(),
        points.end(),
        std::inserter(points_arranged, points_arranged.begin()),
        [](const auto& point)
        {
            using point_t = std::decay_t<decltype(point)>;
            return flp::traits::converter<point_t, vector2>::convert(point);
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
                    edges[tr.get_edge(i)]++;
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
            triangle new_tr{key.a(), key.b(), point};
            triangles.insert(new_tr);
        }
    }

    std::erase_if(triangles,
        [](const triangle& tr)
        {
            return tr.has_similar_vertex(encompassing_triangle);
        });

    triangulation_t result;

    for (const auto& triangle : triangles)
    {
        for (auto i : utils::iota(3))
        {
            result.insert(triangle.get_edge(i));
        }
    }

    return result;
}

} // namespace detail

using triangulation_t = detail::delaunay_triangulator::triangulation_t;

inline triangulation_t
triangulate(const std::ranges::range auto& points)
{
    detail::delaunay_triangulator ator;
    return ator.triangulate(points);
}
