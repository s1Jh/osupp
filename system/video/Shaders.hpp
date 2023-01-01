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

namespace video
{

//constexpr const char *CIRCLE_VERTEX_SHADER =
//    "#version 330 core\nlayout (location = 0) in vec3 aPos;uniform vec2 "
//    "position;uniform vec2 resolution;uniform float radius;uniform float "
//    "aratio;uniform vec4 fill;uniform vec4 outline;uniform uint "
//    "outline_thickness;out vec2 oResolution;out vec2 oCenter;out vec4 "
//    "oFill;out vec4 oOutline;flat out float oRadius;flat out uint "
//    "oOutlineThickness;void main(){oResolution = resolution;oFill = "
//    "fill;oOutline = outline;oCenter = position;oOutlineThickness = "
//    "outline_thickness;oRadius = radius;vec3 pos = aPos;pos *= radius;    "
//    "pos.x /= aratio;pos += vec3(position, 0.0);gl_Position = vec4(pos, 1.0);}";
//
//constexpr const char *CIRCLE_FRAGMENT_SHADER =
//    "#version 330 core\nout vec4 FragColor;in vec2 oResolution;in vec2 "
//    "oCenter;in vec4 oFill;in vec4 oOutline;flat in float oRadius;flat in uint "
//    "oOutlineThickness;void main(){FragColor = oFill, 0.0;}";
//
//constexpr const char *RECT_VERTEX_SHADER =
//    "#version 330 core\nlayout (location = 0) in vec3 aPos;uniform vec2 "
//    "position;uniform vec2 resolution;uniform float radius;uniform float "
//    "aratio;uniform vec4 fill;uniform vec4 outline;uniform uint "
//    "outline_thickness;out vec2 oResolution;out vec2 oCenter;out vec4 "
//    "oFill;out vec4 oOutline;flat out float oRadius;flat out uint "
//    "oOutlineThickness;void main(){oResolution = resolution;oFill = "
//    "fill;oOutline = outline;oCenter = position;oOutlineThickness = "
//    "outline_thickness;oRadius = radius;vec3 pos = aPos;pos *= radius;    "
//    "pos.x /= aratio;pos += vec3(position, 0.0);gl_Position = vec4(pos, 1.0);}";
//
//constexpr const char *RECT_FRAGMENT_SHADER =
//    "#version 330 core\nout vec4 FragColor;in vec2 oResolution;in vec2 "
//    "oCenter;in vec4 oFill;in vec4 oOutline;flat in float oRadius;flat in uint "
//    "oOutlineThickness;void main(){FragColor = oFill, 0.0;}";
//
//constexpr const char *CUBOID_VERTEX_SHADER =
//    "#version 330 core\nlayout (location = 0) in vec3 aPos;uniform vec2 "
//    "position;uniform vec2 resolution;uniform float radius;uniform float "
//    "aratio;uniform vec4 fill;uniform vec4 outline;uniform uint "
//    "outline_thickness;out vec2 oResolution;out vec2 oCenter;out vec4 "
//    "oFill;out vec4 oOutline;flat out float oRadius;flat out uint "
//    "oOutlineThickness;void main(){oResolution = resolution;oFill = "
//    "fill;oOutline = outline;oCenter = position;oOutlineThickness = "
//    "outline_thickness;oRadius = radius;vec3 pos = aPos;pos *= radius;    "
//    "pos.x /= aratio;pos += vec3(position, 0.0);gl_Position = vec4(pos, 1.0);}";
//
//constexpr const char *CUBOID_FRAGMENT_SHADER =
//    "#version 330 core\nout vec4 FragColor;in vec2 oResolution;in vec2 "
//    "oCenter;in vec4 oFill;in vec4 oOutline;flat in float oRadius;flat in uint "
//    "oOutlineThickness;void main(){FragColor = oFill, 0.0;}";

constexpr const char *STATIC_SHAPE_SHADER_VERT =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;"
    "layout (location = 1) in vec2 aUV;"
    ""
    "uniform mat3 transform;"
    "uniform mat3 shape;"
    "uniform mat3 camera;"
    "uniform float z;"
    ""
    "out vec2 UV;"
    ""
    "void main()"
    "{"
    "UV = aUV;"
    "gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);\n"
    "}";

constexpr const char *STATIC_SHAPE_SHADER_FRAG =
    "#version 330 core\n"
    ""
    "uniform vec4 fill;"
    "uniform bool useTexture;"
    "uniform int blendMode;"
    "uniform mat3 textureTransform;"
    "uniform sampler2D fillTexture;"
    ""
    "in vec2 UV;"
    ""
    "out vec4 FragColor;"
    ""
    "void main()"
    "{"
    "if (!useTexture)"
    "{"
    "FragColor = fill;"
    "return;"
    "}"
    ""
    "FragColor = texture(fillTexture, (textureTransform * vec3(UV, 1.0)).xy);\n"
    ""
    "switch (blendMode)"
    "{"
    "case 0: break;"
    "case 1: FragColor *= fill; break;"
    "case 2: FragColor += fill; break;"
    "case 3: FragColor -= fill; break;"
    "}"
    "}";

constexpr const char *LINE_SHADER_VERT =
    "#version 330 core\n"
    ""
    "layout (location=0) in vec2 aPos;"
    ""
    "uniform mat3 camera;"
    "uniform mat3 transform;"
    "uniform mat3 shape;"
    ""
    "uniform float z;"
    ""
    "void main()"
    "{"
    "gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);"
    "}";

constexpr const char *LINE_SHADER_FRAG =
    "#version 330 core\n"
    ""
    "layout(pixel_center_integer) in vec4 gl_FragCoord;"
    ""
    "uniform vec2 resolution;"
    "uniform vec2 A;"
    "uniform vec2 B;"
    "uniform mat3 camera;"
    "uniform mat3 transform;"
    "uniform float thickness;"
    "uniform vec4 fill;"
    ""
    "out vec4 FragColor;"
    ""
    "vec2 normal(vec2 inputVector) {"
    "return vec2(inputVector.y, -inputVector.x);"
    "}"
    ""
    "float aspectRatio = resolution.x / resolution.y;"
    ""
    "float drawLine(vec2 p1, vec2 p2, float thickness) {"
    ""	// our fragment positioned in normalized space
    "vec2 uv = gl_FragCoord.xy / resolution.xy;"
    "uv *= 2.0f;"
    "uv -= 1.0f;"
    "uv.x *= aspectRatio;"
    ""
    ""   // we need the normal vector to calculate the distance of this point to the drawn line
    "vec2 normalVector = normal(p1 - p2);"
    ""   // the intersection line is normal to the original drawn line
    "vec2 intersectNormal = p1 - p2;"
    ""
    ""   // drawn line
    "float a1 = normalVector.x;"
    "float b1 = normalVector.y;"
    "float c1 = -(a1*p1.x + b1 * p1.y);"
    ""
    "float a2 = intersectNormal.x;"
    "float b2 = intersectNormal.y;"
    "float c2 = -(a2*uv.x + b2 * uv.y);"
    ""
    "vec2 intersect;"
    ""
    "intersect.x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);"
    "intersect.y = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);"
    ""
    "const float margin = 0.000001f;"
    ""
    "float smallerX = min(p1.x, p2.x) - margin;"
    "float smallerY = min(p1.y, p2.y) - margin;"
    "float biggerX = max(p1.x, p2.x) + margin;"
    "float biggerY = max(p1.y, p2.y) + margin;"
    ""
    "float factor = 1.0f;"
    ""
    "if (((smallerX <= intersect.x) && (intersect.x <= biggerX)) &&"
    "((smallerY <= intersect.y) && (intersect.y <= biggerY))) {"
    ""   	// IN RANGE
    "factor = abs(a1 * uv.x + b1 * uv.y + c1) / sqrt(pow(a1, 2) + pow(b1, 2));"
    "} else {"
    "if (intersect.x < smallerX) {"
    "if (intersect.y < smallerY) {"
    "factor = distance(uv, vec2(smallerX, smallerY));"
    "} else {"
    "factor = distance(uv, vec2(smallerX, biggerY));"
    "}"
    "} else {"
    "if (intersect.y < smallerY) {"
    "factor = distance(uv, vec2(biggerX, smallerY));"
    "} else {"
    "factor = distance(uv, vec2(biggerX, biggerY));"
    "}"
    "}"
    "}"
    ""
    "if (factor > thickness)"
    "return -1.0f;"
    "return factor;"
    "}"
    ""
    "void main()"
    "{"
    "vec2 tA = vec3(transform * vec3(A, 1.0f)).xy;"
    "vec2 tB = vec3(transform * vec3(B, 1.0f)).xy;"
    "float factor = drawLine(tA, tB, thickness);"
    "if (factor < 0.0f)"
    "discard;"
    "gl_FragColor = fill * mix(1.0, 0.0, smoothstep(0.5 * thickness, 1.5 * thickness, factor));"
    "}";

constexpr const char *DEFAULT_FRAGMENT_SHADER =
    "out vec4 fragColor;"
    "void main()"
    "{"
    "    fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);"
    "}";

constexpr const char *DEFAULT_VERTEX_SHADER =
    "layout(location = 0) in vec3 vPos;"
    "void main()"
    "{"
    "    gl_Position = vec4(vPos, 1.0f);"
    "}";

}

NS_END