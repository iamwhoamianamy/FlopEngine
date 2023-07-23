#pragma once

#include "object.hpp"

namespace gui
{

struct button : public object
{
public:
    [[nodiscard]] static std::shared_ptr<button>
    create(const rectangle& boundary_rectangle = {});

    virtual ~button() = default;

public:
    virtual void draw() override;

protected:
    button(const rectangle& boundary_rectangle);
};

} // namespace gui