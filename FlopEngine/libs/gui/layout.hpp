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

    [[nodiscard]] static std::shared_ptr<layout>
    create(layout* parent);
    
    virtual ~layout() = default;

public:
    virtual void draw() override;
    void add_child(object* child);

protected:
    layout(const rectangle& boundary_rectangle);
    layout(layout* parent);

protected:
    virtual void resize(const rectangle& boundary_rectangle) override;
    void resize(object* object, rectangle boundary_rectangle);
    auto fix_boundary_rectangle(const rectangle& boundary_rectangle) -> rectangle;

protected:
    std::optional<layout*> _parent;
    std::unordered_set<object*> _children;

private:
    friend struct master;
};

} // namespace gui