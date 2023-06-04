#include <algorithm>
#include <execution>
#include <limits>
#include <fstream>

#include "json.hpp"
#include "flock.hpp"
#include "utility.hpp"

flock_t::flock_t() :
    _drawType(flock_draw_type::triangles_filled),
    _color(255, 255, 255),
    _quadtree(rectangle(vector2(), vector2()))
{
}

boid_parameters boid_parameters::create_from_file(const std::string& filename)
{
    auto json = nlohmann::json::parse(std::ifstream(filename.data()));

    return {
        .avoid_vision = json["avoid_vision"],
        .avoid_strength = json["avoid_strength"],
        .align_vision = json["align_vision"],
        .align_strength = json["align_strength"],
        .gather_vision = json["gather_vision"],
        .gather_strength = json["gather_strength"],
        .flee_vision = json["flee_vision"],
        .flee_strength = json["flee_strength"],
        .wander_strength = json["wander_strength"],
        .size = json["size"],
        .max_speed = json["max_speed"],
        .min_speed = json["min_speed"],
        .march_contribution = json["march_contribution"]
    };
}

void flock_t::init_random_on_screen(
    float screen_width,
    float screen_height,
    size_t boidsCount)
{
    _boids.clear();
    _boids.reserve(boidsCount);

    for(size_t i = 0; i < boidsCount; i++)
    {
        vector2 position = {
            math::random_in_range(0, screen_width),
            math::random_in_range(0, screen_height) };
        _boids.emplace_back(position, math::generate_random_vector() * _boid_params.max_speed);
    }
}

void flock_t::go_through_window_borders(float _screen_width, float _screen_height)
{
    for (auto& boid : _boids)
    {
        float x = boid.position.x;
        float y = boid.position.y;

        if (x < 0)
        {
            boid.position.x = _screen_width - 1;
        }
        else
        {
            if (x >= _screen_width)
            {
                boid.position.x = 0;
            }
        }

        if (y < 0)
        {
            boid.position.y = _screen_height - 1;
        }
        else
        {
            if (y >= _screen_height)
            {
                boid.position.y = 0;
            }
        }

        //boid.position.x = std::max(std::min(boid.position.x, screenWidth - 1), 0.0f);
        //boid.position.y = std::max(std::min(boid.position.y, screenWidth - 1), 0.0f);
    }
}

draw::Color& flock_t::color()
{
    return _color;
}

const std::vector<boid_t>& flock_t::boids() const
{
    return _boids;
}

flock_draw_type& flock_t::drawType()
{
    return _drawType;
}

void flock_t::draw() const
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
                vector2 perpDirection = direction.perp();

                vector2 a = boid.position + direction * _boid_params.size / 2;
                vector2 b = boid.position - direction * _boid_params.size / 2 + 
                    perpDirection * _boid_params.size / 3;
                vector2 c = boid.position - direction * _boid_params.size / 2 - 
                    perpDirection * _boid_params.size / 3;

                draw::draw_triangle(a, b, c, _drawType == flock_draw_type::triangles_filled);
            }

            break;
        }
        case flock_draw_type::letter:
        {
            char code = _color.r ^ _color.g ^ _color.b;
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

const quadtree_t& flock_t::quadtree() const
{
    return _quadtree;
}

void flock_t::set_params(const boid_parameters& new_params)
{
    _boid_params = new_params;
}

const boid_parameters& flock_t::params() const
{
    return _boid_params;
}
