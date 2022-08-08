/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#pragma once

#include "define.hpp"

#include "Drawable.hpp"
#include "Types.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Renderer.dpp"

NS_BEGIN

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
    BlendMode blendMode = BlendMode::Multiply;
    color fillColor = WHITE;
    float outlineWidth = 0.003f;
    color outlineColor = BLACK;
    float zIndex = 0.0f;
};

constexpr VisualAppearance DEFAULT_APPEARANCE =
    VisualAppearance{
        .texture = nullptr,
        .uvTransform = nullptr,
        .blendMode = BlendMode::None,
        .fillColor = WHITE,
        .outlineWidth = 0,
        .outlineColor = BLACK,
        .zIndex = 0.0f
    };

namespace detail
{

void DrawGeneric2DShape(const Renderer &renderer, const Shader &shader, const Mesh &mesh,
                        const Mat3f &shape, const VisualAppearance &appearance,
                        const Mat3f &transform,
                        RenderMode mode = RenderMode::Triangles);

void DrawLineSegment(const Renderer &renderer, const dline &seg,
                     const VisualAppearance &appearance, const Mat3f &transform);
}

NS_END
