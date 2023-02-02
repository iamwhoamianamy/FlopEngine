#pragma once
#include <vector>
#include "boid.hpp"
#include "drawing.hpp"
#include "quadtree.hpp"

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
    float align_strength = 0.01;

    float gather_vision = 50;
    float gather_strength = 500;

    float flee_vision = 50;
    float flee_strength = 60;

    float wander_strength = 0.01;

    float size = 8;

    float max_speed = 80;
    float min_speed = 60;

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

using quadtree_t = typename quadtree<boid_t, 8>;

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

    void update_boid_positions(float viscosity, std::chrono::milliseconds ellapsed);
    void form_quadtree(const rectangle& screen_borders);
    void perform_flocking_behaviour(std::chrono::milliseconds ellapsed);
    void go_through_window_borders(float screen_width, float screen_height);
    void perform_fleeing(const flock_t& flock, std::chrono::milliseconds ellapsed);
    void draw() const;

    draw::Color& color();
    const std::vector<boid_t>& boids() const;
    flock_draw_type& drawType();
    const quadtree_t& quadtree() const;
    void set_params(const boid_parameters& new_params);
private:
    void perform_avoiding(boid_t& boid, std::chrono::milliseconds ellapsed);
    void perform_aligning(boid_t& boid, std::chrono::milliseconds ellapsed);
    void perform_gathering(boid_t& boid, std::chrono::milliseconds ellapsed);
    void perform_wandering(boid_t& boid, std::chrono::milliseconds ellapsed);
};