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

#include "Drawables.hpp"
#include "Mesh.hpp"
#include "Rect.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include "Math.hpp"
#include "Generics.hpp"

NS_BEGIN

#define MAKE_2D_PRIMITIVE_RENDER_FUNCTOR(Type, ...) \
BEGIN_TEMPLATED_RENDER_FUNCTOR_DECL(Type, __VA_ARGS__ __VA_OPT__(,) const VisualAppearance& = DEFAULT_APPEARANCE, \
        const Mat3f& = MAT3_NO_TRANSFORM<float>) \
        Shader shader; Mesh mesh; \
END_RENDER_FUNCTOR_DECL()


#define MAKE_1D_PRIMITIVE_RENDER_FUNCTOR(Type, ...) \
BEGIN_TEMPLATED_RENDER_FUNCTOR_DECL(Type, __VA_ARGS__ __VA_OPT__(,) const VisualAppearance& = DEFAULT_APPEARANCE, \
        const Mat3f& = MAT3_NO_TRANSFORM<float>) \
END_RENDER_FUNCTOR_DECL()

MAKE_2D_PRIMITIVE_RENDER_FUNCTOR(rect)
MAKE_2D_PRIMITIVE_RENDER_FUNCTOR(circle)

MAKE_1D_PRIMITIVE_RENDER_FUNCTOR(line)
MAKE_1D_PRIMITIVE_RENDER_FUNCTOR(vec2d, float)

#define IMPLEMENT_2D_PRIMITIVE_RENDER_FUNCTION(Type, ShapeExpression, RenderType) \
        template<typename T> \
        void detail::RenderFunctor<Type<T>>::operator()(Renderer &renderer, const Type<T> &object, \
                                                        const VisualAppearance &appearance, \
                                                        const Mat3f& transform) { \
            Mat3f shape = ShapeExpression;                                        \
            const auto& mesh = GetGenericMeshes().Type;                             \
            DrawGeneric2DShape(renderer, mesh, shape, appearance, transform, RenderType); \
        }


BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(rect) {}

BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(circle) {}

BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(line) {}

BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(vec2d) {}

IMPLEMENT_2D_PRIMITIVE_RENDER_FUNCTION(
    rect,
    math::MakeScaleMatrix<float>(object.size) * math::MakeTranslationMatrix<float>(object.position),
    RenderMode::Triangles
)

IMPLEMENT_2D_PRIMITIVE_RENDER_FUNCTION(
    circle,
    math::MakeScaleMatrix<float>(dvec2d{object.radius, object.radius}) * math::MakeTranslationMatrix<float>(object.position),
    RenderMode::TriangleFan
)

template<typename T>
void detail::RenderFunctor<line<T>>::operator()(Renderer &renderer,
                                                const line<T> &seg,
                                                const VisualAppearance &appearance,
                                                const Mat3f &transform)
{
    detail::DrawLineSegment(renderer, seg, appearance, transform);
}

template<typename T>
void detail::RenderFunctor<vec2d<T>>::operator()(Renderer &renderer, const vec2d<T> &pos, float size,
                                                 const VisualAppearance &appearance, const Mat3f &transform)
{
    size = size / 2.0f;
    dline a = {pos + fvec2d{size, size}, pos + fvec2d{-size, -size}};
    dline b = {pos + fvec2d{-size, size}, pos + fvec2d{size, -size}};
    detail::DrawLineSegment(renderer, a, appearance, transform);
    detail::DrawLineSegment(renderer, b, appearance, transform);
}

NS_END