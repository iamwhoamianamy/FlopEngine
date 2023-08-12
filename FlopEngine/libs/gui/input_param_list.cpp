#include "input_param_list.hpp"

using namespace gui;

std::shared_ptr<input_param_list> input_param_list::create(
    std::initializer_list<init_elem_t> init_list)
{
    return std::shared_ptr<input_param_list>(
        new input_param_list{
            init_list
        }
    );
}

void input_param_list::draw()
{
    _layout->draw();
}

void gui::input_param_list::resize(const frame_t& new_frame)
{
    _layout->resize(new_frame);
}

input_param_list::input_param_list(std::initializer_list<init_elem_t> init_list)
    : object{}
{
    std::vector<object_ptr> layout_init_vec;

    for (auto& [label, field_wrapper] : init_list)
    {
        auto box = input_param_box::create(label, field_wrapper);

        _children.push_back(box);
        layout_init_vec.push_back(box);
    }

    _layout = split_layout::create(
        split_layout::orientation::vertical,
        split_layout::init_container{
            layout_init_vec.begin(),
            layout_init_vec.end()
        }
    );
}
