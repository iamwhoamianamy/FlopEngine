#pragma once

#include <functional>
#include <variant>
#include <string>

#include "object.hpp"
#include "split_layout.hpp"
#include "input_param_box.hpp"

#include "libs/logger/logger.hpp"

namespace flp::gui
{

struct input_param_list : public object
{
public:
    using init_elem_t = std::tuple<std::string, input_param_box::field_wrapper>;

public:
    [[nodiscard]] static std::shared_ptr<input_param_list>
    create(std::initializer_list<init_elem_t> init_list);

    virtual ~input_param_list() = default;

public:
    virtual void draw() override;
    virtual void resize(const frame_t& new_frame) override;

protected:
    input_param_list(std::initializer_list<init_elem_t> init_list);

protected:
    std::vector<std::shared_ptr<input_param_box>> _children;
    std::shared_ptr<split_layout> _layout;

private:
    friend struct master;
};

} // namespace gui