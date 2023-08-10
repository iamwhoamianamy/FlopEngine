#pragma once

#include <unordered_set>
#include <optional>

#include "libs/gui/object.hpp"

namespace gui
{

struct layout : public object
{
public:
    [[nodiscard]] static std::shared_ptr<layout>
    create(const rectangle& boundary_rectangle = {});
    
    virtual ~layout() = default;

public:
    virtual void draw() override;
    void add_child(object* child);

protected:
    layout(const rectangle& boundary_rectangle);

protected:
    virtual void resize(const rectangle& boundary_rectangle) override;
    auto fix_boundary_rectangle(const rectangle& boundary_rectangle) -> rectangle;

protected:
    std::unordered_set<object*> _children;

private:
    friend struct master;
};

} // namespace gui