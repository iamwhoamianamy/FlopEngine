#pragma once

#include "barnes_hut_solver.hpp"

namespace flp
{
template<concepts::object_with_mass Object>
inline auto barnes_hut_solver<Object>::solve(
    concepts::range_of<Object> auto& objects) -> total_force_map_t
{
    _qtree = qtree_t{
        geo::rectangle::make_encompassing<Object>(objects),
        1};

    _qtree.insert(objects);

    init_masses();
    form_interaction_maps(_qtree.head(), _qtree.head());
    account_far_interactions();
    account_close_interactions();

    propagate_total_force();

    return _total_force_map;
}

template<concepts::object_with_mass Object>
inline void flp::barnes_hut_solver<Object>::init_masses()
{
    _qtree.traverse_by_width_reverse(
        [this](const qtree_node_t* node)
        {
            vector2 pos_mass_sum{};

            for (const auto& object : node->points())
            {
                const auto& object_mass =
                    traits::object_with_mass<Object>::mass(*object);

                const auto& object_pos =
                    traits::converter<Object*, vector2>::convert(object);

                pos_mass_sum += object_pos * object_mass;

                node->data.object_count++;
                node->data.total_mass += object_mass;
            }

            for (const auto& child : node->children())
            {
                node->data.object_count += child->data.object_count;
                node->data.total_mass   += child->data.total_mass;

                pos_mass_sum += child->data.center_of_mass * child->data.total_mass;
            }

            if (node->data.object_count)
                node->data.center_of_mass = pos_mass_sum / node->data.total_mass;
        }
    );
}

template<concepts::object_with_mass Object>
inline void barnes_hut_solver<Object>::propagate_total_force()
{
    _qtree.traverse_by_width(
        [this](const qtree_node_t* node)
        {
            for (const auto& child : node->children())
            {
                child->data.total_force += 
                    node->data.total_force *
                    child->data.total_mass /
                    node->data.total_mass;
            }

            for (const auto& object : node->points())
            {
                const auto& object_mass =
                    traits::object_with_mass<Object>::mass(*object);

                _total_force_map[const_cast<Object*>(object)] +=
                    node->data.total_force *
                    object_mass /
                    node->data.total_mass;
            }
        }
    );
}

template<concepts::object_with_mass Object>
inline void barnes_hut_solver<Object>::form_interaction_maps(
    const qtree_node_t* a,
    const qtree_node_t* b)
{
    if (a == b)
    {
        for (const auto& a_child : a->children())
        {
            for (const auto& b_child : b->children())
            {
                form_interaction_maps(a_child.get(), b_child.get());
            }
        }
    }

    if (a->useless() || b->useless())
    {
        return;
    }

    if (far_enough(a, b))
    {
        _far_interaction_map[b].insert(a);
    }
    else
    {
        if (!b->useless())
        {
            bool hildren_are_close_enough = true;

            for (const auto& child : a->children())
            {
                if (far_enough(child.get(), b))
                {
                    hildren_are_close_enough = false;
                    break;
                }
            }

            if (hildren_are_close_enough)
            {
                _close_interaction_map[b].insert(a);
            }
            else
            {
                for (const auto& child : a->children())
                {
                    form_interaction_maps(child.get(), b);
                }
            }
        }
        else
        {
            if ((b->boundary().radius() < a->boundary().radius() ||
                !b->useless()) && a->useless())
            {
                for (const auto& child : a->children())
                {
                    form_interaction_maps(child.get(), b);
                }
            }
            else
            {
                for (const auto& child : b->children())
                {
                    form_interaction_maps(a, child.get());
                }
            }
        }
    }
}

template<concepts::object_with_mass Object>
inline bool barnes_hut_solver<Object>::far_enough(
    const qtree_node_t* a,
    const qtree_node_t* b) const
{
    auto radius1 = a->boundary().radius();
    auto radius2 = b->boundary().radius();

    auto min_radius = std::min(radius1, radius2);
    auto max_radius = std::max(radius1, radius2);

    auto minimal_distance = 2.0 * max_radius + min_radius;

    auto distance_squared = (
        a->boundary().center -
        b->boundary().center).length_squared();

    return minimal_distance * minimal_distance < distance_squared;
}

template<concepts::object_with_mass Object>
inline void barnes_hut_solver<Object>::account_far_interactions()
{
    for (auto& [current_node, interaction_set] : _far_interaction_map)
    {
        for (auto& interaction_node : interaction_set)
        {
            current_node->data.total_force +=
                translated_force(interaction_node, current_node);
        }
    }
}

template<concepts::object_with_mass Object>
inline void barnes_hut_solver<Object>::account_close_interactions()
{
    for (auto& [current_node, interaction_set] : _close_interaction_map)
    {
        for (const auto& current_node_point : current_node->points())
        {
            const auto& current_node_point_pos =
                traits::converter<Object*, vector2>::convert(current_node_point);

            const auto& current_node_point_mass =
                traits::object_with_mass<Object>::mass(*current_node_point);

            for (const auto& interaction_node : interaction_set)
            {
                interaction_node->traverse_points_by_depth(
                    [&](const Object* interaction_node_point)
                    {
                        if (interaction_node_point == current_node_point)
                            return;

                        const auto& interaction_node_point_pos =
                            traits::converter<Object*, vector2>::convert(interaction_node_point);

                        const auto& interaction_node_point_mass =
                            traits::object_with_mass<Object>::mass(*interaction_node_point);

                        _total_force_map[const_cast<Object*>(current_node_point)] +=
                            calc_kernel(
                                interaction_node_point_pos,
                                current_node_point_pos,
                                interaction_node_point_mass,
                                current_node_point_mass);
                    });

                //if (interaction_node->empty() || interaction_node->subdivided())
                //    throw std::runtime_error{"wrong interaction node"};
            }
        }
    }
}

template<concepts::object_with_mass Object>
inline vector2 barnes_hut_solver<Object>::translated_force(
    const qtree_node_t* from,
    const qtree_node_t* to) const
{
    return calc_kernel(
        from->data.center_of_mass,
        to->data.center_of_mass,
        from->data.total_mass,
        to->data.total_mass);
}

template<concepts::object_with_mass Object>
inline vector2 barnes_hut_solver<Object>::calc_kernel(
    const vector2& from,
    const vector2& to,
    float mass_from,
    float mass_to) const
{
    const auto translation = from - to;

    return translation.normalized() *
        mass_from * mass_to /
        std::max(3.0f, translation.length_squared());
}

} // namespace flp