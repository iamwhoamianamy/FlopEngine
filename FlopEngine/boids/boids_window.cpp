#include <thread>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <execution>

#include "boids_window.hpp"
#include "libs/graphics/drawing.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

boids_window::boids_window(
    int argc, char** argv,
    float screen_width, float screen_height,
    std::string name) :
    base_window(argc, argv, screen_width, screen_height, name),
    _boid_param_file_observer{boid_param_filename, 100ms, [this](){ read_boid_params(); }}
{
    for(auto& flock_entry : _flocks)
    {
        flock_entry.init_random_on_screen(
            screen_rectangle(),
            boid_per_flock);

        static int i = 0;
        flock_entry.color() = draw::nice_colors[i++];
    }

    _color_button = gui::button::create(rectangle{screen_rectangle().center, 100, 50});
}

void boids_window::perform_flocking_physics()
{
    std::for_each(std::execution::par, _flocks.begin(), _flocks.end(),
        [=](flock_t& flock)
        {
            flock.form_quadtree(
                rectangle(
                    {_screen_w / 2, _screen_h / 2},
                    {_screen_w / 2, _screen_h / 2})
            );
            flock.perform_flocking_behaviour(_last_ellapsed);
        });

    for (auto i : utils::iota(flock_count))
    {
        for (auto j : utils::iota(i))
        {
            _flocks[i].perform_fleeing(_flocks[j], _last_ellapsed);
            _flocks[j].perform_fleeing(_flocks[i], _last_ellapsed);
        }
    }
}

void boids_window::perform_marching_physics()
{
    for (auto i : utils::iota(flock_count))
    {
        for (const auto& boid : _flocks[i].boids())
        {
            _marching_grids[i].add_contribution_bump(boid.position, _flocks[i].params().march_contribution, _screen_w, _screen_h);
        }
    }
}

void boids_window::physics_loop()
{
    block_on_param_file_update();

    perform_flocking_physics();
    perform_marching_physics();

    for (auto& flock : _flocks)
    {
        flock.update_boid_positions(_viscosity, _last_ellapsed);

        if (_bounce)
        {
            flock.bounce_from_window_borders(screen_rectangle());
        }
        else
        {
            flock.go_through_window_borders(screen_rectangle());
        }
    }
}

void boids_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);

    draw_focks();
    draw_marching_squares();

    glFinish();
}

void boids_window::keyboard_letters(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'c':
        {
            for(auto& flock : _flocks)
            {
                flock.color() = draw::generate_random_color();
            }

            break;
        }
        case 'f':
        {
            glutFullScreenToggle();
            break;
        }
        case 'r':
        {
            for(auto& flock : _flocks)
            {
                flock.drawType() = (flock_draw_type)(
                    ((int)flock.drawType() + 1) % static_cast<int>(flock_draw_type::COUNT));
            }

            break;
        }
        case 'b':
        {
            _drawBoids = !_drawBoids;

            break;
        }
        case 'm':
        {
            _draw_marching_squares = !_draw_marching_squares;

            break;
        }
        case 'n':
        {
            _bounce = !_bounce;

            break;
        }
        case 's':
        {
            if (_smooth)
            {
                glDisable(GL_LINE_SMOOTH);
                glDisable(GL_POLYGON_SMOOTH);
            }
            else
            {
                glEnable(GL_LINE_SMOOTH);
                glEnable(GL_POLYGON_SMOOTH);
                glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
                glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
            }

            _smooth = !_smooth;

            break;
        }
        case 'o':
        {
            read_boid_params();
        }
    }
}

void boids_window::read_boid_params()
{
    try
    {
        auto new_params = boid_parameters::create_from_file(boid_param_filename);

        for (auto& flock : _flocks)
        {
            flock.set_params(new_params);
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void boids_window::block_on_param_file_update()
{
    try
    {
        _boid_param_file_observer.wait_for_unblocking();
    }
    catch (...)
    {

    }
}

void boids_window::draw_focks()
{
    if (_drawBoids)
    {
        for (auto& flock : _flocks)
        {
            flock.draw();
        }
    }
}

void boids_window::draw_marching_squares()
{
    if (_draw_marching_squares)
    {
        glLineWidth(1);

        for (auto i : utils::iota(flock_count))
        {
            draw::set_color(_flocks[i].color());
            _marching_grids[i].march_all_cells(_screen_w, _screen_h);
            _marching_grids[i].clear();
        }
    }
}
