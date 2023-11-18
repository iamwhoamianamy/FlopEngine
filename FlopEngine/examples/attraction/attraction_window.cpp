#include "attraction_window.hpp"

#include <algorithm>
#include <ranges>

#include "GL/freeglut.h"

#include "libs/graphics/drawing.hpp"

template<>
struct traits::access<utils::agent>
{
    static auto position(utils::agent* agent)
    {
        return agent->position;
    }
};

attraction_window::attraction_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
{
    _agents = utils::agent::generate_random(screen_rectangle(), 2000, 0);
}

void attraction_window::physics_loop()
{
    _agent_center = calc_agent_center();

    quadtree<utils::agent> qtree{_agent_center, 32};
    qtree.insert(_agents);

    const float radius = 200.0f;
    
    for (auto& agent : _agents)
    {
        auto range = rectangle{agent.position, radius};

        for (auto& neighbour : qtree.quarry_as_range(range))
        {
            attract(agent, neighbour);
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

    draw::set_color(draw::color::white());
    draw::draw_point(_agent_center.center, 10);

    draw::set_color(draw::color::orange());

    for (const auto agent : _agents)
    {
        draw::draw_point(agent.position, 1.0f);
    }

    glFinish();
}

void attraction_window::resize(float w, float h)
{

}

void attraction_window::attract(utils::agent& a, utils::agent& b)
{
    if (a.position == b.position)
        return;

    const float scale = 1000.0f;

    auto direction = b.position - a.position;
    auto distance = direction.length();

    auto force = scale / (distance * distance);

    a.acceleration += direction * force;
    b.acceleration -= direction * force;
}

auto attraction_window::calc_agent_center() -> rectangle
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

    return rectangle::make_from_two_corners(
        vector2{min_x->position.x, min_y->position.y},
        vector2{max_x->position.x, max_y->position.y}
    );
}
