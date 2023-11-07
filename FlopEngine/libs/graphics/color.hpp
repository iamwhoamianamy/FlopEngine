#pragma once

#include <array>

namespace draw
{

struct color
{
    float r;
    float g;
    float b;
    float a;

    explicit color(float r = 0.0f, float g = 0, float b = 0, float a = 1);

    color& operator*=(float factor)
    {
        r *= factor;
        g *= factor;
        b *= factor;

        return *this;
    }

    static color red()    { return color{1.0f, 0.0f, 0.0f}; };
    static color green()  { return color{0.0f, 1.0f, 0.0f}; };
    static color blue()   { return color{0.0f, 0.0f, 1.0f}; };

    static color yellow() { return color{1.0f, 1.0f, 0.0f}; };
    static color orange() { return color{1.0f, 0.5f, 0.0f}; };
    static color purple() { return color{1.0f, 0.0f, 1.0f}; };

    static color white()  { return color{1.0f, 1.0f, 1.0f}; };
    static color black()  { return color{0.0f, 0.0f, 0.0f}; };
    static color gray()   { return color{0.6f, 0.6f, 0.6f}; };
    
    static color light_blue()  { return color{0.0f, 0.5f, 1.0f}; };
    static color warm_yellow() { return color{1.0f, 0.6f, 0.0f}; };
};

static std::array nice_colors = {color::warm_yellow(), color::gray(), color::light_blue()};

} // namespace draw