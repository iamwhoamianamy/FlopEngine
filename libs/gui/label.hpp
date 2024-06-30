#pragma once

#include "object.hpp"

namespace flp::gui
{

struct label : public object
{
public:
    [[nodiscard]] static std::shared_ptr<label>
    create(const std::string& text = "", const frame_t& frame = {});

    virtual ~label() = default;

public:
    virtual void draw() override;

protected:
    label(const std::string& text, const frame_t& frame);

protected:
    std::string _text;
};

} // namespace gui