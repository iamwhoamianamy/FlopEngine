#pragma once

#include <vector>

#include "boid.hpp"

#include "libs/graphics/drawing.hpp"
#include "libs/quadtree/quadtree.hpp"
#include "libs/meta/concepts.hpp"

namespace flp
{

enum class flock_draw_type
{
    points = 0,
    triangles,
    triangles_filled,
    letter,
    COUNT
};

struct boid_parameters
{
    float avoid_vision = 10;
    float avoid_strength = 40;

    float align_vision = 50;
    float align_strength = 0.01f;

    float gather_vision = 50;
    float gather_strength = 500;

    float flee_vision = 50;
    float flee_strength = 60;

    float wander_strength = 0.01f;
    float size = 8;

    float max_speed = 80;
    float min_speed = 60;

    float march_contribution = 15;

    static boid_parameters create_from_file(const std::string& filename);
};

template <>
struct flp::traits::converter<boid*, vector2>
{
    static vector2& convert(boid* boid)
    {
        return boid->position;
    }
};

using quadtree_t = quadtree<boid>;

class flock
{
private:
    std::vector<boid> _boids;
    flock_draw_type _drawType;
    draw::color _color;
    quadtree_t _quadtree;
    boid_parameters _boid_params;

public:
    bool debug = true;

    flock();

    void init_random_on_screen(geo::rectangle screen, size_t boids_count = 500);
    void update_boid_positions(float viscosity, concepts::duration auto ellapsed);
    void form_quadtree(const geo::rectangle& screen_borders);
    void perform_flocking_behaviour(concepts::duration auto ellapsed);
    void go_through_window_borders(const geo::rectangle& screen_borders);
    void bounce_from_window_borders(const geo::rectangle& screen_borders);
    void perform_fleeing(const flock& other, concepts::duration auto ellapsed);
    void draw() const;

    draw::color& color();
    const std::vector<boid>& boids() const;
    flock_draw_type& drawType();
    const quadtree_t& quadtree() const;

    void set_params(const boid_parameters& new_params);
    const boid_parameters& params() const;

private:
    void perform_avoiding(boid& boid, concepts::duration auto ellapsed);
    void perform_aligning(boid& boid, concepts::duration auto ellapsed);
    void perform_gathering(boid& boid, concepts::duration auto ellapsed);
    void perform_wandering(boid& boid, concepts::duration auto ellapsed);
};

inline void flock::update_boid_positions(
    float viscosity,
    concepts::duration auto ellapsed)
{
    std::for_each(std::execution::par, _boids.begin(), _boids.end(),
        [this, viscosity, ellapsed](boid& boid)
        {
            boid.update_position(viscosity, ellapsed);

            float speed = boid.velocity.length();

            if (speed < _boid_params.min_speed)
            {
                boid.velocity.set_length(_boid_params.min_speed);
            }
            else
            {
                if (_boid_params.max_speed < speed)
                {
                    boid.velocity.limit(_boid_params.max_speed);
                }
            }
        });
}

inline void flock::form_quadtree(const geo::rectangle& screen_borders)
{
    _quadtree = quadtree_t{screen_borders, 64};
    _quadtree.insert(_boids);
}

inline void flock::perform_flocking_behaviour(concepts::duration auto ellapsed)
{
    std::for_each(std::execution::par, _boids.begin(), _boids.end(),
        [this, ellapsed](boid& boid)
        {
            perform_avoiding(boid, ellapsed);
            perform_aligning(boid, ellapsed);
            perform_gathering(boid, ellapsed);
            perform_wandering(boid, ellapsed);
        });
}

inline void flock::perform_avoiding(boid& boid, concepts::duration auto ellapsed)
{
    auto boids_to_avoid = _quadtree.quarry(
        geo::rectangle(boid.position, _boid_params.avoid_vision));

    if (debug)
    {
        draw::set_color(draw::color(255, 0, 0));
        glLineWidth(3);
    }

    for (const auto& boid_to_avoid : boids_to_avoid)
    {
        boid.avoid(boid_to_avoid.position, _boid_params.avoid_strength, ellapsed);

        if (debug)
        {
            draw::draw_line(boid.position, boid_to_avoid.position);
        }
    }
}

inline void flock::perform_aligning(flp::boid& boid, concepts::duration auto ellapsed)
{
    struct projection
    {
        auto operator()(flp::boid& boid)
        {
            return boid.velocity;
        }
    };

    auto boids_to_align_to = _quadtree.quarry(
        geo::rectangle(boid.position, _boid_params.align_vision));

    boid.align(boids_to_align_to, _boid_params.align_strength, ellapsed, projection{});

    if (debug)
    {
        draw::set_color(draw::color(255, 255, 0));
        glLineWidth(2);

        for (const auto& boid_to_align_to : boids_to_align_to)
        {
            draw::draw_line(boid.position, boid_to_align_to.position);
        }
    }
}

inline void flock::perform_gathering(
    flp::boid& boid,
    concepts::duration auto ellapsed)
{
    struct projection
    {
        auto operator()(flp::boid& boid)
        {
            return boid.position;
        }
    };

    auto boids_to_gather_with = _quadtree.quarry(
        geo::rectangle(boid.position, _boid_params.gather_vision));

    boid.gather(boids_to_gather_with, _boid_params.gather_strength, ellapsed, projection{});
}

inline void flock::perform_wandering(
    flp::boid& boid,
    concepts::duration auto ellapsed)
{
    boid.wander(_boid_params.wander_strength, ellapsed);
}

inline void flock::perform_fleeing(
    const flock& other,
    concepts::duration auto ellapsed)
{
    for (auto& boid : _boids)
    {
        auto boids_to_flee_from = other.quadtree().quarry(
            geo::rectangle(boid.position, _boid_params.flee_vision));

        for (const auto& boid_to_flee_from : boids_to_flee_from)
        {
            boid.avoid(boid_to_flee_from.position, _boid_params.flee_strength, ellapsed);
        }
    }
}

} // namespace flp