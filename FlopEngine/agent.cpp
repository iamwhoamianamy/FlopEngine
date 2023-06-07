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

}