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

//
// Created by sijh on 29.03.22.
//
#pragma once

#include "define.hpp"

NS_BEGIN

static const char *CIRCLE_VERTEX_SHADER =
    "#version 330 core\nlayout (location = 0) in vec3 aPos;uniform vec2 "
    "position;uniform vec2 resolution;uniform float radius;uniform float "
    "aratio;uniform vec4 fill;uniform vec4 outline;uniform uint "
    "outline_thickness;out vec2 oResolution;out vec2 oCenter;out vec4 "
    "oFill;out vec4 oOutline;flat out float oRadius;flat out uint "
    "oOutlineThickness;void main(){oResolution = resolution;oFill = "
    "fill;oOutline = outline;oCenter = position;oOutlineThickness = "
    "outline_thickness;oRadius = radius;vec3 pos = aPos;pos *= radius;    "
    "pos.x /= aratio;pos += vec3(position, 0.0);gl_Position = vec4(pos, 1.0);}";

static const char *CIRCLE_FRAGMENT_SHADER =
    "#version 330 core\nout vec4 FragColor;in vec2 oResolution;in vec2 "
    "oCenter;in vec4 oFill;in vec4 oOutline;flat in float oRadius;flat in uint "
    "oOutlineThickness;void main(){FragColor = oFill, 0.0;}";

static const char *RECT_VERTEX_SHADER =
    "#version 330 core\nlayout (location = 0) in vec3 aPos;uniform vec2 "
    "position;uniform vec2 resolution;uniform float radius;uniform float "
    "aratio;uniform vec4 fill;uniform vec4 outline;uniform uint "
    "outline_thickness;out vec2 oResolution;out vec2 oCenter;out vec4 "
    "oFill;out vec4 oOutline;flat out float oRadius;flat out uint "
    "oOutlineThickness;void main(){oResolution = resolution;oFill = "
    "fill;oOutline = outline;oCenter = position;oOutlineThickness = "
    "outline_thickness;oRadius = radius;vec3 pos = aPos;pos *= radius;    "
    "pos.x /= aratio;pos += vec3(position, 0.0);gl_Position = vec4(pos, 1.0);}";

static const char *RECT_FRAGMENT_SHADER =
    "#version 330 core\nout vec4 FragColor;in vec2 oResolution;in vec2 "
    "oCenter;in vec4 oFill;in vec4 oOutline;flat in float oRadius;flat in uint "
    "oOutlineThickness;void main(){FragColor = oFill, 0.0;}";

static const char *CUBOID_VERTEX_SHADER =
    "#version 330 core\nlayout (location = 0) in vec3 aPos;uniform vec2 "
    "position;uniform vec2 resolution;uniform float radius;uniform float "
    "aratio;uniform vec4 fill;uniform vec4 outline;uniform uint "
    "outline_thickness;out vec2 oResolution;out vec2 oCenter;out vec4 "
    "oFill;out vec4 oOutline;flat out float oRadius;flat out uint "
    "oOutlineThickness;void main(){oResolution = resolution;oFill = "
    "fill;oOutline = outline;oCenter = position;oOutlineThickness = "
    "outline_thickness;oRadius = radius;vec3 pos = aPos;pos *= radius;    "
    "pos.x /= aratio;pos += vec3(position, 0.0);gl_Position = vec4(pos, 1.0);}";

static const char *CUBOID_FRAGMENT_SHADER =
    "#version 330 core\nout vec4 FragColor;in vec2 oResolution;in vec2 "
    "oCenter;in vec4 oFill;in vec4 oOutline;flat in float oRadius;flat in uint "
    "oOutlineThickness;void main(){FragColor = oFill, 0.0;}";

NS_END