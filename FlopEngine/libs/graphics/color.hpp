#pragma once

#include <array>

namespace flp::draw
{

struct color
{
    float r{};
    float g{};
    float b{};
    float a{};

    color() = default;
    constexpr explicit color(float saturation, float a = 1.0f);
    constexpr explicit color(float r, float g, float b, float a = 1.0f);

    constexpr color& operator*=(float factor)
    {
        r *= factor;
        g *= factor;
        b *= factor;

        return *this;
    }

    constexpr static color red()    { return color{1.0f, 0.0f, 0.0f}; };
    constexpr static color green()  { return color{0.0f, 1.0f, 0.0f}; };
    constexpr static color blue()   { return color{0.0f, 0.0f, 1.0f}; };

    constexpr static color yellow() { return color{1.0f, 1.0f, 0.0f}; };
    constexpr static color orange() { return color{1.0f, 0.5f, 0.0f}; };
    constexpr static color purple() { return color{1.0f, 0.0f, 1.0f}; };

    constexpr static color white()  { return color{1.0f, 1.0f, 1.0f}; };
    constexpr static color black()  { return color{0.0f, 0.0f, 0.0f}; };
    constexpr static color gray()   { return color{0.6f, 0.6f, 0.6f}; };
    
    constexpr static color light_blue()  { return color{0.0f, 0.5f, 1.0f}; };
    constexpr static color warm_yellow() { return color{1.0f, 0.6f, 0.0f}; };
};

static std::array nice_colors = {color::warm_yellow(), color::gray(), color::light_blue()};

inline constexpr draw::color::color(float saturation, float a)
    : r{saturation}
    , g{saturation}
    , b{saturation}
    , a{a}
{

}

constexpr inline color::color(float r, float g, float b, float a)
    : r{r}
    , g{g}
    , b{b}
    , a{a}
{

}

} // namespace flp::draw