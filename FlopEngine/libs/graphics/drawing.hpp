#pragma once

#include <array>

#include "GL/freeglut.h"

#include "libs/geometry/vector2.hpp"
#include "libs/geometry/rectangle.hpp"
#include "libs/graphics/color.hpp"
#include "libs/grid/grid.hpp"

#include "libs/meta/range_of.hpp"
#include "libs/meta/trait_convertible_to.hpp"

namespace flp::draw
{

void draw_line(const vector2& a, const vector2& b);
void draw_point(const vector2& point, float size);
void draw_points(concepts::range_of<vector2> auto&& points, float size);
void draw_rect(const geo::rectangle& rect);
void draw_rect(const vector2& center, float half_width, float half_height);
void draw_rect(const vector2& a, const vector2& b, const vector2& c, const vector2& d);
void draw_triangle(const vector2& a, const vector2& b, const vector2& c, bool filled = false);
void draw_circle(const vector2& center, float radius);

void draw_filled_rect(const geo::rectangle& rect);

void draw_line_gradient(
    const vector2& a, const vector2& b,
    const color& a_color, const color& b_color);

void draw_color_grid(const grid<draw::color>& grid);
void draw_value_grid(const grid<float>& grid);

auto generate_random_color() -> color;

void render_string(const vector2& position, float size, const std::string& string);
void render_letter(const vector2& position, float size, char letter);

void set_background_color(const color& color);

void set_color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
void set_color(const color& color);
void set_line_width(float width);
void set_point_size(float size);
void set_line_stripple(float length);
void disable_line_stripple();

inline void draw_line(const vector2& a, const vector2& b)
{
    glBegin(GL_LINES);
    {
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
    }
    glEnd();
}

inline void draw_point(const vector2& point, float size)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

template<concepts::trait_convertible_to<vector2> Point>
inline void draw_points(concepts::range_of<Point> auto&& points, float size)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    {
        for (const auto& point : points)
        {
            const auto& position = traits::converter<Point, vector2>::convert(point);
            glVertex2f(position.x, position.y);
        }
    }
    glEnd();
}

inline void draw_rect(const geo::rectangle& rect)
{
    draw_rect(rect.center, rect.half_dimensions.x, rect.half_dimensions.y);
}

inline void draw_rect(const vector2& center, float half_width, float half_height)
{
    glBegin(GL_LINE_LOOP);
    {
        glVertex2f(center.x - half_width, center.y - half_height);
        glVertex2f(center.x + half_width, center.y - half_height);
        glVertex2f(center.x + half_width, center.y + half_height);
        glVertex2f(center.x - half_width, center.y + half_height);
    }
    glEnd();
}

inline void draw_rect(const vector2& a, const vector2& b, const vector2& c, const vector2& d)
{
    glBegin(GL_LINE_LOOP);
    {
        glVertex3f(a.x, a.y, 0);
        glVertex3f(b.x, b.y, 0);
        glVertex3f(c.x, c.y, 0);
        glVertex3f(d.x, d.y, 0);
    }
    glEnd();
}

inline void draw_filled_rect(const geo::rectangle& rect)
{
    glBegin(GL_POLYGON);
    {
        glVertex3f(rect.top_left().x,  rect.top_left().y,  0);
        glVertex3f(rect.top_right().x, rect.top_right().y, 0);
        glVertex3f(rect.bot_right().x, rect.bot_right().y, 0);
        glVertex3f(rect.bot_left().x,  rect.bot_left().y,  0);
    }
    glEnd();
}

inline void set_color(float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
}

inline void set_color(const color& color)
{
    glColor4f(color.r, color.g, color.b, color.a);
}

inline void set_line_width(float width)
{
    glLineWidth(width);
}

inline void set_point_size(float size)
{
    glPointSize(size);
}

inline void set_line_stripple(float length)
{
    glLineStipple(static_cast<GLint>(length), 0x00FF);
    glEnable(GL_LINE_STIPPLE);
}

inline void disable_line_stripple()
{
    glDisable(GL_LINE_STIPPLE);
}

inline void draw_line_gradient(
    const vector2& a, const vector2& b, 
    const color& a_color, const color& b_color)
{
    glBegin(GL_LINES);
    {
        set_color(a_color);
        glVertex2f(a.x, a.y);

        set_color(b_color);
        glVertex2f(b.x, b.y);
    }
    glEnd();
}

inline void draw_color_grid(const grid<draw::color>& grid)
{
    glDrawPixels(
        static_cast<GLsizei>(grid.width()),
        static_cast<GLsizei>(grid.height()),
        GL_RGBA,
        GL_FLOAT,
        grid.data());
}

inline void draw_value_grid(const grid<float>& grid)
{
    glDrawPixels(
        static_cast<GLsizei>(grid.width()),
        static_cast<GLsizei>(grid.height()),
        GL_LUMINANCE,
        GL_FLOAT,
        grid.data());
}

} // namespace flp::draw