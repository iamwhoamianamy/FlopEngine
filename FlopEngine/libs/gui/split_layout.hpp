#pragma once

#include <unordered_set>
#include <optional>

#include "libs/gui/layout.hpp"

namespace gui
{

struct split_layout : public layout
{
public:
    struct init_container
    {
        enum class markup
        {
            absolute = 0,
            relative = 1
        };

        using element_t   = std::tuple<object_ptr, markup, float>;
        using container_t = std::vector<element_t>;
        
        container_t elements;

        init_container(std::initializer_list<element_t> init_list);
    };

public:
    enum class orientation
    {
        horizontal = 0,
        vertical = 1
    };

public:
    [[nodiscard]] static std::shared_ptr<split_layout>
    create(std::shared_ptr<layout> parent, orientation o, init_container&& container);

    virtual ~split_layout() = default;

public:
    virtual void resize(const rectangle& boundary_rectangle) override;

protected:
    split_layout(std::shared_ptr<layout> parent, orientation o, init_container&& container);

private:
    void resize_children();

private:
    const orientation _orientation;
    init_container::container_t _init_container;
};

} // namespace gui