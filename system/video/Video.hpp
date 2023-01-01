#pragma once

#include "define.hpp"

#include "Matrix.hpp"
#include "Vec2.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "EnumOperators.hpp"

NS_BEGIN

namespace video
{

enum class AppearanceFlags
{
    IGNORE_CAMERA = 1 << 0,
};

enum class AnimationLayout: bool
{
    HORIZONTAL, VERTICAL
};

enum BlendMode
{
    NONE, MULTIPLY, ADD, SUBTRACT
};

// TODO: Move these somewhere more fitting later.
struct Transform2D
{
    operator Mat3f() const;

    fvec2d translate = {0.f, 0.f};
    float rotate = 0.f;
    fvec2d rotationCenter = {0.0f, 0.0f};
    fvec2d scale = {1.f, 1.f};
    fvec2d shear = {0.f, 0.f};
    bool reflectX = false;
    bool reflectY = false;
};

// TODO: Transform3D
struct VisualAppearance
{
    const Texture *texture = nullptr;
    const Mat3f *uvTransform = nullptr;
    BlendMode blendMode = BlendMode::MULTIPLY;
    color fillColor = WHITE;
    float outlineWidth = 0.01f;
    color outlineColor = BLACK;
    float zIndex = 0.0f;
    AppearanceFlags flags = static_cast<AppearanceFlags>(0);
};

constexpr VisualAppearance DEFAULT_APPEARANCE =
    VisualAppearance{
        .texture = nullptr,
        .uvTransform = nullptr,
        .blendMode = BlendMode::NONE,
        .fillColor = WHITE,
        .outlineWidth = 0.01f,
        .outlineColor = BLACK,
        .zIndex = 0.0f,
        .flags = static_cast<AppearanceFlags>(0)
    };

}

ENABLE_BITMASK_OPERATORS(video::AppearanceFlags)

NS_END
