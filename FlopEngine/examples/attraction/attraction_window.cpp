#include "attraction_window.hpp"

#include <algorithm>
#include <ranges>
#include <random>

#include "GL/freeglut.h"

#include "libs/graphics/drawing.hpp"
#include "libs/quadtree/quadtree_help.hpp"

using namespace flp;

attraction_window::attraction_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
{
    _agents = utils::agent::generate_random<flp::body>(
        screen_rectangle(), 4000, 1.0f,
        [](flp::body& b)
        {
            static std::mt19937 generator;
            static std::normal_distribution<float> dist{3.0f, 2.0f};
            b.mass = dist(generator);
        });

    //_agents.emplace_back(screen_rectangle().center, vector2{}, vector2{}, 30.0f);
}

void attraction_window::physics_loop()
{
    _edges.clear();
    _agent_center = calc_agent_center();

    _qtree = decltype(_qtree){_agent_center, 32};
    _qtree.insert(_agents);

    const float radius = 200.0f;
    
    for (auto& agent : _agents)
    {
        auto range = geo::rectangle{agent.position, radius};

        for (auto& neighbour : _qtree.quarry_as_range(range))
        {
            attract(agent, neighbour);
            _edges.emplace_back(agent.position, neighbour.position);
        }
    }

    for (auto& agent : _agents)
    {
        agent.update_position(1.0f, _last_ellapsed);
    }
}

void attraction_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /*draw::set_line_width(1.0f);
    draw::set_color(draw::color{0.4f, 0.2f, 0.1f, 0.01f});
    for (const auto& [a, b] : _edges)
    {
        draw::draw_line(a, b);
    }*/

    //draw::set_line_width(0.1f);
    //draw::set_color(draw::color{0.4f, 0.4f, 0.0f, 0.1f});
    //draw_quadtree(_qtree);

    for (const auto& body : _agents)
    {
        draw::set_color(draw::color{0.2f * body.mass, 0.05f * body.mass, 0.0f, 0.2f * body.mass});
        draw::draw_point(body.position, body.mass);

        //draw::set_color(draw::color{0.2f * body.mass, 0.05f * body.mass, 0.0f, 0.01f * body.mass});
        //draw::draw_circle(body.position, 200.0f);
    }

    glFinish();
}

void attraction_window::resize(float w, float h)
{

}

void attraction_window::attract(flp::body& a, flp::body& b)
{
    if (a.position == b.position)
        return;

    const float scale = 200.0f;

    auto direction = b.position - a.position;
    auto distance = direction.length();

    auto force = scale / (distance * distance) * a.mass * b.mass;

    a.acceleration += direction * force;
    b.acceleration -= direction * force;
}

auto attraction_window::calc_agent_center() -> geo::rectangle
{
    auto [min_x, max_x] = std::ranges::minmax_element(_agents,
        [](const utils::agent& a, const utils::agent& b)
        {
            return a.position.x < b.position.x;
        });

    auto [min_y, max_y] = std::ranges::minmax_element(_agents,
        [](const utils::agent& a, const utils::agent& b)
        {
            return a.position.y < b.position.y;
        });

    return geo::rectangle::make_from_two_corners(
        vector2{min_x->position.x, min_y->position.y},
        vector2{max_x->position.x, max_y->position.y}
    );
}
