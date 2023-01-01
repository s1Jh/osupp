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
    CheckGLh("start");
    static bool Init = false;
    static unsigned int frame = 0, color = 0, depth = 0;
    auto size = renderer.getWindow().size();
    static isize initSize = {0, 0};
    if (size != initSize) {
        log::debug("Recreating slider buffer texture");
        glDeleteTextures(1, &frame);
        glDeleteRenderbuffers(1, &depth);
        glDeleteFramebuffers(1, &frame);
        Init = false;
    }
    if (!Init) {
        initSize = size;
        glGenFramebuffers(1, &frame);
        glBindFramebuffer(GL_FRAMEBUFFER, frame);

        glGenTextures(1, &color);
        glBindTexture(GL_TEXTURE_2D, color);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glGenRenderbuffers(1, &depth);
        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.w, size.h);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);

        unsigned int buffers = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, &buffers);
        CheckGLh("glDrawBuffers");

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            return;
        }

        CheckGLh("created framebuffers");
        Init = true;
    }

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
        tint.a = 1.0f;

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
    };

    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    CheckGLh("glBindFramebuffer");
    glViewport(0, 0, size.w, size.h);

    glDepthFunc(GL_LEQUAL);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
    CheckGLh("post draw");

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // restore framebuffer
    CheckGLh("glBindFramebuffer 0");
    glViewport(0, 0, size.w, size.h);     // restore size
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_ALWAYS);

    video::Texture tex(color);
    auto fill = WHITE;
    fill.a = info.alpha;
    DrawRect::Call(
        renderer, UNIT_RECT < float >,
        video::VisualAppearance{
            .texture = &tex,
            .fillColor = fill,
            .flags = video::AppearanceFlags::IGNORE_CAMERA
        },
        MAT3_NO_TRANSFORM<float>
    );
}

NS_END
