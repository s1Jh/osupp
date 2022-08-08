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
#include "Matrix.hpp"
#include "Resource.hpp"
#include "Renderer.dpp"
#include "Shader.hpp"

#include <memory>
#include <vector>
#include <filesystem>

NS_BEGIN

enum class AttributeType
{
    Scalar = 1, Vec2, Vec3, Vec4, Color = Vec4
};

// direct mapping to opengl render modes
enum class RenderMode
{
    Points,
    Lines,
    LineLoop,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
    Quads,
    QuadStrip,
    Polygon
};

class Mesh: public detail::Resource
{
public:
    Mesh() noexcept;

    bool load(const std::filesystem::path &path) override;

    bool create() override;

    using Vertex = std::vector<float>;

    [[nodiscard]] const Mat4<float> &getTransform() const;

    void setTransform(Mat4<float>);

    void clear() noexcept;

    void setAttributeDescriptors(std::vector<AttributeType> descriptors);

    void addAttributeDescriptor(AttributeType desc);

    unsigned int getAttributeCount();

    unsigned int insertVertex(const Vertex &v);

    unsigned int insertVertices(const std::vector<Vertex> &vs);

    void insertIndice(unsigned int i);

    void insertIndices(const std::vector<unsigned int> &is,
                       unsigned int offset = 0);

    unsigned int insertTriangle(const Vertex &v1, const Vertex &v2,
                                const Vertex &v3);

    [[nodiscard]] bool isValid() const;

    bool upload();

    void deleteMesh();

    [[nodiscard]] int getVertexCount() const;

    [[nodiscard]] int getElementCount() const;

    [[nodiscard]] unsigned int getVAO() const;

    [[nodiscard]] unsigned int getVBO() const;

    [[nodiscard]] unsigned int getEBO() const;

    [[nodiscard]] RenderMode getRenderMode() const;

    void setRenderMode(RenderMode renderMode);

    [[nodiscard]] unsigned int getAttributeDescriptorCount() const;

private:
    struct GLObjs
    {
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
    };

    static void GLObjDeleter(GLObjs *obj);

    Mat4<float> meshTransform;
    std::vector<AttributeType> dataDescriptors;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    RenderMode renderMode;

    int totalDataPerVertex = 0;
    int vertexCount = 0;
    int elementCount = 0;

    std::shared_ptr<GLObjs> data;
};

using MeshP = std::shared_ptr<Mesh>;

BEGIN_RENDER_FUNCTOR_DECL(Mesh, const Shader &shader,
                          const Shader::Uniforms &shaderUniforms = {},
                          const Shader::Textures &textures = {},
                          const Mat3f &transform = MAT3_NO_TRANSFORM<float>)
END_RENDER_FUNCTOR_DECL()

NS_END