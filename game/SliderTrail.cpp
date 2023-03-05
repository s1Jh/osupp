#include "SliderTrail.hpp"

#include "GL.hpp"
#include "Util.hpp"
#include "LambdaRender.hpp"
#include "StandardDrawCalls.hpp"

namespace PROJECT_NAMESPACE {

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
    "uniform float radius;"
    "uniform vec4 fill;"

    "uniform mat3 camera;"
    "uniform mat3 transform;"

    "out vec4 FragColor;"

    "vec2 normal(vec2 inputVector) {"
        "return vec2(inputVector[1], -inputVector[0]);"
    "}"

    "float aspectRatio = resolution[0] / resolution[1];"

    "float findDistance(vec2 p1, vec2 p2, float thickness) {"

    "vec2 uv = gl_FragCoord[0]y / resolution[0]y;"
    "uv *= 2.0f;"
    "uv -= 1.0f;"
    "uv[0] *= aspectRatio;"

    "vec2 normalVector = normal(p1 - p2);"
    "vec2 intersectNormal = p1 - p2;"

    "float a1 = normalVector[0];"
    "float b1 = normalVector[1];"
    "float c1 = -(a1*p1[0] + b1 * p1[1]);"

    "float a2 = intersectNormal[0];"
    "float b2 = intersectNormal[1];"
    "float c2 = -(a2*uv[0] + b2 * uv[1]);"

    "vec2 intersect;"

    "intersect[0] = (b1*c2 - b2*c1) / (a1*b2 - a2*b1);"
    "intersect[1] = (a2*c1 - a1*c2) / (a1*b2 - a2*b1);"

    "const float margin = 0.000001f;"

    "float smallerX = min(p1[0], p2[0]) - margin;"
    "float smallerY = min(p1[1], p2[1]) - margin;"
    "float biggerX = max(p1[0], p2[0]) + margin;"
    "float biggerY = max(p1[1], p2[1]) + margin;"

    "float factor = 1.0f;"

    "if (((smallerX <= intersect[0]) && (intersect[0] <= biggerX)) &&"
        "((smallerY <= intersect[1]) && (intersect[1] <= biggerY))) {"
        "factor = abs(a1 * uv[0] + b1 * uv[1] + c1) / sqrt(pow(a1, 2) + pow(b1, 2));"
    "} else {"
        "if (intersect[0] < smallerX) {"
            "if (intersect[1] < smallerY) {"
                "factor = distance(uv, vec2(smallerX, smallerY));"
            "} else {"
                "factor = distance(uv, vec2(smallerX, biggerY));"
            "}"
        "} else {"
            "if (intersect[1] < smallerY) {"
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
    "float thickness = radius / 2.f;"
    "vec2 tA = vec3(transform * vec3(A, 1.0f))[0]y;"
    "vec2 tB = vec3(transform * vec3(B, 1.0f))[0]y;"
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
            log::Error("Failed to upload slider shader!");
        }
        Init = true;
        CheckGLh("DrawSliderSegment: Init");
    }

    float thickness = info.thickness;

    auto right = (float) math::Max(seg.A[0], seg.B[0]) + thickness;
    auto left = (float) math::Min(seg.A[0], seg.B[0]) - thickness;
    auto top = (float) math::Max(seg.A[1], seg.B[1]) + thickness;
    auto bottom = (float) math::Min(seg.A[1], seg.B[1]) - thickness;

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

    DrawMesh::Call(
        renderer, renderer.getMeshes().screenRectMask, sliderShader,
        video::Shader::Uniforms{
            {"resolution", (fvec2d) renderer.getSize()},
            {"A", seg.A},
            {"B", seg.B},
            {"radius", thickness},
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
    unsigned int frame = 0, color = 0, depth = 0;
    auto size = renderer.getSize();
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
    auto size = renderer.getSize();
    static isize initSize = {0, 0};
    if (size != initSize) {
        log::Debug("Recreating slider buffer texture");
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

    auto rect = SCREEN_RECT<float>;
    rect.size.w /= renderer.camera.getAspectRatio();

    if (info.useTexture) {
        DrawRect::Call(
            renderer, rect,
            video::VisualAppearance{
                .texture = info.bakedTexture,
                .fillColor = info.tint
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

        Resource<video::Texture> tex(color);

        DrawRect::Call(
            renderer, rect,
            video::VisualAppearance{
                .texture = tex,
                .fillColor = info.tint
            },
            info.transform
        );
    }
}

}
