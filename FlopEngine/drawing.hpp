#pragma once
#include "GL/freeglut.h"
#include "vector2.hpp"

namespace draw
{
    struct Color
    {
        UCHAR r;
        UCHAR g;
        UCHAR b;
        UCHAR a;

        Color(UCHAR r = 0, UCHAR g = 0, UCHAR b = 0, UCHAR a = 255);
    };
     
    inline void draw_line(vector2 a, vector2 b);
    inline void draw_point(vector2 point, float size);
    inline void draw_rect(vector2 center, float half_width, float half_height);
    inline void draw_rect(vector2 a, vector2 b, vector2 c, vector2 d);
    void draw_triangle(vector2 a, vector2 b, vector2 c, bool filled);
    Color generate_random_color();

    void render_string(vector2 position, float size, const std::string& string);
    void render_letter(vector2 position, float size, char letter);

    inline void set_color(GLubyte r = 255, GLubyte g = 255, GLubyte b = 255, GLubyte a = 255)
    {
        glColor4ub(r, g, b, a);
    }

    inline void set_color(Color color)
    {
        glColor4ub(color.r, color.g, color.b, color.a);
    }
}

inline void draw::draw_line(vector2 a, vector2 b)
{
    glBegin(GL_LINES);
    {
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
    }
    glEnd();
}

inline void draw::draw_point(vector2 point, float size)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

inline void draw::draw_rect(vector2 center, float half_width, float half_height)
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

inline void draw::draw_rect(vector2 a, vector2 b, vector2 c, vector2 d)
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