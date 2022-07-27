#pragma once

#include "define.hpp"

#include "Drawables.hpp"
#include "Mesh.hpp"
#include "Rect.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include "Math.hpp"

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
            const auto& shader = getData().shader; \
            const auto& mesh = getData().mesh; \
            Mat3f shape = ShapeExpression;\
            DrawGeneric2DShape(renderer, shader, mesh, shape, appearance, transform, RenderType); \
        }


BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(rect)
{
    shader.load("static_shape.shader");
    // Create geometry for rectangular meshes
    mesh.setAttributeDescriptors({
                                     AttributeType::Vec2, // position
                                     AttributeType::Vec2  // uv
                                 });
    mesh.insertVertices({{1.f, 1.f, 1.f, 1.f},
                         {1.f, -1.f, 1.f, 0.f},
                         {-1.f, -1.f, 0.f, 0.f},
                         {-1.f, 1.f, 0.f, 1.f}});
    mesh.insertIndices({0, 1, 2, 0, 3, 2});
    mesh.upload();
}

BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(circle)
{
    shader.load("static_shape.shader");

    const unsigned int resolution = 32;

    double circle_rotation = 2.0 * PI / (resolution - 1);
    fvec2d circle_vec = {1.f, 0.f};

    mesh.setAttributeDescriptors({
                                     AttributeType::Vec2, // position
                                     AttributeType::Vec2  // uv
                                 });
    mesh.insertVertex({0.0, 0.0, 0.5f, 0.5f});

    for (unsigned int i = 0; i < resolution; i++) {
        fvec2d vec = {circle_vec.x, circle_vec.y};
        fvec2d uv = (vec + 1.0f) / 2.0f;
        mesh.insertVertex({vec.x, vec.y, uv.x, uv.y});
        mesh.insertIndice(i);
        circle_vec = Rotate(circle_vec, circle_rotation);
    }
    mesh.insertIndice(1);
    mesh.upload();
}

BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(line)
{
}

BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(vec2d)
{
}

IMPLEMENT_2D_PRIMITIVE_RENDER_FUNCTION(
    rect,
    MakeScaleMatrix<float>(object.size) * MakeTranslationMatrix<float>(object.position),
    RenderMode::Triangles
)

IMPLEMENT_2D_PRIMITIVE_RENDER_FUNCTION(
    circle,
    MakeScaleMatrix<float>(dvec2d{object.radius, object.radius}) * MakeTranslationMatrix<float>(object.position),
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