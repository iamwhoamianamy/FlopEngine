#include "split_layout.hpp"

#include <numeric>

using namespace gui;

std::shared_ptr<split_layout> gui::split_layout::create(
    std::shared_ptr<layout> parent,
    orientation o,
    init_container&& container)
{
    return std::shared_ptr<split_layout>(new split_layout{parent, o, std::move(container)});
}

void gui::split_layout::resize(const rectangle& boundary_rectangle)
{
    layout::resize(boundary_rectangle);
    resize_children();
}

split_layout::split_layout(
    std::shared_ptr<layout> parent, 
    orientation o, 
    init_container&& container)
    : layout{fix_boundary_rectangle(parent->boundary_rectangle())}
    , _orientation{o}
    , _init_container{container.elements}
{
    parent->add_child(this);

    for (const auto& tuple : _init_container)
    {
        add_child(std::get<0>(tuple).get());
    }

    resize_children();
}

void gui::split_layout::resize_children()
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

        layout::resize(child.get(), new_rectangle);
    }
}

gui::split_layout::init_container::init_container(std::initializer_list<element_t> init_list)
    : elements{init_list}
{

}
