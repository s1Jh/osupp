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

#include <string>

#include "Color.hpp"
#include "Matrix.hpp"
#include "Resource.hpp"
#include "Texture.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "define.hpp"

#include <string>
#include <unordered_map>
#include <variant>

NS_BEGIN

class Shader: public detail::Resource
{
public:
    Shader() noexcept;

    bool fromString(
        const std::string &vert_src,
        const std::string &frag_src,
        const std::string &geom_src = ""
    );

    bool load(const std::filesystem::path &path) override;

    bool create() override;

    // Calls OpenGL's glUseProgram with it's id.
    void use() const;

    // Unbinds currently active program.
    static void unbind();

    // Returns it's ID.
    [[nodiscard]] unsigned int getID() const;

#define SHADER_SETTER_METHODS                                                  \
  GENERATE_SHADER_SET_FUNC(int, 1i, int)                                       \
  GENERATE_SHADER_SET_FUNC(float, 1f, float)                                   \
  GENERATE_SHADER_SET_FUNC(unsigned int, 1ui, unsigned int)                    \
  GENERATE_SHADER_SET_FUNC(fvec2d, 2f, float)                                  \
  GENERATE_SHADER_SET_FUNC(ivec2d, 2i, int)                                    \
  GENERATE_SHADER_SET_FUNC(uvec2d, 2ui, unsigned int)                          \
  GENERATE_SHADER_SET_FUNC(fvec3d, 3f, float)                                  \
  GENERATE_SHADER_SET_FUNC(ivec3d, 3i, int)                                    \
  GENERATE_SHADER_SET_FUNC(uvec3d, 3ui, unsigned int)                          \
  GENERATE_SHADER_SET_FUNC(fvec4d, 4f, float)                                  \
  GENERATE_SHADER_SET_FUNC(ivec4d, 4i, int)                                    \
  GENERATE_SHADER_SET_FUNC(uvec4d, 4ui, unsigned int)                          \
  GENERATE_SHADER_SET_FUNC(color, 4f, float)

#define SHADER_MATRIX_SETTER_METHODS                                           \
  GENERATE_SHADER_SET_FUNC(Mat2f, Matrix2f, float)                             \
  GENERATE_SHADER_SET_FUNC(Mat3f, Matrix3f, float)                             \
  GENERATE_SHADER_SET_FUNC(Mat4f, Matrix4f, float)                             \
  GENERATE_SHADER_SET_FUNC(Mat2x3f, Matrix2x3f, float)                         \
  GENERATE_SHADER_SET_FUNC(Mat3x2f, Matrix3x2f, float)                         \
  GENERATE_SHADER_SET_FUNC(Mat2x4f, Matrix2x4f, float)                         \
  GENERATE_SHADER_SET_FUNC(Mat4x2f, Matrix4x2f, float)                         \
  GENERATE_SHADER_SET_FUNC(Mat3x4f, Matrix3x4f, float)                         \
  GENERATE_SHADER_SET_FUNC(Mat4x3f, Matrix4x3f, float)

#define SHADER_INTER_CAST_SETTER_METHODS                                       \
  GENERATE_SHADER_SET_FUNC(double, 1f, float, float)                           \
  GENERATE_SHADER_SET_FUNC(dvec2d, 2f, float, fvec2d)                          \
  GENERATE_SHADER_SET_FUNC(dvec3d, 3f, float, fvec3d)                          \
  GENERATE_SHADER_SET_FUNC(dvec4d, 4f, float, dvec4d)

#define GENERATE_SHADER_SET_FUNC(TYPE, ...)                                    \
  bool set(const std::string &name, const TYPE &value) const noexcept;

    SHADER_SETTER_METHODS

    SHADER_MATRIX_SETTER_METHODS

    SHADER_INTER_CAST_SETTER_METHODS

#undef GENERATE_SHADER_SET_FUNC

#ifndef INCLUDE_SHADER_METHOD_DEFINITIONS
#undef SHADER_SETTER_METHODS
#undef SHADER_MATRIX_SETTER_METHODS
#undef SHADER_INTER_CAST_SETTER_METHODS
#endif

    using Textures = std::unordered_map<int, Texture *>;
    using Uniforms = std::unordered_map<
        std::string,
        std::variant<int, float, unsigned int, double, fvec2d, ivec2d, uvec2d,
                     dvec2d, fvec3d, ivec3d, uvec3d, dvec3d, fvec4d, ivec4d,
                     uvec4d, dvec4d, fvec2d *, ivec2d *, uvec2d *, dvec2d *,
                     fvec3d *, ivec3d *, uvec3d *, dvec3d *, fvec4d *, ivec4d *,
                     uvec4d *, dvec4d *, Mat2f *, Mat3f *, Mat4f *, color *,
                     Texture *>>;

private:
    static unsigned int compileShader(const std::string &file, unsigned int type);

    unsigned int id;
};

using ShaderP = std::shared_ptr<Shader>;

NS_END