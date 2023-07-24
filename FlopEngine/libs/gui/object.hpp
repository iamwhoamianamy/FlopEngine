#pragma once

#include <functional>
#include <optional>

#include "libs/geometry/rectangle.hpp"
#include "libs/gui/defines.hpp"

namespace gui
{

struct object : public std::enable_shared_from_this<object>
{
public:
    constexpr static float border_width = 4.0f;

public:
    [[nodiscard]] static object_ptr
    create(const rectangle& boundary_rectangle = {});

    virtual ~object() = default;

public:
    const rectangle& boundary_rectangle() const;

    void on_press(callback_t&& callback);
    void on_release(callback_t&& callback);

    bool hidden() const;
    bool hovered_over() const;
    bool pressed() const;
    bool active() const;

protected:
    object(const rectangle& boundary_rectangle = {});

protected:
    virtual void resize(const rectangle& boundary_rectangle);

protected:
    void set_hovered_over(bool hovered_over);
    void set_pressed(bool pressed);
    void set_active(bool active);

    void release();
    virtual void draw();

private:
    void init();

private:
    rectangle _boundary_rectangle;

    bool _hidden       = false;
    bool _hovered_over = false;
    bool _pressed      = false;
    bool _active       = false;

    std::optional<callback_t> _on_press;
    std::optional<callback_t> _on_release;

    friend struct master;
    friend struct layout;
};

} // namespace gui

