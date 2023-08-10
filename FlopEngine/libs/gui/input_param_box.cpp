#include "input_param_box.hpp"

#include "libs/graphics/drawing.hpp"
#include "libs/logger/logger.hpp"

using namespace gui;

void input_param_box::field_wrapper::updater::operator()(float* val)
{
    *val = std::stof(new_val);
}

void input_param_box::field_wrapper::updater::operator()(int* val)
{
    *val = std::stoi(new_val);
}

void input_param_box::field_wrapper::updater::operator()(bool* val)
{
    *val = static_cast<bool>(std::stoi(new_val));
}

void input_param_box::field_wrapper::updater::operator()(std::string* val)
{
    *val = new_val;
}

void gui::input_param_box::field_wrapper::update(const std::string& new_val)
{
    std::visit(updater{new_val}, _data);
}

input_param_box::field_wrapper::field_wrapper(data_t&& data)
    : _data{std::move(data)}
{

}

auto input_param_box::field_wrapper::as_string() const -> std::string
{
    std::string result;

    std::visit(
        [&](const auto& uderlying_data)
        {
            auto& val = *uderlying_data;

            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, bool>)
                result = std::format("{}", static_cast<int>(val));
            else
                result = std::format("{}", val);
        },
        _data);

    return result;
}

std::shared_ptr<input_param_box> 
gui::input_param_box::create(
    const std::string& label,
    field_wrapper data,
    const rectangle& boundary_rectangle)
{
    return std::shared_ptr<input_param_box>{
        new input_param_box{label, data, boundary_rectangle}
    };
}

void gui::input_param_box::draw()
{
    update_from_outer_sources();

    if (active())
        draw::set_line_stripple(1);

    draw::set_line_width(2);
    draw::set_color(draw::color::yellow());
    draw::draw_rect(object::boundary_rectangle());
    draw::disable_line_stripple();

    draw::set_line_width(1);
    draw::render_string(boundary_rectangle().center_left(), 20, data_as_string());
}

input_param_box::input_param_box(
    const std::string& label,
    field_wrapper data,
    const rectangle& boundary_rectangle)
    : object{boundary_rectangle}
    , _label{label}
    , _data{data}
    , _tmp_string{data.as_string()}
    , _last_updated_as_string{data.as_string()}
{
    on_key_pressed([this](keyboard_key_t key)
        {
            switch (key)
            {
                case keyboard_key_t{'\r'}:
                {
                    logger::log_trace("key pressed: enter");

                    commit_update();
                    
                    break;
                }
                case keyboard_key_t{'\b'}:
                {
                    logger::log_trace("key pressed: backspace");

                    if (_tmp_string.length())
                        _tmp_string.pop_back();

                    break;
                }
                default:
                {
                    logger::log_trace("key pressed: {}", key);

                    _tmp_string += key;

                    break;
                }
            }
        });
}

void input_param_box::commit_update()
{
    try
    {
        _data.update(_tmp_string);
        _last_updated_as_string = _data.as_string();

        logger::log_trace("update field '{}'", _label);
    }
    catch (const std::exception& e)
    {
        logger::log_error("failed to update field {}: {}", _label, e.what());
    }
}

auto input_param_box::data_as_string() const -> std::string
{
    return std::format("{}: {}", _label, _tmp_string);
}

void gui::input_param_box::update_from_outer_sources()
{
    auto true_data_as_string = _data.as_string();

    if (true_data_as_string != _tmp_string &&
        true_data_as_string != _last_updated_as_string)
    {
        _tmp_string = true_data_as_string;
        _last_updated_as_string = true_data_as_string;
    }
}
