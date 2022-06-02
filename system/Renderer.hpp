#pragma once

#include "define.hpp"

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <optional>

#include "Types.hpp"
#include "Resources.hpp"
#include "df2.hpp"
#include "Mesh.hpp"
#include "Matrix.hpp"
#include "Color.hpp"
#include "Camera.hpp"


namespace GAME_TITLE {

    enum BlendMode {
        None, Multiply, Add, Subtract
    };

    struct Transform2D {
        operator Mat3f() const;

        fvec2d translate = {0.f, 0.f};
        float rotate = 0.f;
        fvec2d rotationCenter = {0.0f, 0.0f};
        fvec2d scale = {1.f, 1.f};
        fvec2d shear = {0.f, 0.f};
        bool reflectX = false;
        bool reflectY = false;
    };

    struct VisualAppearance {
        Texture *texture = nullptr;
        BlendMode blendMode = BlendMode::Multiply;
        color fillColor = WHITE;
        unsigned int outlineWidth = 0;
        color outlineColor = BLACK;
    };

    constexpr VisualAppearance DEFAULT_APPEARANCE = VisualAppearance{
            .texture = nullptr,
            .blendMode = BlendMode::None,
            .fillColor = WHITE,
            .outlineWidth = 0,
            .outlineColor = BLACK
    };

    class Renderer {
    public:
        Camera2D camera;

        Renderer();

        ~Renderer();

        bool create(df2 &settings);

        void destroy();

        bool setMode(int width, int height, bool fullscreen, int refreshRate = 60);

        bool runTasks(double delta);

        void begin();

        void end();

        virtual void setResources(Resources *res);

        [[nodiscard]] isize getSize() const;

        GLFWwindow *getWindowHandle();

        void drawClear(const color &color);

        void drawRect(const drect &rec, const VisualAppearance &appearance = DEFAULT_APPEARANCE,
                      const Mat3f& transform = MAT3_NO_TRANSFORM<float>);

        void drawCircle(const dcircle &circ, const VisualAppearance &appearance = DEFAULT_APPEARANCE,
                        const Mat3f& transform = MAT3_NO_TRANSFORM<float>);

        void drawSegment(const GLLine &seg, const VisualAppearance &appearance = DEFAULT_APPEARANCE,
                         const Mat3f& transform = MAT3_NO_TRANSFORM<float>);

        void drawMesh(const Mesh &mesh, const Shader &shader, const Shader::Uniforms &shaderUniforms = {},
                      const Shader::Textures &textures = {}, const Mat3f& transform = MAT3_NO_TRANSFORM<float>);

    private:
        static void onError(int code, const char* msg);
        static void onResize(GLFWwindow *window, int width, int height);

        bool createStaticGeometry(int resolution);

        Mesh rectShape, circleShape, cuboidShape, sphereShape, arrowTopShape;
        Shader static2DShader, lineShader;

        Resources *resources;
        GLFWwindow *windowHandle;
    };
}