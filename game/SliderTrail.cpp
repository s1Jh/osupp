#include "SliderTrail.hpp"

#include "GL.hpp"
#include "LambdaRender.hpp"
#include "StandardDrawCalls.hpp"

NS_BEGIN

template<>
void Draw(
    video::LambdaRender &renderer,
    const math::Curve<SliderPathT::iterator> &trail,
    const SliderTrailDrawInfo &info
)
{
    auto DrawSegment = [&](const fline &seg)
    {
        auto offset = info.thickness;

        auto right = (float) math::Max(seg.A.x, seg.B.x) + offset;
        auto left = (float) math::Min(seg.A.x, seg.B.x) - offset;
        auto top = (float) math::Max(seg.A.y, seg.B.y) + offset;
        auto bottom = (float) math::Min(seg.A.y, seg.B.y) - offset;

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

        Mat3f shape = math::MakeScaleMatrix<float>(rect.size) *
            math::MakeTranslationMatrix<float>(rect.position);

        auto tint = info.sprite->getTint();
        tint.a = info.alpha;

        DrawMesh::Call(
            renderer, renderer.getMeshes().rectMask, *info.shader,
            video::Shader::Uniforms{
                {"resolution", (fvec2d) renderer.getWindow().size()},
                {"A", seg.A},
                {"B", seg.B},
                {"thickness", offset},
                {"fill", tint},
                {"shape", shape}
            },
            video::Shader::Textures{
                {0, info.sprite->getTexture().get()}
            },
            &info.transform
        );

        // FIXME: Create some helper for calling draw functions on their own, this is ridiculous.
//        Draw<video::LambdaRender &, const video::Mesh &, const video::Shader &,
//             const video::Shader::Uniforms &, const video::Shader::Textures &,
//             const video::Shader::TransformMatrixUniform &>(
//            renderer, renderer.getMeshes().rectMask, *info.shader,
//            video::Shader::Uniforms{
//                {"resolution", (fvec2d) renderer.getWindow().size()},
//                {"A", seg.A},
//                {"B", seg.B},
//                {"thickness", offset},
//                {"fill", tint},
//                {"shape", shape}
//            },
//            video::Shader::Textures{
//                {0, info.sprite->getTexture().get()}
//            },
//            &info.transform
//        );
    };

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    if (info.useTexture) {
        // TODO: use the pre-baked texture ... once we have them
        for (auto it = trail.getBegin(); it != trail.getEnd(); it++) {
            if (it != std::prev(trail.getEnd())) {
                DrawSegment({it->position, std::next(it)->position});
            }
        }
    } else {
        // slowly extend
        bool draw = false;
        bool over = false;
        float currentProgress = 0.0f;
        auto prev = trail.getBegin();
        for (auto it = std::next(trail.getBegin()); (it != trail.getEnd()) && !over; it++) {
            float distance = math::Distance(prev->position, it->position);
            float contrib = distance / trail.getLength();

            fvec2d start = prev->position;
            fvec2d end = it->position;

            if ((currentProgress + contrib >= info.start) && !draw) {
                float t = (info.start - currentProgress) / contrib;
                start = math::BiLerp(start, end, t);

                draw = true;
            }
            if (currentProgress + contrib >= info.end) {
                float t = (info.end - currentProgress) / contrib;
                end = math::BiLerp(start, end, t);

                over = true;
            }

            if (draw) {
                DrawSegment({start, end});
            }

            currentProgress += contrib;

            prev = it;
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_ALWAYS);
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
}

NS_END
