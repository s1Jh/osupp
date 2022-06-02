#include "Renderer.hpp"

#include "Shaders.hpp"
#include "Util.hpp"
#include "Math.hpp"

#include "line.h"


namespace GAME_TITLE {

    Renderer::Renderer() :
            windowHandle(nullptr),
            camera(), resources(nullptr) {
    }

    Renderer::~Renderer() {
        destroy();
    }

    bool Renderer::setMode(int width, int height, bool fullscreen, int refreshRate) {
        LOG_ENTER("GFX");
        log::info("Setting mode ", width, "x", height, "@", refreshRate, "Hz fullscreen: ", fullscreen);

        if (fullscreen)
            glfwSetWindowMonitor(windowHandle, glfwGetPrimaryMonitor(), 0, 0, width, height, refreshRate);
        else
            glfwSetWindowSize(windowHandle, width, height);

        camera.setAspectRatio(float(height) / float(width));

        CheckGLFWh("RENDERER::SET_MODE");

        glfwShowWindow(windowHandle);
        CheckGLFW;

        return true;
    }

    bool Renderer::runTasks(double delta) {
        glfwPollEvents();
        camera.recalculateMatrix();

        //if (resources)
        //    resources->updateSprites(delta);
        return !glfwWindowShouldClose(windowHandle);
    }

    void Renderer::begin() {
        glfwMakeContextCurrent(windowHandle);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::end() {
        glfwSwapBuffers(windowHandle);
    }

    void Renderer::setResources(Resources *res) {
        resources = res;
    }

    void Renderer::destroy() {
        log::debug("Terminating renderer");
        glfwDestroyWindow(windowHandle);
        glfwTerminate();
    }

    void Renderer::onResize(GLFWwindow *window, int width, int height) {
        glfwMakeContextCurrent(window);
        glViewport(0, 0, width, height);
    }

    bool Renderer::create(df2 &settings) {
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
#endif //DEBUG
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

        //glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef DEBUG
        glEnable(GL_DEBUG_OUTPUT);
#endif // DEBUG

        log::info("OpenGL version: ", glGetString(GL_VERSION));
        log::info("GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
        log::info("Renderer: ", glGetString(GL_RENDERER));
        log::info("Vendor: ", glGetString(GL_VENDOR));

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glfwSwapInterval(0);
        glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        CheckGLFW;
        setMode(
                settings["width"].integer(640),
                settings["height"].integer(480),
                settings["fullscreen"].integer(0),
                settings["refresh_rate"].integer(60)
        );

        if (!createStaticGeometry(settings["resolution"].integer(64))) {
            log::error("Failed to create static shape geometry");
            return false;
        }

        return true;
    }

    GLFWwindow *Renderer::getWindowHandle() {
        return windowHandle;
    }

    isize Renderer::getSize() const {
        isize ret;
        glfwGetWindowSize(windowHandle, &ret.w, &ret.h);
        return ret;
    }

    void
    Renderer::drawRect(const drect &rec, const VisualAppearance &appearance, const std::unique_ptr<Mat3f> transform) {
        static2DShader.use();
        static2DShader.set("fill", appearance.fillColor);
        static2DShader.set("outline", appearance.outlineColor);
        static2DShader.set("outline_width", appearance.outlineWidth);
        static2DShader.set("blendMode", (int) appearance.blendMode);
        if (transform) {
            static2DShader.set("transform", *transform);
            static2DShader.set("camera", MAT3_NO_TRANSFORM<float>);
        } else {
            static2DShader.set("camera", camera.getMatrix());
            static2DShader.set("transform", MAT3_NO_TRANSFORM<float>);
        }

        Mat3f shape =
                MakeScaleMatrix<float>(rec.size) *
                MakeTranslationMatrix<float>(rec.position);
        static2DShader.set("shape", shape);

        if (appearance.texture) {
            appearance.texture->use(0);
            static2DShader.set("useTexture", true);
        } else
            static2DShader.set("useTexture", false);

        glBindVertexArray(rectShape.getVAO());
        glDrawElements(GL_TRIANGLES, rectShape.getElementCount(), GL_UNSIGNED_INT, nullptr);
        Shader::unbind();
        Texture::unbind(0);
    }

    void Renderer::drawCircle(const dcircle &circ, const VisualAppearance &appearance,
                              const std::unique_ptr<Mat3f> transform) {
        static2DShader.use();
        static2DShader.set("fill", appearance.fillColor);
        static2DShader.set("outline", appearance.outlineColor);
        static2DShader.set("outline_width", appearance.outlineWidth);
        static2DShader.set("blendMode", (int) appearance.blendMode);
        CheckGLh("Set shader appearance uniforms");
        if (transform) {
            static2DShader.set("transform", *transform);
            static2DShader.set("camera", MAT3_NO_TRANSFORM<float>);
        } else {
            static2DShader.set("camera", camera.getMatrix());
            static2DShader.set("transform", MAT3_NO_TRANSFORM<float>);
        }

        Mat3f shape =
                MakeScaleMatrix<float>(dvec2d{circ.radius, circ.radius}) *
                MakeTranslationMatrix<float>(circ.position);

        static2DShader.set("shape", shape);
        CheckGLh("Set shader matrices");

        if (appearance.texture) {
            appearance.texture->use(0);
            CheckGLh("Set texture sampler");
            static2DShader.set("useTexture", true);
            CheckGLh("Set texture uniform");
        } else {
            static2DShader.set("useTexture", false);
            CheckGLh("Set texture uniform");
        }

        glBindVertexArray(circleShape.getVAO());
        glDrawElements(GL_TRIANGLE_FAN, circleShape.getElementCount(), GL_UNSIGNED_INT, nullptr);
        CheckGLh("Draw");

        Shader::unbind();
        Texture::unbind(0);
        CheckGLh("Unbound");
    }

    void Renderer::drawMesh(
            const Mesh &mesh, const Shader &shader, const Shader::Uniforms &shaderUniforms,
            const Shader::Textures &textures, std::unique_ptr<Mat3f> transform
    ) {
        shader.use();
        CheckGLh("Bound shader");

        for (auto &uniform: shaderUniforms) {
#define MATCHF(type) if constexpr (std::is_same_v<T, type>) shader.set(uniform.first, arg);
#define MATCH(type) if constexpr (std::is_same_v<T, type>) shader.set(uniform.first, arg);
#define MATCHP(type) if constexpr (std::is_same_v<T, type>) shader.set(uniform.first, *arg);
            std::visit([&](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;
                MATCHF(float)
                MATCH(int)
                MATCH(unsigned int)
                MATCH(double)
                MATCH(fvec2d)
                MATCH(ivec2d)
                MATCH(uvec2d)
                MATCH(dvec2d)
                MATCH(fvec3d)
                MATCH(ivec3d)
                MATCH(uvec3d)
                MATCH(dvec3d)
                MATCH(fvec4d)
                MATCH(ivec4d)
                MATCH(uvec4d)
                MATCH(dvec4d)
                MATCHP(fvec2d*)
                MATCHP(ivec2d*)
                MATCHP(uvec2d*)
                MATCHP(dvec2d*)
                MATCHP(fvec3d*)
                MATCHP(ivec3d*)
                MATCHP(uvec3d*)
                MATCHP(dvec3d*)
                MATCHP(fvec4d*)
                MATCHP(ivec4d*)
                MATCHP(uvec4d*)
                MATCHP(dvec4d*)
                MATCHP(Mat2f*)
                MATCHP(Mat3f*)
                MATCHP(Mat4f*)
                MATCHP(color*)
            }, uniform.second);
#undef MATCHF
#undef MATCH
#undef MATCHP
        }
        CheckGLh("Set shader uniforms");

        if (transform) {
            shader.set("transform", *transform);
            shader.set("camera", MAT3_NO_TRANSFORM<float>);
        } else {
            shader.set("camera", camera.getMatrix());
            shader.set("transform", MAT3_NO_TRANSFORM<float>);
        }
        DumpGlErrors();

        for (auto &texture: textures) {
            texture.second->use(texture.first);
        }
        CheckGLh("Bound textures");

        glBindVertexArray(mesh.getVAO());
        glDrawElements(
                mesh.getRenderMode(), mesh.getElementCount(),
                GL_UNSIGNED_INT, nullptr);
        CheckGLh("Draw");

        Shader::unbind();
        for (auto &texture: textures) {
            Texture::unbind(texture.first);
        }
        CheckGLh("Unbound");
    }

    void Renderer::drawSegment(const GLLine &seg, const VisualAppearance &appearance,
                               std::unique_ptr<Mat3f> transform) {
        if (!seg.mesh.isValid())
            return;

        lineShader.use();
        lineShader.set("fill", appearance.fillColor);

        CheckGLh("Set shader appearance uniforms");
        if (transform) {
            lineShader.set("transform", *transform);
            lineShader.set("camera", MAT3_NO_TRANSFORM<float>);
        } else {
            lineShader.set("camera", camera.getMatrix());
            lineShader.set("transform", MAT3_NO_TRANSFORM<float>);
        }
        CheckGLh("Set shader matrices");

        glBindVertexArray(seg.mesh.getVAO());
        glDrawElements(GL_LINES, seg.mesh.getElementCount(), GL_UNSIGNED_INT, nullptr);
        CheckGLh("Draw");

        Shader::unbind();
        Texture::unbind(0);
        CheckGLh("Unbound");
    }

/*====================================================================================================================*/
/*  Creates geometry used for simple shapes                                                                           */
/*--------------------------------------------------------------------------------------------------------------------*/
    bool Renderer::createStaticGeometry(int resolution) {
        bool success = true;

        // Load the generic shape shader
        success &= static2DShader.load("static_shape.shader");
        success &= lineShader.load("line.shader");

        // Calculate circle geometry
        double circle_rotation = 2.0 * PI / (resolution - 1);
        fvec2d circle_vec = {1.f, 0.f};

        circleShape.setAttributeDescriptors(
                {
                        AttributeType::Vec2, // position
                        AttributeType::Vec2 // uv
                });
        circleShape.insertVertex({0.0, 0.0, 0.5f, 0.5f});
        int i = 0;
        for (; i < resolution; i++) {
            fvec2d vec = {circle_vec.x, circle_vec.y};
            fvec2d uv = (vec + fvec2d{1.f, 1.f}) / fvec2d{2.f, 2.f};
            circleShape.insertVertex({vec.x, vec.y, uv.x, uv.y});
            circleShape.insertIndice(i);
            circle_vec = Rotate(circle_vec, circle_rotation);
        }
        circleShape.insertIndice(1);
        success &= circleShape.upload();

        // Create geometry for rectangular meshes
        rectShape.setAttributeDescriptors(
                {
                        AttributeType::Vec2, // position
                        AttributeType::Vec2  // uv
                });
        rectShape.insertVertices(
                {
                        {1.f,  1.f,  1.f, 1.f},
                        {1.f,  -1.f, 1.f, 0.f},
                        {-1.f, -1.f, 0.f, 0.f},
                        {-1.f, 1.f,  0.f, 1.f}
                });
        rectShape.insertIndices(
                {
                        0, 1, 2,
                        0, 3, 2
                });
        success &= rectShape.upload();

        arrowTopShape.setAttributeDescriptors(
                {
                        AttributeType::Vec2 // position
                });
        arrowTopShape.insertVertices(
                {
                        {-0.8f, -0.5f},
                        {-0.8f, -0.8f},
                        {0.0f,  -0.2f},
                        {0.0f,  0.1f},
                        {0.8f,  -0.5f},
                        {0.8f,  -0.8f}
                });

        arrowTopShape.insertIndices(
                {
                        0, 1, 2,
                        0, 2, 3,
                        3, 4, 2,
                        2, 4, 5
                });

        return success;
    }

    void Renderer::drawClear(const color &color) {
        glClearColor(DECOMPOSE_COLOR_RGBA(color));
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::onError(int code, const char *msg) {
        log::error("GLFW: [", code, "] ", msg);
    }

    Transform2D::operator std::unique_ptr<Mat3f>() const {
        return std::make_unique<Mat3f>(
                MakeTranslationMatrix(translate) *
                MakeRotationMatrix<float>(rotate) *
                MakeScaleMatrix(scale) *
                MakeShearMatrix(shear)
        );
    }

    Transform2D::operator Mat3f() const {
        return
                MakeTranslationMatrix(translate) *
                MakeRotationMatrix<float>(rotate) *
                MakeScaleMatrix(scale) *
                MakeShearMatrix(shear);
    }
}