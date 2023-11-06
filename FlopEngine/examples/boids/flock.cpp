#include <algorithm>
#include <execution>
#include <limits>
#include <fstream>

#include "3rdparty/json/json.hpp"
#include "flock.hpp"
#include "utils/utils.hpp"

flock::flock() :
    _drawType(flock_draw_type::triangles_filled),
    _color(255, 255, 255),
    _quadtree(rectangle(vector2(), vector2()))
{
}

boid_parameters boid_parameters::create_from_file(const std::string& filename)
{
    auto json{nlohmann::json::parse(std::ifstream{filename.data()})};

    return {
        .avoid_vision      {json["avoid_vision"]},
        .avoid_strength    {json["avoid_strength"]},
        .align_vision      {json["align_vision"]},
        .align_strength    {json["align_strength"]},
        .gather_vision     {json["gather_vision"]},
        .gather_strength   {json["gather_strength"]},
        .flee_vision       {json["flee_vision"]},
        .flee_strength     {json["flee_strength"]},
        .wander_strength   {json["wander_strength"]},
        .size              {json["size"]},
        .max_speed         {json["max_speed"]},
        .min_speed         {json["min_speed"]},
        .march_contribution{json["march_contribution"]}
    };
}

void flock::init_random_on_screen(rectangle screen, size_t boids_count)
{
    auto agents{utils::agent::generate_random(screen, boids_count, _boid_params.max_speed)};

    std::transform(agents.begin(), agents.end(), std::back_inserter(_boids), 
        [](const utils::agent& agent)
        {
            return boid_t{agent.position, agent.velocity, agent.acceleration};
        });
}

void flock::go_through_window_borders(const rectangle& screen_borders)
{
    for (auto& boid : _boids)
    {
        boid.go_through_borders(screen_borders);
    }
}

void flock::bounce_from_window_borders(const rectangle& screen_borders)
{
    for (auto& boid : _boids)
    {
        boid.bounce_from_borders(screen_borders);
    }
}

draw::color& flock::color()
{
    return _color;
}

const std::vector<boid_t>& flock::boids() const
{
    return _boids;
}

flock_draw_type& flock::drawType()
{
    return _drawType;
}

void flock::draw() const
{
    draw::set_color(_color);

    switch (_drawType)
    {
        case flock_draw_type::points:
        {
            for (const auto& boid : _boids)
            {
                draw::draw_point(boid.position, _boid_params.size);
            }

            break;
        }
        case flock_draw_type::triangles:
        case flock_draw_type::triangles_filled:
        {
            for (const auto& boid : _boids)
            {
                vector2 direction = boid.velocity.normalized();
                vector2 perp_direction = direction.perp();

                vector2 a = boid.position + direction * _boid_params.size / 2;
                vector2 b = boid.position - direction * _boid_params.size / 2 + 
                    perp_direction * _boid_params.size / 3;
                vector2 c = boid.position - direction * _boid_params.size / 2 - 
                    perp_direction * _boid_params.size / 3;

                draw::draw_triangle(a, b, c, _drawType == flock_draw_type::triangles_filled);
            }

            break;
        }
        case flock_draw_type::letter:
        {
            char code = int(255 * _color.r) ^ int(255 * _color.g) ^ int(255 * _color.b);
            char letter = math::map(
                code,
                std::numeric_limits<char>::min(),
                std::numeric_limits<char>::max(),
                '0', '9');

            for(const auto& boid : _boids)
            {
                draw::render_letter(boid.position, _boid_params.size, letter);
            }

            break;
        }
    }
}

const quadtree_t& flock::quadtree() const
{
    return _quadtree;
}

void flock::set_params(const boid_parameters& new_params)
{
    _boid_params = new_params;
}

const boid_parameters& flock::params() const
{
    return _boid_params;
}
