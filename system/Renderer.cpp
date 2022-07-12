#include "Renderer.hpp"

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>

#include "Math.hpp"
#include "Util.hpp"

NS_BEGIN

Renderer::Renderer()
    : camera(), windowHandle(nullptr)
{}

Renderer::~Renderer()
{ destroy(); }

bool Renderer::setMode(int width, int height, bool fullscreen,
                       int refreshRate)
{
    LOG_ENTER("GFX");
    log::info("Setting mode ", width, "x", height, "@", refreshRate,
              "Hz fullscreen: ", fullscreen);

    if (fullscreen)
        glfwSetWindowMonitor(windowHandle, glfwGetPrimaryMonitor(), 0, 0, width,
                             height, refreshRate);
    else
        glfwSetWindowSize(windowHandle, width, height);

    camera.setAspectRatio(float(height) / float(width));

    CheckGLFWh("RENDERER::SET_MODE");

    glfwShowWindow(windowHandle);
    CheckGLFW;

    return true;
}

bool Renderer::runTasks(double delta)
{
    glfwPollEvents();
    camera.recalculateMatrix();
    return !glfwWindowShouldClose(windowHandle);
}

void Renderer::begin()
{
    glfwMakeContextCurrent(windowHandle);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::end()
{ glfwSwapBuffers(windowHandle); }

void Renderer::destroy()
{
    log::debug("Terminating renderer");
    glfwDestroyWindow(windowHandle);

    // TODO: this probably shouldn't automatically de-initialize GLFW
    glfwTerminate();
}

void Renderer::onResize(GLFWwindow *window, int width, int height)
{
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

bool Renderer::create(df2 &settings)
{
    glfwSetErrorCallback(Renderer::onError);

    if (glfwInit() != GLFW_TRUE) {
        log::error("Failed to initialize GLFW");
        CheckGLFW;
        return false;
    }
    log::info("GLFW version: ", glfwGetVersionString());

#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // DEBUG
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    windowHandle = glfwCreateWindow(1, 1, TOSTRING(GAME_TITLE), nullptr, nullptr);
    if (!windowHandle) {
        CheckGLFW;
        log::error("Unable to open a GLFW window");
        return false;
    }

    glfwSetWindowSizeCallback(windowHandle, onResize);
    glfwMakeContextCurrent(windowHandle);

    if (glewInit() != GLEW_OK) {
        log::error("Failed to load GLEW");
        return false;
    }

//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
#endif // DEBUG

    log::info("OpenGL version: ", glGetString(GL_VERSION));
    log::info("GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
    log::info("Renderer: ", glGetString(GL_RENDERER));
    log::info("Vendor: ", glGetString(GL_VENDOR));

    glfwSwapInterval(0);
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    CheckGLFW;
    setMode(settings["width"].integer(640), settings["height"].integer(480),
            settings["fullscreen"].integer(0),
            settings["refresh_rate"].integer(60));

    if (!createStaticGeometry(settings["resolution"].integer(64))) {
        log::error("Failed to create static shape geometry");
        return false;
    }

    return true;
}

GLFWwindow *Renderer::getWindowHandle()
{ return windowHandle; }

isize Renderer::getSize() const
{
    isize ret;
    glfwGetWindowSize(windowHandle, &ret.w, &ret.h);
    return ret;
}

void Renderer::drawMesh(const Mesh &mesh, const Shader &shader,
                        const Shader::Uniforms &shaderUniforms,
                        const Shader::Textures &textures,
                        const Mat3f &transform)
{
    if (!mesh.isValid())
        return;

    shader.use();
    CheckGLh("Bound shader");

    for (auto &uniform: shaderUniforms) {
#define MATCHF(type)                                                           \
  if constexpr (std::is_same_v<T, type>)                                       \
    shader.set(uniform.first, arg);
#define MATCH(type)                                                            \
  else if constexpr (std::is_same_v<T, type>) shader.set(uniform.first, arg);
#define MATCHP(type)                                                           \
  else if constexpr (std::is_same_v<T, type>) shader.set(uniform.first, *arg);
#define END_MATCH()                                                            \
  else log::warning("Unable to deduce type for shader uniform ", uniform.first);
        std::visit(
            [&](auto &&arg)
            {
                using T = std::decay_t<decltype(arg)>;
                MATCHF(float)
                MATCH(int)MATCH(unsigned int) MATCH(double) MATCH(fvec2d) MATCH(ivec2d)MATCH(uvec2d) MATCH(dvec2d) MATCH(
                    fvec3d) MATCH(ivec3d)MATCH(uvec3d) MATCH(dvec3d) MATCH(fvec4d) MATCH(ivec4d)MATCH(uvec4d) MATCH(
                    dvec4d) MATCH(color) MATCHP(fvec2d *)MATCHP(ivec2d *) MATCHP(uvec2d *) MATCHP(dvec2d *)MATCHP(fvec3d *) MATCHP(
                    ivec3d *) MATCHP(uvec3d *)MATCHP(dvec3d *) MATCHP(fvec4d *)MATCHP(ivec4d *) MATCHP(uvec4d *)MATCHP(
                    dvec4d *) MATCHP(Mat2f *)MATCHP(Mat3f *) MATCHP(Mat4f *)MATCHP(color *) END_MATCH()
            },
            uniform.second);
#undef MATCHF
#undef MATCH
#undef MATCHP
#undef END_MATCH
    }
    CheckGLh("Set shader uniforms");

    shader.set("transform", transform);
    shader.set("camera", camera.getMatrix());
    DumpGlErrors();

    for (auto &texture: textures) {
        texture.second->use(texture.first);
    }
    CheckGLh("Bound textures");

    glBindVertexArray(mesh.getVAO());
    glDrawElements(static_cast<unsigned int>(mesh.getRenderMode()),
                   mesh.getElementCount(), GL_UNSIGNED_INT, nullptr);
    CheckGLh("draw");

    Shader::unbind();
    for (auto &texture: textures) {
        Texture::unbind(texture.first);
    }
    CheckGLh("Unbound");
}

void Renderer::drawRect(const drect &rec, const VisualAppearance &appearance,
                        const Mat3f &transform)
{
    Mat3f shape = MakeScaleMatrix<float>(rec.size) *
        MakeTranslationMatrix<float>(rec.position);
    drawGenericShape(static2DShader, rectShape, shape, appearance, transform,
                     RenderMode::Triangles);
}

void Renderer::drawCircle(const dcircle &circ,
                          const VisualAppearance &appearance,
                          const Mat3f &transform)
{
    Mat3f shape = MakeScaleMatrix<float>(dvec2d{circ.radius, circ.radius}) *
        MakeTranslationMatrix<float>(circ.position);
    drawGenericShape(static2DShader, circleShape, shape, appearance, transform,
                     RenderMode::TriangleFan);
}

void Renderer::drawSegment(const dline &seg,
                           const VisualAppearance &appearance,
                           const Mat3f &transform)
{
    lineShader.use();
    CheckGLh("Set shader");

    lineShader.set("resolution", (fvec2d) getSize());
    lineShader.set("transform", transform);
    lineShader.set("camera", camera.getMatrix());
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

void Renderer::drawSprite(const Sprite &sprite, const Mat3f &transform)
{
    Mat3f shape =
        MakeScaleMatrix<float>(sprite.getSize()) *
            MakeRotationMatrix<float>(sprite.getRotation(), sprite.getPivotPoint()) *
            MakeTranslationMatrix<float>(sprite.getPosition());
    Mat3f texture = MakeScaleMatrix<float>(sprite.getClipRectSize()) *
        MakeTranslationMatrix<float>(sprite.getClipRectPosition());
    drawGenericShape(spriteShader, rectShape, shape,
                     {.texture = sprite.getTexture().get(),
                         .uvTransform = &texture,
                         .fillColor = sprite.getTint()},
                     transform);

    spriteShader.use();
}

void Renderer::drawCross(const fvec2d &pos, float size,
                         const VisualAppearance &appearance,
                         const Mat3f &transform)
{
    size = size / 2.0f;
    dline a = {pos + fvec2d{size, size}, pos + fvec2d{-size, -size}};
    dline b = {pos + fvec2d{-size, size}, pos + fvec2d{size, -size}};
    drawSegment(a, appearance, transform);
    drawSegment(b, appearance, transform);
}

/*====================================================================================================================*/
/*  Creates geometry used for simple shapes */
/*--------------------------------------------------------------------------------------------------------------------*/
bool Renderer::createStaticGeometry(int resolution)
{
    bool success = true;

    // Load the generic shape shader
    success &= static2DShader.load("static_shape.shader");
    success &= spriteShader.load("sprite.shader");
    success &= lineShader.load("line.shader");

    // Calculate circle geometry
    double circle_rotation = 2.0 * PI / (resolution - 1);
    fvec2d circle_vec = {1.f, 0.f};

    circleShape.setAttributeDescriptors({
                                            AttributeType::Vec2, // position
                                            AttributeType::Vec2  // uv
                                        });
    circleShape.insertVertex({0.0, 0.0, 0.5f, 0.5f});
    int i = 0;
    for (; i < resolution; i++) {
        fvec2d vec = {circle_vec.x, circle_vec.y};
        fvec2d uv = (vec + 1.0f) / 2.0f;
        circleShape.insertVertex({vec.x, vec.y, uv.x, uv.y});
        circleShape.insertIndice(i);
        circle_vec = Rotate(circle_vec, circle_rotation);
    }
    circleShape.insertIndice(1);
    success &= circleShape.upload();

    // Create geometry for rectangular meshes
    rectShape.setAttributeDescriptors({
                                          AttributeType::Vec2, // position
                                          AttributeType::Vec2  // uv
                                      });
    rectShape.insertVertices({{1.f, 1.f, 1.f, 1.f},
                              {1.f, -1.f, 1.f, 0.f},
                              {-1.f, -1.f, 0.f, 0.f},
                              {-1.f, 1.f, 0.f, 1.f}});
    rectShape.insertIndices({0, 1, 2, 0, 3, 2});
    success &= rectShape.upload();

    crossShape.setAttributeDescriptors({
                                           AttributeType::Vec2 // position
                                       });
    float sqr2 = float(std::sqrt(2.0)) / 2.0f;
    crossShape.insertVertices({
                                  {sqr2, sqr2},
                                  {sqr2, -sqr2},
                                  {-sqr2, sqr2},
                                  {-sqr2, -sqr2},
                              });

    crossShape.insertIndices({0, 3, 1, 2});
    success &= crossShape.upload();

    return success;
}

void Renderer::drawClear(const color &color)
{
    glClearColor(DECOMPOSE_COLOR_RGBA(color));
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::onError(int code, const char *msg)
{
    log::error("GLFW: [", code, "] ", msg);
}

void Renderer::drawGenericShape(const Shader &shader, const Mesh &mesh,
                                const Mat3f &shape,
                                const VisualAppearance &appearance,
                                const Mat3f &transform, RenderMode mode) const
{
    shader.use();
    shader.set("fill", appearance.fillColor);
    shader.set("outline", appearance.outlineColor);
    shader.set("outline_width", appearance.outlineWidth);
    shader.set("blendMode", (int) appearance.blendMode);
    lineShader.set("z", appearance.zIndex);

    shader.set("transform", transform);
    shader.set("camera", camera.getMatrix());
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

Transform2D::operator Mat3f() const
{
    return MakeTranslationMatrix(translate) *
        MakeRotationMatrix<float>(rotate, rotationCenter) *
        MakeScaleMatrix(scale) * MakeShearMatrix(shear);
}

NS_END