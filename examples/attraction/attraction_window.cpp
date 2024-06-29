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
        screen_rectangle(), 10000, 0.0f,
        [](flp::body& b)
        {
            static std::mt19937 generator;
            //static std::normal_distribution<float> dist{4.0f, 1.0f};
            static std::negative_binomial_distribution<int> dist{1, 0.05f};
            b.mass = 10.0f + dist(generator);
        });
}

void attraction_window::physics_loop()
{

}

void attraction_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    flp::barnes_hut_solver<flp::body> solver;
    auto total_forces = solver.solve(_agents);

    for (auto& agent : _agents)
    {
        agent.acceleration += total_forces[&agent] * 150.0f;
        agent.update_position(1.0f, _last_ellapsed);
    }

    for (const auto& body : _agents)
    {
        auto mass_factor = std::powf(body.mass, 1.0f / 3.0f);

        draw::set_color(
            draw::color{
                0.2f * mass_factor,
                0.05f * mass_factor,
                0.0f, 0.2f * mass_factor});

        draw::draw_point(body.position, mass_factor);
    }

    glFinish();
}

void attraction_window::resize(float w, float h)
{

}
