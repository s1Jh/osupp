#include "Helpers.hpp"

#include "MatrixMath.hpp"
#include "Constraint.hpp"
#include "Shaders.hpp"

NS_BEGIN

namespace video::helpers
{

void DrawGeneric2DShape(
    const LambdaRender &renderer, const Mesh &mesh,
    const Mat3f &shape, const VisualAppearance &appearance,
    const Mat3f &transform, RenderMode mode
)
{
    static bool wasInit;
    static Shader shader;
    if (!wasInit) {
        shader.fromString(STATIC_SHAPE_SHADER_VERT, STATIC_SHAPE_SHADER_FRAG);
        if (!shader.upload()) {
            log::error("Failed to upload the generic 2D shape shader!");
        }
        wasInit = true;
    }

    shader.use();
    shader.set("fill", appearance.fillColor);
    shader.set("outline", appearance.outlineColor);
    shader.set("outline_width", appearance.outlineWidth);
    shader.set("blendMode", (int) appearance.blendMode);
    shader.set("z", appearance.zIndex);

    shader.set("transform", transform);
    if (bool(appearance.flags & video::AppearanceFlags::IGNORE_CAMERA)) {
        shader.set("camera", MAT3_NO_TRANSFORM<float>);
    } else {
        shader.set("camera", renderer.camera.getMatrix());
    }
    CheckGLh("Set shader matrices");

    shader.set("shape", shape);
    CheckGLh("Set shader shape matrix");

    if (appearance.texture) {
        appearance.texture->use(0);
        shader.set("useTexture", true);
    } else {
        shader.set("useTexture", false);
    }
    CheckGLh("set texture");

    if (appearance.uvTransform) {
        shader.set("textureTransform", *appearance.uvTransform);
    } else if (appearance.texture) {
        shader.set("textureTransform", appearance.texture->getUVTransform());
    } else {
        shader.set("textureTransform", MAT3_NO_TRANSFORM < float > );
    }
    CheckGLh("set texture UV transform");

    glDepthFunc(GL_ALWAYS);
    glBindVertexArray(mesh.getGLData().VAO);
    glDrawElements(
        static_cast<unsigned int>(mode), mesh.getElementCount(),
        GL_UNSIGNED_INT, nullptr
    );
    CheckGLh("draw");

    Shader::unbind();
    Texture::unbind(0);
    CheckGLh("Unbound");
}

// TODO: Use geometry shaders to generate these generic meshes.

void DrawLineSegment(
    const LambdaRender &renderer, const dline &seg,
    const VisualAppearance &appearance, const Mat3f &transform
)
{
    static bool wasInit;
    static Shader lineShader;
    if (!wasInit) {
        lineShader.fromString(LINE_SHADER_VERT, LINE_SHADER_FRAG);
        if (!lineShader.upload()) {
            log::error("Failed to upload the generic line shader!");
        }
        wasInit = true;
    }

    const auto &rectShape = renderer.getMeshes().rectMask;

    lineShader.use();
    CheckGLh("Set shader");

    lineShader.set("resolution", (fvec2d) renderer.getWindow().size());
    lineShader.set("transform", transform);
    if (bool(appearance.flags & video::AppearanceFlags::IGNORE_CAMERA)) {
        lineShader.set("camera", MAT3_NO_TRANSFORM<float>);
    } else {
        lineShader.set("camera", renderer.camera.getMatrix());
    }
    CheckGLh("Set shader matrices");

    auto offset = appearance.outlineWidth;

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

    lineShader.set("z", appearance.zIndex);

    lineShader.set("shape", shape);
    CheckGLh("Set shader matrix");

    lineShader.set("thickness", appearance.outlineWidth);
    lineShader.set("fill", appearance.fillColor);
    lineShader.set("A", seg.A);
    lineShader.set("B", seg.B);
    CheckGLh("Set shader params");

    glBindVertexArray(rectShape.getGLData().VAO);
    glDrawElements(static_cast<unsigned int>(GL_TRIANGLES), rectShape.getElementCount(), GL_UNSIGNED_INT, nullptr);
    CheckGLh("draw");

    Shader::unbind();
    Texture::unbind(0);
    CheckGLh("Unbound");
}

}

NS_END
