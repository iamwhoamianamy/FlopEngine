#pragma once

#include "barnes_hut_solver.hpp"

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

template <>
struct flp::traits::object_with_mass<flp::body>
{
    constexpr static float mass(flp::body& obj)
    {
        return obj.mass;
    }

    constexpr static float mass(const flp::body& obj)
    {
        return obj.mass;
    }
};

} // namespace flp

namespace flp
{

struct attraction_window : public flp::base_window
{
private:
    std::vector<body> _agents;

public:
    attraction_window(flp::window_settings&& settings);

    void physics_loop() override;
    void display() override;
    void resize(float w, float h) override;

};

} // namespace flp