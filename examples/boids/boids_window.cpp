#include <thread>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <execution>

#include "boids_window.hpp"
#include "libs/graphics/drawing.hpp"
#include "utils/utils.hpp"
#include "libs/logger/logger.hpp"
#include "libs/gui/master.hpp"

using namespace flp;
using namespace std::chrono_literals;

boids_window::boids_window(flp::window_settings&& settings)
    : base_window{std::move(settings)}
    , _boid_param_file_observer{
        flp::utils::file_observer::settings{
            boid_param_filename, 100ms
        },
        [this](){ read_boid_params(); }
    }
{
    for(auto& flock_entry : _flocks)
    {
        flock_entry.init_random_on_screen(
            screen_rectangle(),
            boid_per_flock);

        static int i = 0;

        if (i < draw::nice_colors.size())
            flock_entry.color() = draw::nice_colors[i++];
        else
            flock_entry.color() = draw::generate_random_color();
    }

    logger::open(logger::detail::log_level::all);
    setup_gui();
}

void boids_window::perform_flocking_physics()
{
    std::for_each(std::execution::par, _flocks.begin(), _flocks.end(),
        [=](flock& flock)
        {
            flock.form_quadtree(
                geo::rectangle(
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
    if (_perform_marching_physics)
    {
        for (auto i : utils::iota(flock_count))
        {
            for (const auto& boid : _flocks[i].boids())
            {
                _marching_grids[i].add_contribution_cone(boid.position, _flocks[i].params().march_contribution);
            }
        }
    }
}

void boids_window::update_boid_positions()
{
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

void boids_window::physics_loop()
{
    block_on_param_file_update();

    perform_flocking_physics();
    perform_marching_physics();

    update_boid_positions();
}

void boids_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    draw::set_background_color(draw::color::black());

    draw_focks();
    draw_marching_squares();

    glFinish();
}

void boids_window::keyboard_letters(unsigned char key, int x, int y)
{
    bool known_key = true;

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
        case 'x':
        {
            for (size_t i = 0; i < _flocks.size(); i++)
            {
                const float saturation_step = 1.0f / _flocks.size();
                _flocks[i].color() = draw::color{saturation_step * (i + 1)};
            }

            break;
        }
        case 'z':
        {
            for (size_t i = 0; i < _flocks.size(); i++)
            {
                _flocks[i].color() = draw::nice_colors[i];
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
            _draw_boids = !_draw_boids;

            break;
        }
        case 'm':
        {
            _perform_marching_physics = !_perform_marching_physics;

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
            break;
        }
        case '\x1b':
        {
            _master_hidden = !_master_hidden;
            utils::singleton<gui::master>::get().set_hidden(_master_hidden);
            break;
        }
        default:
        {
            known_key = false;
        }
    }

    if (known_key)
        logger::log_trace("key: '{}'", (char)key);
    else
        logger::log_error("unknown key: '{}'", (char)key);
}

void boids_window::resize(float w, float h)
{
    for (auto& grid : _marching_grids)
    {
        grid.resize(_screen_w, _screen_h);
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
    if (_draw_boids)
    {
        for (auto& flock : _flocks)
        {
            flock.draw();
        }
    }
}

void boids_window::draw_marching_squares()
{
    if (_perform_marching_physics)
    {
        glLineWidth(1);

        for (auto i : utils::iota(flock_count))
        {
            draw::set_color(_flocks[i].color());
            _marching_grids[i].march_all_cells(0.3f);
            _marching_grids[i].march_all_cells(0.4f);
            _marching_grids[i].march_all_cells(0.5f);
            _marching_grids[i].march_all_cells(0.6f);
            _marching_grids[i].march_all_cells(0.7f);
            _marching_grids[i].clear();
        }
    }
}

void boids_window::setup_gui()
{
    // pmf param list
    _pmf_param_list = gui::input_param_list::create(
        {
            {
                "perform marching physics", 
                gui::input_param_box::field_wrapper{
                    &_perform_marching_physics
                }
            },
            {
                "draw boids", 
                gui::input_param_box::field_wrapper{
                    &_draw_boids
                }
            }
        }
    );

    // right button
    _right_button = gui::button::create("Le button");

    _right_button->on_press([]
        {
            logger::log_debug("mouse pressed right button!");
        });

    _right_button->on_release([]
        {
            logger::log_debug("mouse released right button!");
        });

    // split layout
    _main_split_layout = gui::split_layout::create(
        gui::split_layout::orientation::horizontal,
        gui::split_layout::init_container{
            {_pmf_param_list,  gui::split_layout::init_container::markup::relative, 0.3f},
            {_right_button,    gui::split_layout::init_container::markup::relative, 0.7f}
        }
    );

    // finilize
    utils::singleton<gui::master>::get().set_layout(_main_split_layout.get());
    utils::singleton<gui::master>::get().set_hidden(_master_hidden);
}
