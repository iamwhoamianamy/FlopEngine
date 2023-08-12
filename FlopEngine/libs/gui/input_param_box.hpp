#pragma once

#include <functional>
#include <variant>
#include <string>

#include "libs/gui/object.hpp"

namespace gui
{

struct input_param_box : public object
{
public:
    struct field_wrapper
    {
        using data_t = std::variant<float*, int*, bool*, std::string*>;

        struct updater
        {
            const std::string& new_val;

            void operator()(float*       val);
            void operator()(int*         val);
            void operator()(bool*        val);
            void operator()(std::string* val);
        };

    public:
        explicit field_wrapper(data_t&& data);

    public:
        void update(const std::string& new_val);
        auto as_string() const -> std::string;

    private:

        data_t _data;
    };

public:
    [[nodiscard]] static std::shared_ptr<input_param_box>
    create(const std::string& label, field_wrapper data, const frame_t& frame = {});

    virtual ~input_param_box() = default;

public:
    virtual void draw() override;

protected:
    input_param_box(const std::string& label, field_wrapper data, const frame_t& frame);

private:
    void commit_update();
    auto data_as_string() const -> std::string;

    // TODO le kostil`
    void update_from_outer_sources();

protected:
    std::string _label;
    std::string _tmp_string;
    std::string _last_updated_as_string;

    field_wrapper _data;

private:
    friend struct master;
};

} // namespace gui