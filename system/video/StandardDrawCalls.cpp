//=*==*==*==*==*==*=ImGuiContext*=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
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
#include "StandardDrawCalls.hpp"

#include "define.hpp"

#include "Color.hpp"
#include "GL.hpp"
#include "imgui.h"
#include "Util.hpp"

#include <functional>

NS_BEGIN

template<>
void Draw(color clearColor)
{
	glClearColor(DECOMPOSE_COLOR_RGBA(clearColor));
	glClear(GL_COLOR_BUFFER_BIT);
}

template<>
void Draw(const std::function<void()> &func, const std::string &title, bool *open, int flags)
{
	if (ImGui::Begin(title.c_str(), open, flags)) {
		func();
		ImGui::End();
	}
}

template<>
void Draw(const video::Mesh &mesh, const video::Shader &shader,
		  const video::Shader::Uniforms &uniforms, const video::Shader::Textures &textures,
		  const video::Shader::TransformMatrixUniform &transform)
{
	if (!mesh.uploaded()) {
        return;
	}

	if (!shader.uploaded()) {
        return;
	}

	shader.use();
	CheckGLh("Bound shader");

#define MATCHF(src, type)                                                           \
  if constexpr (std::is_same_v<T, type>)                                       \
    shader.set(src, arg);
#define MATCH(src, type)                                                            \
  else if constexpr (std::is_same_v<T, type>) shader.set(src, arg);
#define MATCHFP(src, type)                                                           \
  if constexpr (std::is_same_v<T, type>) shader.set(src, *arg);
#define MATCHP(src, type)                                                           \
  else if constexpr (std::is_same_v<T, type>) shader.set(src, *arg);
#define END_MATCH(src)                                                            \
  else log::warning("Unable to deduce type for shader uniform ", src);

	for (auto &uniform : uniforms) {
		std::visit(
			[&](auto &&arg)
			{
				using T = std::decay_t<decltype(arg)>;
				MATCHF(uniform.first, float)
				MATCH(uniform.first, int)MATCH(uniform.first, unsigned int) MATCH(uniform.first, double)
				MATCH(uniform.first, fvec2d) MATCH(uniform.first, ivec2d)MATCH(uniform.first, uvec2d)
				MATCH(uniform.first, dvec2d) MATCH(uniform.first, fvec3d) MATCH(uniform.first, ivec3d)
				MATCH(uniform.first, uvec3d) MATCH(uniform.first, dvec3d) MATCH(uniform.first, fvec4d)
				MATCH(uniform.first, ivec4d) MATCH(uniform.first, uvec4d) MATCH(uniform.first, dvec4d)
                MATCH(uniform.first, Mat2f) MATCH(uniform.first, Mat3f) MATCH(uniform.first, Mat4f)
				MATCH(uniform.first, color) MATCHP(uniform.first, fvec2d *)
                MATCHP(uniform.first, ivec2d *) MATCHP(uniform.first, uvec2d *)
                MATCHP(uniform.first, dvec2d *) MATCHP(uniform.first, fvec3d *)
				MATCHP(uniform.first, ivec3d *) MATCHP(uniform.first, uvec3d *)
                MATCHP(uniform.first, dvec3d *) MATCHP(uniform.first, fvec4d *)
                MATCHP(uniform.first, ivec4d *) MATCHP(uniform.first, uvec4d *)
				MATCHP(uniform.first, dvec4d *) MATCHP(uniform.first, Mat2f *)
                MATCHP(uniform.first, Mat3f *) MATCHP(uniform.first, Mat4f *)
                MATCHP(uniform.first, color *) END_MATCH(uniform.first)
			},
			uniform.second);
		CheckGLh("Set shader uniform " + uniform.first);
	}

	std::visit([&](auto&& arg) {
		using T = std::decay_t<decltype(arg)>;
		MATCHFP("transform", const Mat2f *) MATCHP("transform", const Mat3f *) MATCHP("transform", const Mat4f *) END_MATCH("transform")
	}, transform);

	// TODO: Camera
	shader.set("camera", MAT3_NO_TRANSFORM<float>);

	for (auto &texture : textures) {
		if (!texture.second->uploaded()) {
			if (!const_cast<video::Texture*>(texture.second)->upload()) {
				continue;
			}
		}
		texture.second->use(texture.first);
	}
	CheckGLh("Bound textures");

	glBindVertexArray(mesh.getGLData().VAO);
    CheckGLh("Bound VAO");
	glDrawElements(static_cast<unsigned int>(mesh.getRenderMode()),
				   mesh.getElementCount(), GL_UNSIGNED_INT, nullptr);
	CheckGLh("Draw");

	video::Shader::unbind();
	for (auto &texture : textures) {
		video::Texture::unbind(texture.first);
	}
	CheckGLh("Unbound");

#undef MATCHF
#undef MATCH
#undef MATCHP
#undef END_MATCH
}

template<>
void osupp::Draw(const fline &, const video::VisualAppearance &, const Mat3f &)
{

}

template<>
void osupp::Draw(const frect &, const video::VisualAppearance &, const Mat3f &)
{

}

template<>
void osupp::Draw(const fcircle &, const video::VisualAppearance &, const Mat3f &)
{

}

template<>
void osupp::Draw(const fvec2d &, const video::VisualAppearance &, const Mat3f &)
{

}

NS_END