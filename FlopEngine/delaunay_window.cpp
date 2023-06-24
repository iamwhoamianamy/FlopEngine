#include "GL/freeglut.h"

#include "delaunay_window.hpp"
#include "drawing.hpp"
#include "delaunay_triangulator.hpp"

const size_t delaunay_window::_agent_count{400};

delaunay_window::delaunay_window(int argc, char** argv,
    float screen_width, float screen_height, std::string name)
    : base_window{argc, argv, screen_width, screen_height, name}
    , _agents{utils::agent::generate_random(screen_rectangle(), _agent_count, 50)}
{
}

void delaunay_window::calc_point_weights()
{
    auto increase_weight = [](auto& pair, float weight)
    {
        pair.first += weight;
        pair.second++;
    };

    _point_weights.clear();

    for (const auto& edge : _triangulation)
    {
        auto edge_length{edge.perimeter()};

        increase_weight(_point_weights[edge.a()], edge_length);
        increase_weight(_point_weights[edge.b()], edge_length);
    }
}

void delaunay_window::draw_with_edge_length()
{
    float max_true_length{screen_rectangle().diagonal() / 8};

    for (const auto& edge : _triangulation)
    {
        float width{math::map(edge.perimeter(), 0.0f, max_true_length, 6.0f, 2.0f)};
        float saturation{math::map(edge.perimeter(), 0.0f, max_true_length, 100.0f, 20.0f)};

        draw::color color_fixed(
            2.5f * saturation,
            2.0 * saturation,
            1.0 * saturation);

        draw::set_color(color_fixed);
        draw::set_line_width(width);
        draw::draw_line(edge[0], edge[1]);
    }
}

void delaunay_window::draw_with_edge_length_gradient()
{
    //auto [min_pair, max_pair] = std::minmax(
    //    _point_weights.begin(), _point_weights.end(),
    //    [](const auto& p1, const auto& p2)
    //    {
    //        return p1->second < p2->second;
    //    });
    float max_true_length{screen_rectangle().diagonal() / 8};

    auto min_saturation{0.0f};
    auto max_saturation{150.0f};

    for (const auto& edge : _triangulation)
    {
        float width{math::map(edge.perimeter(), 0.0f, max_true_length, 6.0f, 2.0f)};

        auto w1{_point_weights[edge.a()].first / _point_weights[edge.a()].second};
        auto w2{_point_weights[edge.b()].first / _point_weights[edge.b()].second};

        float saturation1{math::map(w1, min_saturation, max_saturation, 100.0f, 0.0f)};
        float saturation2{math::map(w2, min_saturation, max_saturation, 100.0f, 0.0f)};

        draw::color color_fixed1(
            2.5f * saturation1,
            2.0 * saturation1,
            1.0 * saturation1);

        draw::color color_fixed2(
            2.5f * saturation2,
            2.0 * saturation2,
            1.0 * saturation2);

        draw::set_line_width(width);
        draw::draw_line_gradient(edge[0], edge[1], color_fixed1, color_fixed2);
    }
}

void delaunay_window::physics_loop()
{
    for (auto& agent : _agents)
    {
        agent.update_position(1.0f, _last_ellapsed);
        // agent.bounce_from_borders(screen_rectangle());
        agent.go_through_borders(screen_rectangle());
    }

    _triangulation = triangulate(_agents);

    calc_point_weights();
}

void delaunay_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    draw_with_edge_length_gradient();

    draw::set_color(255, 255, 100);
    for (const auto& agent : _agents)
    {
        draw::draw_point(agent.position, 12);
    }

    glFinish();
}
