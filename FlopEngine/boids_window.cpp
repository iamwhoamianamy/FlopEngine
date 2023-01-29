#include <thread>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <execution>

#include "boids_window.hpp"
#include "drawing.hpp"

boids_window::boids_window(
    int argc, char** argv,
    float screen_width, float screen_height,
    std::string name) :
    base_window(argc, argv, screen_width, screen_height, name)
{
    for(auto& flock_entry : _flocks)
    {
        flock_entry.init_random_on_screen(_screen_width, _screen_height, boid_per_flock);
        flock_entry.color() = draw::generate_random_color();
    }

    read_boid_params();
    start_watching_for_boid_param_file_change();
    start_physics();
}

void boids_window::start_watching_for_boid_param_file_change()
{
    _lock = std::unique_lock(_mutex, std::defer_lock);
    std::jthread([this]()
        {
            watch_for_boid_param_file_change();
        }).detach();
}

void boids_window::start_physics()
{
    std::jthread([this]
        {
            while (true)
            {
                perform_physics_loop();
            }
        }).detach();
}

void boids_window::perform_flocking_physics()
{
    std::for_each(std::execution::par, _flocks.begin(), _flocks.end(),
        [=](flock_t& flock)
        {
            flock.form_quadtree(
                rectangle(
                    {_screen_width / 2, _screen_height / 2},
                    {_screen_width / 2, _screen_height / 2})
            );
            flock.perform_flocking_behaviour(_last_ellapsed);
        });

    for (size_t i = 0; i < _flocks.size(); i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            _flocks[i].perform_fleeing(_flocks[j], _last_ellapsed);
            _flocks[j].perform_fleeing(_flocks[i], _last_ellapsed);
        }
    }
}

void boids_window::perform_marching_physics()
{
    for (size_t i = 0; i < boids_window::flock_count; i++)
    {
        for (const auto& boid : _flocks[i].boids())
        {
            _marching_grids[i].add_contribution_bump(boid.position, 15, _screen_width, _screen_height);
        }
    }
}

void boids_window::perform_physics_loop()
{
    auto start = std::chrono::steady_clock::now();
    auto desirable_end = start + physics_interval;
    perform_flocking_physics();

    for (auto& flock : _flocks)
    {
        flock.update_boid_positions(_viscosity, _last_ellapsed);
        flock.go_through_window_borders(_screen_width, _screen_height);
    }

    std::this_thread::sleep_until(desirable_end);
    _last_ellapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
}

void boids_window::watch_for_boid_param_file_change()
{
    while (true)
    {
        auto last_modified = std::filesystem::last_write_time(boid_param_filename);
        auto now = std::chrono::file_clock::now();
        auto deb = decltype(last_modified)::clock::time_point(last_modified);
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - deb);

        if (seconds.count() < 5)
        {
            if (_lock.try_lock())
            {
                read_boid_params();

                _lock.unlock();
                _cv.notify_all();
            }
            else
            {
                _cv.wait(_lock);

                _lock.lock();
                read_boid_params();
                _lock.unlock();
                _cv.notify_all();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void boids_window::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    
    try
    {
        if (_lock.try_lock())
        {
            _lock.unlock();
            _cv.notify_all();
        }
        else
        {
            _cv.wait(_lock);
        }
    }
    catch(...)
    {

    }

    //if (_drawMarchingSquares)
    //{
    //    std::jthread flockPhysicsHandler(
    //        flockPhysics, std::ref(_flocks), screenWidth, screenHeight, FPS);

    //    std::jthread marchingPhysicsHandler(
    //        marchingPhysics, std::ref(_flocks), std::ref(_marchingGrid), screenWidth, screenHeight);
    //}
    //else
    //{
    //    flockPhysics(_flocks, screenWidth, screenHeight, FPS);
    //}

    //for(auto& flock_entry : _flocks)
    //{
    //    flock_entry.update_boid_positions(_viscosity, 1.0 / FPS);
    //    flock_entry.go_through_window_borders(screenWidth, screenHeight);
    //}

    if(_drawBoids)
    {
        for(auto& flock : _flocks)
        {
            flock.draw();
        }
    }

    if(_draw_marching_squares)
    {
        glLineWidth(1);

        for (size_t i = 0; i < flock_count; i++)
        {
            draw::set_color(_flocks[i].color());
            _marching_grids[i].march_all_cells(_screen_width, _screen_height);
            _marching_grids[i].clear();
        }
    }

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

void boids_window::mouse(int button, int state, int x, int y)
{
}

void boids_window::mouse_passive(int x, int y)
{
}

void boids_window::exiting_function()
{
}

void boids_window::read_boid_params()
{
    try
    {
        auto newParams = boid_parameters::create_from_file(boid_param_filename);

        for (auto& flock : _flocks)
        {
            flock.set_params(newParams);
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}