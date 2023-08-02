#pragma once

#include <functional>
#include <variant>
#include <string>

#include "libs/gui/object.hpp"
#include "libs/gui/split_layout.hpp"
#include "libs/logger/logger.hpp"

namespace gui
{

struct input_param_list : public object
{
public:
    struct init_container
    {
        struct element
        {
            using data_t = std::variant<float*, int*, bool*, std::string*>;

            struct updater
            {
                const std::string& new_val;

                void operator()(float* val)
                {
                    *val = std::stof(new_val);
                }

                void operator()(int* val)
                {
                    *val = std::stoi(new_val);
                }

                void operator()(bool* val)
                {
                    *val = static_cast<bool>(std::stoi(new_val));
                }

                void operator()(std::string* val)
                {
                    *val = new_val;
                }
            };

            element(auto& val) :
                data{&val}
            {

            }

            void update(const std::string& new_val)
            {
                try
                {
                    std::visit(updater{new_val}, data);
                }
                catch (const std::exception& e)
                {
                    logger::log_error("{}", e.what());
                }
            }

            std::string _label;
            data_t data;
        };

        using element_t   = element;
        using container_t = std::vector<element_t>;

        init_container(std::initializer_list<element_t> init_list)
            : data{init_list}
        {

        }

        container_t data;
    };

public:
    [[nodiscard]] static std::shared_ptr<input_param_list>
    create(init_container&& init_container, const rectangle& boundary_rectangle = {});

    virtual ~input_param_list() = default;

public:
    virtual void draw() override;

protected:
    input_param_list(init_container&& init_container, const rectangle& boundary_rectangle);

protected:
    init_container _fields;

private:
    friend struct master;
};

} // namespace gui