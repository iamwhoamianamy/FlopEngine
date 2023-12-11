#pragma once

#include "libs/graphics/base_window.hpp"
#include "utils/agent.hpp"
#include "libs/quadtree/quadtree.hpp"

namespace flp
{

struct body : public utils::agent
{
    float mass = 1.0f;

    body(
        const vector2& pos = {},
        const vector2& vel = {},
        const vector2& acc = {},
        float mass = 1.0f)
        : agent{pos, vel, acc}
        , mass{mass}
    {

    }
};

template<>
struct flp::traits::physics_object<flp::body>
{
    constexpr static vector2& position(flp::body& obj)
    {
        return obj.position;
    }

    constexpr static vector2& velocity(flp::body& obj)
    {
        return obj.velocity;
    }

    constexpr static vector2& acceleration(flp::body& obj)
    {
        return obj.acceleration;
    }
};


template <>
struct flp::traits::converter<flp::body, vector2>
{
    static flp::concepts::base_same_as<vector2> auto&& convert(
        flp::concepts::base_same_as<flp::body> auto&& a)
    {
        return a.position;
    }
};

template <>
struct flp::traits::converter<flp::body*, vector2>
{
    static flp::concepts::base_same_as<vector2> auto&& convert(
        flp::concepts::base_same_as<flp::body*> auto&& a)
    {
        return a->position;
    }
};

} // namespace flp

namespace flp
{

struct attraction_window : public flp::base_window
{
private:
    std::vector<body> _agents;
    geo::rectangle _agent_center;
    std::vector<std::pair<vector2, vector2>> _edges;
    quadtree<body> _qtree;

public:
    attraction_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;

private:
    void attract(flp::body& a, flp::body& b);
};

} // namespace flp