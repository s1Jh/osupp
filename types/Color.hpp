#pragma once

#include "define.hpp"
#include <iostream>

#define DECOMPOSE_COLOR_RGBA(obj) obj.r, obj.g, obj.b, obj.a
#define DECOMPOSE_COLOR_RGB(obj) obj.r, obj.g, obj.b

NS_BEGIN

struct color8;
struct color;

struct color
{
    constexpr color()
        : r(0), g(0), b(0), a(1)
    {}

    constexpr color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 0xff)
        : r(float(_r) / 255.f), g(float(_g) / 255.f), b(float(_b) / 255.f),
          a(float(_a) / 255.f)
    {}

    float r, g, b, a;

    inline operator color8() const;

    inline operator uint32_t() const
    {
        uint32_t rval = 0;
        rval |= ((uint32_t) (r * 255.f)) << 0;
        rval |= ((uint32_t) (g * 255.f)) << 8;
        rval |= ((uint32_t) (b * 255.f)) << 16;
        rval |= ((uint32_t) (a * 255.f)) << 24;
        return rval;
    }
};

struct color8
{
    constexpr color8()
        : r(0), g(0), b(0), a(255)
    {}

    constexpr color8(float _r, float _g, float _b, float _a = 0xff)
        : r(uint8_t(_r * 255.f)), g(uint8_t(_g * 255.f)), b(uint8_t(_b * 255.f)),
          a(uint8_t(_a * 255.f))
    {}

    constexpr explicit color8(uint32_t rgba)
    {
        r = (rgba & (0xff << 0)) >> 0;
        g = (rgba & (0xff << 8)) >> 8;
        b = (rgba & (0xff << 16)) >> 16;
        a = (rgba & (0xff << 24)) >> 24;
    }

    uint8_t r, g, b, a;

    inline operator color() const;

    inline operator uint32_t() const
    {
        uint32_t rval = 0;
        rval |= ((uint32_t) (r)) << 0;
        rval |= ((uint32_t) (g)) << 8;
        rval |= ((uint32_t) (b)) << 16;
        rval |= ((uint32_t) (a)) << 24;
        return rval;
    }
};

inline color::operator color8() const
{ return {r, g, b, a}; }

inline color8::operator color() const
{ return {r, g, b, a}; }

inline std::ostream &operator<<(std::ostream &os, const color &dt)
{
    os << std::hex << "(" << dt.r << " " << dt.g << " " << dt.b << " " << dt.a
       << ")";

    return os;
}

constexpr color TRANSPARENT = {0x00, 0x00, 0x00, 0x00};

constexpr color AMARANTH = {0xE5, 0x2B, 0x50};

constexpr color AMBER = {0xFF, 0xBF, 0x00};

constexpr color AMETHYST = {0x99, 0x66, 0xCC};

constexpr color APRICOT = {0xFB, 0xCE, 0xB1};

constexpr color AQUAMARINE = {0x7F, 0xFF, 0xD4};

constexpr color AZURE = {0x00, 0x7F, 0xFF};

constexpr color BABY_BLUE = {0x89, 0xCF, 0xF0};

constexpr color BEIGE = {0xF5, 0xF5, 0xDC};

constexpr color BRICK_RED = {0xCB, 0x41, 0x54};

constexpr color BLACK = {0x00, 0x00, 0x00};

constexpr color BLUE = {0x00, 0x00, 0xFF};

constexpr color BLUE_GREEN = {0x00, 0x95, 0xB6};

constexpr color BLUE_VIOLET = {0x8A, 0x2B, 0xE2};

constexpr color BLUSH = {0xDE, 0x5D, 0x83};

constexpr color BRONZE = {0xCD, 0x7F, 0x32};

constexpr color BROWN = {0x96, 0x4B, 0x00};

constexpr color BURGUNDY = {0x80, 0x00, 0x20};

constexpr color BYZANTIUM = {0x70, 0x29, 0x63};

constexpr color CARMINE = {0x96, 0x00, 0x18};

constexpr color CERISE = {0xDE, 0x31, 0x63};

constexpr color CERULEAN = {0x00, 0x7B, 0xA7};

constexpr color CHAMPAGNE = {0xF7, 0xE7, 0xCE};

constexpr color CHARTREUSE_GREEN = {0x7F, 0xFF, 0x00};

constexpr color CHOCOLATE = {0x7B, 0x3F, 0x00};

constexpr color COBALT_BLUE = {0x00, 0x47, 0xAB};

constexpr color COFFEE = {0x6F, 0x4E, 0x37};

constexpr color COPPER = {0xB8, 0x73, 0x33};

constexpr color CORAL = {0xFF, 0x7F, 0x50};

constexpr color CRIMSON = {0xDC, 0x14, 0x3C};

constexpr color CYAN_AQUA = {0x00, 0xFF, 0xFF};

constexpr color DESERT_SAND = {0xED, 0xC9, 0xAF};

constexpr color ELECTRIC_BLUE = {0x7D, 0xF9, 0xFF};

constexpr color EMERALD = {0x50, 0xC8, 0x78};

constexpr color ERIN = {0x00, 0xFF, 0x3F};

constexpr color GOLD = {0xFF, 0xD7, 0x00};

constexpr color GRAY = {0x80, 0x80, 0x80};

constexpr color GREEN = {0x00, 0xFF, 0x00};

constexpr color HARLEQUIN = {0x3F, 0xFF, 0x00};

constexpr color INDIGO = {0x4B, 0x00, 0x82};

constexpr color IVORY = {0xFF, 0xFF, 0xF0};

constexpr color JADE = {0x00, 0xA8, 0x6B};

constexpr color JUNGLE_GREEN = {0x29, 0xAB, 0x87};

constexpr color LAVENDER = {0xB5, 0x7E, 0xDC};

constexpr color LEMON = {0xFF, 0xF7, 0x00};

constexpr color LILAC = {0xC8, 0xA2, 0xC8};

constexpr color LIME = {0xBF, 0xFF, 0x00};

constexpr color MAGENTA = {0xFF, 0x00, 0xFF};

constexpr color MAGENTA_ROSE = {0xFF, 0x00, 0xAF};

constexpr color MAROON = {0x80, 0x00, 0x00};

constexpr color MAUVE = {0xE0, 0xB0, 0xFF};

constexpr color NAVY_BLUE = {0x00, 0x00, 0x80};

constexpr color OCHRE = {0xCC, 0x77, 0x22};

constexpr color OLIVE = {0x80, 0x80, 0x00};

constexpr color ORANGE = {0xFF, 0x66, 0x00};

constexpr color ORANGE_RED = {0xFF, 0x45, 0x00};

constexpr color ORCHID = {0xDA, 0x70, 0xD6};

constexpr color PEACH = {0xFF, 0xE5, 0xB4};

constexpr color PEAR = {0xD1, 0xE2, 0x31};

constexpr color PERIWINKLE = {0xCC, 0xCC, 0xFF};

constexpr color PERSIAN_BLUE = {0x1C, 0x39, 0xBB};

constexpr color PINK = {0xFD, 0x6C, 0x9E};

constexpr color PLUM = {0x8E, 0x45, 0x85};

constexpr color PRUSSIAN_BLUE = {0x00, 0x31, 0x53};

constexpr color PUCE = {0xCC, 0x88, 0x99};

constexpr color PURPLE = {0x80, 0x00, 0x80};

constexpr color RASPBERRY = {0xE3, 0x0B, 0x5C};

constexpr color RED = {0xFF, 0x00, 0x00};

constexpr color RED_VIOLET = {0xC7, 0x15, 0x85};

constexpr color ROSE = {0xFF, 0x00, 0x7F};

constexpr color RUBY = {0xE0, 0x11, 0x5F};

constexpr color SALMON = {0xFA, 0x80, 0x72};

constexpr color SANGRIA = {0x92, 0x00, 0x0A};

constexpr color SAPPHIRE = {0x0F, 0x52, 0xBA};

constexpr color SCARLET = {0xFF, 0x24, 0x00};

constexpr color SILVER = {0xC0, 0xC0, 0xC0};

constexpr color SLATE_GRAY = {0x70, 0x80, 0x90};

constexpr color SPRING_BUD = {0xA7, 0xFC, 0x00};

constexpr color SPRING_GREEN = {0x00, 0xFF, 0x7F};

constexpr color TAN = {0xD2, 0xB4, 0x8C};

constexpr color TAUPE = {0x48, 0x3C, 0x32};

constexpr color TEAL = {0x00, 0x80, 0x80};

constexpr color TURQUOISE = {0x40, 0xE0, 0xD0};

constexpr color ULTRAMARINE = {0x3F, 0x00, 0xFF};

constexpr color VIOLET = {0x7F, 0x00, 0xFF};

constexpr color VIRIDIAN = {0x40, 0x82, 0x6D};

constexpr color WHITE = {0xFF, 0xFF, 0xFF};

constexpr color YELLOW = {0xFF, 0xFF, 0x00};

NS_END
