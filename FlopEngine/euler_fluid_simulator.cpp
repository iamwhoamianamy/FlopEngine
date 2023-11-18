#include "euler_fluid_simulator.hpp"

euler_fluid_simulator::euler_fluid_simulator(size_t width, size_t height)
    : _cells{width, height}
{

}

void euler_fluid_simulator::resize(size_t width, size_t height)
{
    _cells.reinit(width, height);

    static const float radius = 50;

    const auto center_x = static_cast<int>(width) / 2;
    const auto center_y = static_cast<int>(height) / 2;

    for (auto& [val, x, y] : _cells.as_plain_range())
    {
        if (std::abs(static_cast<int>(x) - center_x) +
            std::abs(static_cast<int>(y) - center_y) > radius)
        {
            val.obstalce = true;
        }
    }
}

auto euler_fluid_simulator::as_saturation_grid() const -> grid<float>
{
    grid<float> result{_cells.width(), _cells.height()};
    auto& g = *const_cast<grid<cell>*>(&_cells);

    for (auto& [val, x, y] : g.as_plain_range())
    {
        result.set(x, y, val.velocity.length());
    }

    return result;
}
