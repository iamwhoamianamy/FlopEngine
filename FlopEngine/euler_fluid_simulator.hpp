#pragma once

#include "libs/geometry/vector2.hpp"
#include "libs/grid/grid.hpp"
#include "libs/meta/concepts.hpp"
#include "utils/utils.hpp"

struct euler_fluid_simulator
{
public:
    struct cell
    {
        vector2 velocity;
        bool obstalce = false;
    };

public:
    static constexpr const float gravity = 0.98f;

public:
    euler_fluid_simulator(size_t width = size_t{}, size_t height = size_t{});

public:
    void resize(size_t width, size_t height);
    void update(flp::duration auto&& ellapsed);
    auto as_saturation_grid() const -> grid<float>;

private:
    void account_initial_flow(flp::duration auto&& ellapsed);
    void account_gravity(flp::duration auto&& ellapsed);
    void dither(flp::duration auto&& ellapsed);

private:
    grid<cell> _cells;
};

void euler_fluid_simulator::update(flp::duration auto&& ellapsed)
{
    account_initial_flow(ellapsed);
    account_gravity(ellapsed);
    dither(ellapsed);
}

inline void euler_fluid_simulator::account_initial_flow(flp::duration auto&& ellapsed)
{
    for (auto row_id : utils::iota(_cells.height()))
    {
        _cells.get(0, row_id).velocity.x = 1.0f;
    }
}

void euler_fluid_simulator::account_gravity(flp::duration auto&& ellapsed)
{
    //for (auto& [val, x, y] : _cells.as_plain_range())
    //{
    //    val.velocity.y += utils::true_ellapsed(ellapsed) * gravity;
    //}
}

inline void euler_fluid_simulator::dither(flp::duration auto&& ellapsed)
{
    auto next_step = _cells;

    //for (auto& [val, x, y] : _cells.as_plain_range())
    //{
    //    size_t neighbour_count;
    //    vector2 delta{};

    //    if (auto c = _cells.top(x, y))
    //    {
    //        neighbour_count++;
    //        delta += (*c)->velocity.y;
    //    }

    //    if (auto c = _cells.bot(x, y))
    //    {
    //        neighbour_count++;
    //        delta += (*c)->velocity.y;
    //    }

    //    if (auto c = _cells.left(x, y))
    //    {
    //        neighbour_count++;
    //        delta += (*c)->velocity.x;
    //    }

    //    if (auto c = _cells.right(x, y))
    //    {
    //        neighbour_count++;
    //        delta += (*c)->velocity.x;
    //    }

    //    /*_cells.bot(x, y).and_then(
    //        [&](cell* c)
    //        {
    //            neighbours.push_back(c);
    //        });

    //    _cells.left(x, y).and_then(
    //        [&](cell* c)
    //        {
    //            neighbours.push_back(c);
    //        });

    //    _cells.right(x, y).and_then(
    //        [&](cell* c)
    //        {
    //            neighbours.push_back(c);
    //        });*/

    //    delta /= neighbour_count;

    //    for (auto& neighbour : neighbours)
    //    {
    //        neighbour->velocity += delta;
    //    }
    //}

    _cells.swap(next_step);
}
