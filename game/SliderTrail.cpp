#include "SliderTrail.hpp"

#include "GL.hpp"
#include "LambdaRender.hpp"
#include "StandardDrawCalls.hpp"

NS_BEGIN

constexpr const char *SLIDER_TRAIL_VERTEX_SHADER =
    "#version 330 core\n"
    "layout (location=0) in vec2 aPos;"

    "uniform mat3 camera;"
    "uniform mat3 transform;"
    "uniform mat3 shape;"

    "void main()"
    "{"
    "gl_Position = vec4(camera * transform * shape * vec3(aPos, 1.0f), 1.f);"
    "}";

// FRAGMENT SHADER
constexpr const char *SLIDER_TRAIL_FRAGMENT_SHADER =
    "#version 330 core\n"

    "layout(pixel_center_integer) in vec4 gl_FragCoord;"

    "uniform sampler2D shadeTexture;"

    "uniform vec2 resolution;"
    "uniform vec2 A;"
    "uniform vec2 B;"
    "uniform float thickness;"
    "uniform vec4 fill;"

    "uniform mat3 camera;"
    "uniform mat3 transform;"

    "out vec4 FragColor;"

    "vec2 normal(vec2 inputVector) {"
        "return vec2(inputVector.y, -inputVector.x);"
    "}"

    "float aspectRatio = resolution.x / resolution.y;"

    "float findDistance(vec2 p1, vec2 p2, float thickness) {"

    "vec2 uv = gl_FragCoord.xy / resolution.xy;"
    "uv *= 2.0f;"
    "uv -= 1.0f;"
    "uv.x *= aspectRatio;"

    "vec2 normalVector = normal(p1 - p2);"
    "vec2 intersectNormal = p1 - p2;"

    "float a1 = normalVector.x;"
    "float b1 = normalVector.y;"
    "float c1 = -(a1*p1.x + b1 * p1.y);"

    "float a2 = intersectNormal.x;"
    "float b2 = intersectNormal.y;"
    "float c2 = -(a2*uv.x + b2 * uv.y);"

    "vec2 intersect;"

    "intersect.x = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);"
    "intersect.y = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);"

    "const float margin = 0.000001f;"

    "float smallerX = min(p1.x, p2.x) - margin;"
    "float smallerY = min(p1.y, p2.y) - margin;"
    "float biggerX = max(p1.x, p2.x) + margin;"
    "float biggerY = max(p1.y, p2.y) + margin;"

    "float factor = 1.0f;"

    "if (((smallerX <= intersect.x) && (intersect.x <= biggerX)) &&"
        "((smallerY <= intersect.y) && (intersect.y <= biggerY))) {"
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

    "if (factor > thickness)"
        "return -1.0f;"
    "return (factor / thickness);"
    "}"

    "void main()"
    "{"
    "vec2 tA = vec3(transform * vec3(A, 1.0f)).xy;"
    "vec2 tB = vec3(transform * vec3(B, 1.0f)).xy;"
    "float factor = findDistance(tA, tB, thickness);"
    "if (factor < 0.0f) {"
        "discard;"
    "}"
    "gl_FragDepth = factor;"
    "gl_FragColor = texture(shadeTexture, vec2(factor, 0.5f));"
    "}";

void DrawSliderSegment(
    video::LambdaRender &renderer, const fline &seg, const SliderTrailDrawInfo &info
)
{
    CheckGLh("DrawSliderSegment");
    static bool Init = false;
    static video::Shader sliderShader;

    if (!Init) {
        sliderShader.fromString(SLIDER_TRAIL_VERTEX_SHADER, SLIDER_TRAIL_FRAGMENT_SHADER);
        if (!sliderShader.upload()) {
            log::error("Failed to upload slider shader!");
        }
        Init = true;
        CheckGLh("DrawSliderSegment: Init");
    }

    float thickness = info.thickness;

    auto right = (float) math::Max(seg.A.x, seg.B.x) + thickness;
    auto left = (float) math::Min(seg.A.x, seg.B.x) - thickness;
    auto top = (float) math::Max(seg.A.y, seg.B.y) + thickness;
    auto bottom = (float) math::Min(seg.A.y, seg.B.y) - thickness;

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

    auto tint = info.tint;
    tint.a = 1.0f;

    DrawMesh::Call(
        renderer, renderer.getMeshes().rectMask, sliderShader,
        video::Shader::Uniforms{
            {"resolution", (fvec2d) renderer.getWindow().size()},
            {"A", seg.A},
            {"B", seg.B},
            {"thickness", thickness},
            {"shape", shape}
        },
        video::Shader::Textures{
            {0, info.trailTexture}
        },
        &MAT3_NO_TRANSFORM<float>
    );
}

video::Texture DrawTrailToTexture(
    video::LambdaRender &renderer,
    math::Curve<SliderPathT::iterator> &trail,
    SliderTrailDrawInfo info
)
{
    CheckGLh("DrawTrailToTexture");
    static unsigned int frame = 0, color = 0, depth = 0;
    auto size = renderer.getWindow().size();
    glGenFramebuffers(1, &frame);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    CheckGLh("DrawTrailToTexture: Gen/Bind frame");

    glGenTextures(1, &color);
    glBindTexture(GL_TEXTURE_2D, color);
    CheckGLh("DrawTrailToTexture: Gen/Bind color");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.w, size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CheckGLh("DrawTrailToTexture: Color params");

    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.w, size.h);
    CheckGLh("DrawTrailToTexture: Depth Gen/Conf");

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color, 0);
    CheckGLh("DrawTrailToTexture: Attach to FB");

    unsigned int buffers = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, &buffers);
    CheckGLh("DrawTrailToTexture: Draw buffers");

    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    CheckGLh("DrawTrailToTexture: Bind");
    glViewport(0, 0, size.w, size.h);

    glDepthFunc(GL_LEQUAL);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    CheckGLh("DrawTrailToTexture: CFG+Clean");
    for (auto it = trail.getBegin(); it != std::prev(trail.getEnd()); it++) {
        auto next = std::next(it);
        DrawSliderSegment(renderer, {it->position, next->position}, info);
    }

    CheckGLh("DrawTrailToTexture: Post Draw");

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // restore framebuffer
    CheckGLh("DrawTrailToTexture: Unbind FB");
    glViewport(0, 0, size.w, size.h);     // restore size
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_ALWAYS);
    CheckGLh("DrawTrailToTexture: Restore");

    video::Texture t(color);
    CheckGLh("DrawTrailToTexture: Create Texture");
    glDeleteRenderbuffers(1, &depth);
    glDeleteFramebuffers(1, &frame);
    CheckGLh("DrawTrailToTexture: Delete Buffers");

    return t;
}

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
        glDeleteTextures(1, &color);
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

    if (info.useTexture) {
        DrawRect::Call(
            renderer, UNIT_RECT<float>,
            video::VisualAppearance{
                .texture = info.bakedTexture,
                .fillColor = info.tint,
                .flags = video::AppearanceFlags::IGNORE_CAMERA
            },
            info.transform
        );
    } else {
        // slowly extend

        // Render the whole slider to a texture first, then render the whole texture
        // onto the screen to ensure proper layering.
        glBindFramebuffer(GL_FRAMEBUFFER, frame);
        CheckGLh("glBindFramebuffer");
        glViewport(0, 0, size.w, size.h);

        glDepthFunc(GL_LEQUAL);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
        glClearDepth(1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
                DrawSliderSegment(renderer, {start, end}, info);
            }

            currentProgress += contrib;

            prev = it;
        }

        CheckGLh("post draw");

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // restore framebuffer
        CheckGLh("glBindFramebuffer 0");
        glViewport(0, 0, size.w, size.h);     // restore size
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_ALWAYS);

        video::Texture tex(color);

        DrawRect::Call(
            renderer, UNIT_RECT<float>,
            video::VisualAppearance{
                .texture = &tex,
                .fillColor = info.tint,
                .flags = video::AppearanceFlags::IGNORE_CAMERA
            },
            info.transform
        );
    }
}

NS_END
