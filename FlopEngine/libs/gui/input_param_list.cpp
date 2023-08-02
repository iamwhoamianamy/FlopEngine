#include "input_param_list.hpp"

using namespace gui;

std::shared_ptr<input_param_list> input_param_list::create(
    init_container&& init_container,
    const rectangle& boundary_rectangle)
{
    return std::shared_ptr<input_param_list>(
        new input_param_list{
            std::move(init_container),
            boundary_rectangle
        }
    );
}

void input_param_list::draw()
{
    for (auto& field : _fields.data)
    {

    }
}

input_param_list::input_param_list(
    init_container&& init_container, 
    const rectangle& boundary_rectangle)
    : object{boundary_rectangle}
    , _fields{std::move(init_container)}
{

}
