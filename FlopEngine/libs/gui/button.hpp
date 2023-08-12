#pragma once

#include "object.hpp"

namespace gui
{

struct button : public object
{
public:
    [[nodiscard]] static std::shared_ptr<button>
    create(const frame_t& frame = {});

    virtual ~button() = default;

public:
    virtual void draw() override;

protected:
    button(const frame_t& frame);
};

} // namespace gui