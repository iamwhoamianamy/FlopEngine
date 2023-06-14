#pragma once
#include "GL/freeglut.h"
#include "vector2.hpp"

namespace draw
{

struct color
{
    UCHAR r;
    UCHAR g;
    UCHAR b;
    UCHAR a;

    color(UCHAR r = 0, UCHAR g = 0, UCHAR b = 0, UCHAR a = 255);

    color& operator*=(float factor)
    {
        r *= factor;
        g *= factor;
        b *= factor;

        return *this;
    }
};
     
void draw_line(const vector2& a, const vector2& b);
void draw_point(const vector2& point, float size);
void draw_rect(const vector2& center, float half_width, float half_height);
void draw_rect(const vector2& a, const vector2& b, const vector2& c, const vector2& d);
void draw_triangle(const vector2& a, const vector2& b, const vector2& c, bool filled = false);
void draw_circle(const vector2& center, float radius);
auto generate_random_color() -> color;

void render_string(const vector2& position, float size, const std::string& string);
void render_letter(const vector2& position, float size, char letter);

void set_color(GLubyte r = 255, GLubyte g = 255, GLubyte b = 255, GLubyte a = 255);
void set_color(const color& color);

} // namespace draw

inline void draw::draw_line(const vector2& a, const vector2& b)
{
    glBegin(GL_LINES);
    {
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
    }
    glEnd();
}

inline void draw::draw_point(const vector2& point, float size)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

inline void draw::draw_rect(const vector2& center, float half_width, float half_height)
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

inline void draw::draw_rect(const vector2& a, const vector2& b, const vector2& c, const vector2& d)
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

inline void draw::set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
    glColor4ub(r, g, b, a);
}

inline void draw::set_color(const color& color)
{
    glColor4ub(color.r, color.g, color.b, color.a);
}