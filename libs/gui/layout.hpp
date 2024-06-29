#pragma once

#include <unordered_set>
#include <optional>

#include "object.hpp"

namespace flp::gui
{

struct layout : public object
{
public:
    [[nodiscard]] static std::shared_ptr<layout>
    create(const frame_t& frame = {});
    
    virtual ~layout() = default;

public:
    virtual void draw() override;
    void add_child(object* child);

protected:
    layout(const frame_t& frame);

protected:
    virtual void resize(const frame_t& new_frame) override;
    auto fix_frame(const frame_t& frame) -> frame_t;

protected:
    std::unordered_set<object*> _children;

private:
    friend struct master;
};

} // namespace gui