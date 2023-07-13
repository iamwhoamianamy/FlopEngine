#pragma once

#include <memory>

#include "libs/geometry/rectangle.hpp"

namespace gui
{

struct master;

struct object : public std::enable_shared_from_this<object>
{
public:
    [[noexcept]] static std::shared_ptr<object>
    create();

    [[noexcept]] static std::shared_ptr<object>
    create(const rectangle& boundary_rectangle);

    virtual ~object();

public:
    // TODO mb not public...
    virtual void draw();

public:
    const rectangle& boundary_rectangle() const;
    bool hidden() const;
    bool hovered_over() const;

protected:
    object();
    object(const rectangle& boundary_rectangle);

protected:
    void set_hovered_over(bool is_hovered_over);

private:
    void init();

private:
    rectangle _boundary_rectangle;
    bool _hidden = false;
    bool _hovered_over = false;

    friend class master;
};

} // namespace gui

