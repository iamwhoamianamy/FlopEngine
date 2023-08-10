#include "split_layout.hpp"

#include <numeric>

using namespace gui;

std::shared_ptr<split_layout> split_layout::create(
    orientation o,
    init_container&& container)
{
    return std::shared_ptr<split_layout>(new split_layout{o, std::move(container)});
}

void split_layout::resize(const rectangle& boundary_rectangle)
{
    layout::resize(boundary_rectangle);
    resize_children();
}

split_layout::split_layout(
    orientation o, 
    init_container&& container)
    : layout{{}}
    , _orientation{o}
    , _init_container{container.elements}
{
    for (const auto& tuple : _init_container)
    {
        add_child(std::get<0>(tuple).get());
    }

    resize_children();
}

void split_layout::resize_children()
{
    const auto stretch_axis = 
        _orientation == orientation::horizontal 
        ? 0 
        : 1;

    const auto const_axis = stretch_axis ^ 1;
    
    const auto& rect = object::boundary_rectangle();

    float rest_length = rect.half_dimensions[stretch_axis] * 2.0f;

    for (const auto& [child, markup, factor] : _init_container)
    {
        vector2 center;
        vector2 half_dim;

        half_dim[const_axis] = rect.half_dimensions[const_axis];
        center[const_axis] = rect.center[const_axis];

        half_dim[stretch_axis] = factor * rect.half_dimensions[stretch_axis];
        center[stretch_axis] =
            rect.center[stretch_axis] + rect.half_dimensions[stretch_axis] - rest_length +
            half_dim[stretch_axis];

        rectangle new_rectangle{center, half_dim};

        rest_length -= new_rectangle.half_dimensions[stretch_axis] * 2.0f;

        new_rectangle = layout::fix_boundary_rectangle(new_rectangle);

        child->resize(new_rectangle);
    }
}

split_layout::init_container::init_container(
    std::initializer_list<element_t> init_list)
    : elements{init_list}
{

}

split_layout::init_container::init_container(
    std::initializer_list<object_ptr> init_list)
{
    elements.reserve(init_list.size());
    const float elem_ratio = (1.0f / init_list.size());

    for (auto object : init_list)
        elements.emplace_back(object, markup::relative, elem_ratio);
}
