#pragma once

#include <unordered_map>
#include <unordered_set>

#include "libs/quadtree/quadtree.hpp"

namespace flp
{

namespace traits
{

template<typename T>
struct object_with_mass
{
    constexpr static float mass(T& t);
};

} // namespace traits

namespace concepts
{

template<typename Obj>
concept object_with_mass =
    concepts::trait_convertible_to<Obj*, vector2>
    && requires(Obj&& obj)
{
    { traits::object_with_mass<Obj>::mass(obj) } -> std::convertible_to<float>;
};

} // namespace concepts

struct qtree_node_data
{
    size_t  object_count{};
    float   total_mass{};
    vector2 total_force{};
    vector2 center_of_mass{};
};

template<concepts::object_with_mass Object>
struct barnes_hut_solver
{
    using qtree_node_t      = flp::quadtree_node_with_data<Object, qtree_node_data>;
    using qtree_t           = flp::quadtree<Object, qtree_node_t>;
    using interaction_map_t = std::unordered_map<const qtree_node_t*, std::unordered_set<const qtree_node_t*>>;
    using total_force_map_t = std::unordered_map<Object*, vector2>;

public:
    auto solve(concepts::range_of<Object> auto& objects) -> total_force_map_t;
    
private:
    void init_masses();
    void propagate_total_force();
    void form_interaction_maps(const qtree_node_t* a, const qtree_node_t* b);
    bool far_enough(const qtree_node_t* a, const qtree_node_t* b) const;
    void account_far_interactions();
    void account_close_interactions();

    vector2 translated_force(
        const qtree_node_t* from,
        const qtree_node_t* to) const;

    vector2 calc_kernel(
        const vector2& from,
        const vector2& to,
        float mass_from,
        float mass_to) const;

private:
    qtree_t           _qtree;
    interaction_map_t _far_interaction_map;
    interaction_map_t _close_interaction_map;
    total_force_map_t _total_force_map;
};

} // namespace flp

#include "barnes_hut_solver_impl.hpp"