//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#pragma once

#include "define.hpp"

#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "RenderTask.hpp"
#include "Video.hpp"
#include "Circle.hpp"
#include "Line.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"

#include <functional>

NS_BEGIN

namespace video
{

class LambdaRender;

}

template<>
void Draw(video::LambdaRender&, color clearColor);

template<>
void Draw(video::LambdaRender& renderer, const std::function<void()> &, const std::string &, bool *, int);

template<>
void Draw(video::LambdaRender& renderer, const video::Mesh &, const video::Shader &,
          const video::Shader::Uniforms &, const video::Shader::Textures &,
		  const video::Shader::TransformMatrixUniform &);

template<>
void Draw(video::LambdaRender& renderer, const fline &, const video::VisualAppearance&, const Mat3f&);

template<>
void Draw(video::LambdaRender& renderer, const frect &, const video::VisualAppearance&, const Mat3f&);

template<>
void Draw(video::LambdaRender& renderer, const fcircle &, const video::VisualAppearance&, const Mat3f&);

template<>
void Draw(video::LambdaRender& renderer, const fvec2d &, float size, const video::VisualAppearance&, const Mat3f&);

using ClearScreen = video::RenderTask<color>;
using ImGuiWindow = video::RenderTask<const std::function<void()> &, const std::string &, bool *, int>;
using DrawMesh    = video::RenderTask<const video::Mesh &,
									  const video::Shader &,
									  const video::Shader::Uniforms &,
									  const video::Shader::Textures &,
									  const video::Shader::TransformMatrixUniform &>;
using DrawLine = video::RenderTask<const fline&, const video::VisualAppearance&, const Mat3f&>;
using DrawRect = video::RenderTask<const frect&, const video::VisualAppearance&, const Mat3f&>;
using DrawCircle = video::RenderTask<const fcircle&, const video::VisualAppearance&, const Mat3f&>;
using DrawPoint = video::RenderTask<const fvec2d&, const video::VisualAppearance&, const Mat3f&>;

NS_END
