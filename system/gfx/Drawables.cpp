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

#include "Drawables.hpp"

#include "define.hpp"

#include "Math.hpp"
#include "Renderer.dpp"

#include <GL/glew.h>

NS_BEGIN

Transform2D::operator Mat3f() const
{
    return MakeTranslationMatrix(translate) *
        MakeRotationMatrix<float>(rotate, rotationCenter) *
        MakeScaleMatrix(scale) * MakeShearMatrix(shear);
}

void detail::DrawGeneric2DShape(const Renderer &renderer, const Shader &shader,
                                const Mesh &mesh, const Mat3f &shape,
                                const VisualAppearance &appearance,
                                const Mat3f &transform, RenderMode mode)
{
    shader.use();
    shader.set("fill", appearance.fillColor);
    shader.set("outline", appearance.outlineColor);
    shader.set("outline_width", appearance.outlineWidth);
    shader.set("blendMode", (int) appearance.blendMode);
    shader.set("z", appearance.zIndex);

    shader.set("transform", transform);
    shader.set("camera", renderer.camera.getMatrix());
    CheckGLh("Set shader matrices");

    shader.set("shape", shape);
    CheckGLh("Set shader shape matrix");

    if (appearance.texture) {
        appearance.texture->use(0);
        shader.set("useTexture", true);
    }
    else {
        shader.set("useTexture", false);
    }
    CheckGLh("set texture");

    if (appearance.uvTransform) {
        shader.set("textureTransform", *appearance.uvTransform);
    }
    else if (appearance.texture) {
        shader.set("textureTransform", appearance.texture->getUVTransform());
    }
    else {
        shader.set("textureTransform", MAT3_NO_TRANSFORM<float>);
    }
    CheckGLh("set texture UV transform");

    glBindVertexArray(mesh.getVAO());
    glDrawElements(static_cast<unsigned int>(mode), mesh.getElementCount(),
                   GL_UNSIGNED_INT, nullptr);
    CheckGLh("draw");

    Shader::unbind();
    Texture::unbind(0);
    CheckGLh("Unbound");
}

// FIXME: Dirty hack to get a rectangle mesh for the DrawLineSegment function, should be rectified
//        once we move to render batching and using geometry shaders for creating geometry of primitives.
//        In the future, we would like to just throw the A and B points of the line segment into a VBO batch
//        and use the geometry shader to fill in the rest.
static bool wasInit = false;

static Mesh rectShape;

static Shader lineShader;

void detail::DrawLineSegment(const Renderer &renderer, const dline &seg,
                             const VisualAppearance &appearance, const Mat3f &transform)
{
    if (!wasInit) {
        lineShader.load("line.shader");
        rectShape.setAttributeDescriptors({
                                              AttributeType::Vec2 // position
                                          });
        rectShape.insertVertices({{1.f, 1.f},
                                  {1.f, -1.f},
                                  {-1.f, -1.f},
                                  {-1.f, 1.f}});
        rectShape.insertIndices({0, 1, 2, 0, 3, 2});
        rectShape.upload();

        wasInit = true;
    }

    lineShader.use();
    CheckGLh("Set shader");

    lineShader.set("resolution", (fvec2d) renderer.getSize());
    lineShader.set("transform", transform);
    lineShader.set("camera", renderer.camera.getMatrix());
    CheckGLh("Set shader matrices");

    auto right = (float) Max(seg.A.x, seg.B.x);
    auto left = (float) Min(seg.A.x, seg.B.x);
    auto top = (float) Max(seg.A.y, seg.B.y);
    auto bottom = (float) Min(seg.A.y, seg.B.y);

    frect rect = {
        {
            left - right,
            top - bottom
        },
        {
            (left + right) / 2.0f,
            (top + bottom) / 2.0f,
        }
    };

    Mat3f shape = MakeScaleMatrix<float>(rect.size) *
        MakeTranslationMatrix<float>(rect.position);

    lineShader.set("z", appearance.zIndex);

    lineShader.set("shape", shape);
    CheckGLh("Set shader matrix");

    lineShader.set("thickness", appearance.outlineWidth);
    lineShader.set("fill", appearance.fillColor);
    lineShader.set("A", seg.A);
    lineShader.set("B", seg.B);
    CheckGLh("Set shader params");

    glBindVertexArray(rectShape.getVAO());
    glDrawElements(static_cast<unsigned int>(GL_TRIANGLES), rectShape.getElementCount(), GL_UNSIGNED_INT, nullptr);
    CheckGLh("draw");

    Shader::unbind();
    Texture::unbind(0);
    CheckGLh("Unbound");
}

NS_END
