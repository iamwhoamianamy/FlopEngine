#pragma once

#include <memory>
#include <functional>

#include "libs/geometry/rectangle.hpp"

namespace gui
{

struct master;

struct object : public std::enable_shared_from_this<object>
{
public:
    using callback_t = std::function<void()>;

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

    void on_press(callback_t&& callback);
    void on_release(callback_t&& callback);

    bool hidden() const;
    bool hovered_over() const;
    bool pressed() const;

protected:
    object();
    object(const rectangle& boundary_rectangle);

protected:
    void set_hovered_over(bool hovered_over);
    void set_pressed_status(bool pressed);

private:
    void init();

private:
    rectangle _boundary_rectangle;

    bool _hidden       = false;
    bool _hovered_over = false;
    bool _pressed      = false;

    callback_t _on_press;
    callback_t _on_release;

    friend class master;
};

} // namespace gui

