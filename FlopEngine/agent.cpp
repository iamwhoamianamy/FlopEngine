#include "agent.hpp"

namespace utils
{

agent::agent(
    const vector2& position,
    const vector2& velocity,
    const vector2& acceleration)
    : position{position}
    , velocity{velocity}
    , acceleration{acceleration}
{
}

void agent::go_through_borders(const rectangle& screen_borders)
{
    auto& x{position.x};
    auto& y{position.y};

    if (x < 0)
    {
        x = screen_borders.width() - 1;
    }
    else
    {
        if (x >= screen_borders.width())
        {
            x = 0;
        }
    }

    if (y < 0)
    {
        y = screen_borders.height() - 1;
    }
    else
    {
        if (y >= screen_borders.height())
        {
            y = 0;
        }
    }
}

void agent::bounce_from_borders(const rectangle& screen_borders)
{
    auto& x{position.x};
    auto& y{position.y};

    if (x < 0)
    {
        x = 0;
        velocity.x *= -1;
    }
    else
    {
        if (x >= screen_borders.width())
        {
            x = screen_borders.width() - 1;
            velocity.x *= -1;
        }
    }

    if (y < 0)
    {
        y = 0;
        velocity.y *= -1;
    }
    else
    {
        if (y >= screen_borders.height())
        {
            y = screen_borders.height() - 1;
            velocity.y *= -1;
        }
    }
}

}