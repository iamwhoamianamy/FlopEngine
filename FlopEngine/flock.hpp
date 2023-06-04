#pragma once
#include <vector>

#include "boid.hpp"
#include "drawing.hpp"
#include "quadtree.hpp"
#include "concepts.hpp"

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
    float avoid_vision   {10};
    float avoid_strength {40};

    float align_vision   {50};
    float align_strength {0.01f};

    float gather_vision  {50};
    float gather_strength{500};

    float flee_vision    {50};
    float flee_strength  {60};

    float wander_strength{0.01f};

    float size           {8};

    float max_speed      {80};
    float min_speed      {60};

    static boid_parameters create_from_file(const std::string& filename);
};

namespace traits
{

template <>
struct access<boid_t>
{
    static vector2 position(boid_t* boid)
    {
        return boid->position;
    }
};

}

using quadtree_t = quadtree<boid_t, 8>;

class flock_t
{
private:
    std::vector<boid_t> _boids;
    flock_draw_type _drawType;
    draw::Color _color;
    quadtree_t _quadtree;
    boid_parameters _boid_params;

public:
    bool debug = false;

    flock_t();

    void init_random_on_screen(
        float screen_width,
        float screen_height,
        size_t boids_count = 500);

    void update_boid_positions(float viscosity, flp::duration auto ellapsed);
    void form_quadtree(const rectangle& screen_borders);
    void perform_flocking_behaviour(flp::duration auto ellapsed);
    void go_through_window_borders(float screen_width, float screen_height);
    void perform_fleeing(const flock_t& flock, flp::duration auto ellapsed);
    void draw() const;

    draw::Color& color();
    const std::vector<boid_t>& boids() const;
    flock_draw_type& drawType();
    const quadtree_t& quadtree() const;
    void set_params(const boid_parameters& new_params);

private:
    void perform_avoiding(boid_t& boid, flp::duration auto ellapsed);
    void perform_aligning(boid_t& boid, flp::duration auto ellapsed);
    void perform_gathering(boid_t& boid, flp::duration auto ellapsed);
    void perform_wandering(boid_t& boid, flp::duration auto ellapsed);
};

inline void flock_t::update_boid_positions(float viscosity, flp::duration auto ellapsed)
{
    std::for_each(std::execution::par, _boids.begin(), _boids.end(),
        [this, viscosity, ellapsed](boid_t& boid)
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

inline void flock_t::form_quadtree(const rectangle& screen_borders)
{
    _quadtree = quadtree_t(screen_borders);
    _quadtree.insert(_boids);
}

inline void flock_t::perform_flocking_behaviour(flp::duration auto ellapsed)
{
    std::for_each(std::execution::par, _boids.begin(), _boids.end(),
        [this, ellapsed](boid_t& boid)
        {
            perform_avoiding(boid, ellapsed);
            perform_aligning(boid, ellapsed);
            perform_gathering(boid, ellapsed);
            perform_wandering(boid, ellapsed);
        });
}

inline void flock_t::perform_avoiding(boid_t& boid, flp::duration auto ellapsed)
{
    auto boids_to_avoid = _quadtree.quarry(
        rectangle(boid.position, _boid_params.avoid_vision));

    if (debug)
    {
        draw::set_color(draw::Color(255, 0, 0));
        glLineWidth(3);

    }

    for (auto& boid_to_avoid : boids_to_avoid)
    {
        boid.avoid(boid_to_avoid->position, _boid_params.avoid_strength, ellapsed);

        if (debug)
        {
            draw::draw_line(boid.position, boid_to_avoid->position);
        }
    }
}

inline void flock_t::perform_aligning(boid_t& boid, flp::duration auto ellapsed)
{
    struct projection
    {
        auto operator()(boid_t* boid)
        {
            return boid->velocity;
        }
    };

    auto boids_to_align_to = _quadtree.quarry(
        rectangle(boid.position, _boid_params.align_vision));

    boid.align(boids_to_align_to, _boid_params.align_strength, ellapsed, projection{});

    if (debug)
    {
        draw::set_color(draw::Color(255, 255, 0));
        glLineWidth(2);

        for (auto& boid_to_align_to : boids_to_align_to)
        {
            draw::draw_line(boid.position, boid_to_align_to->position);
        }
    }

}

inline void flock_t::perform_gathering(boid_t& boid, flp::duration auto ellapsed)
{
    struct projection
    {
        auto operator()(boid_t* boid)
        {
            return boid->position;
        }
    };

    auto boidsToGatherWith =
        _quadtree.quarry(rectangle(boid.position, _boid_params.gather_vision));

    boid.gather(boidsToGatherWith, _boid_params.gather_strength, ellapsed, projection{});
}

inline void flock_t::perform_wandering(boid_t& boid, flp::duration auto ellapsed)
{
    boid.wander(_boid_params.wander_strength, ellapsed);
}

inline void flock_t::perform_fleeing(const flock_t& flock, flp::duration auto ellapsed)
{
    for (auto& boid : _boids)
    {
        auto boids_to_flee_from = flock.quadtree().quarry(
            rectangle(boid.position, _boid_params.flee_vision));

        for (const auto& boid_to_flee_from : boids_to_flee_from)
        {
            boid.avoid(boid_to_flee_from->position, _boid_params.flee_strength, ellapsed);
        }
    }
}