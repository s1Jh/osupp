#pragma once

#include "define.hpp"

#include "Camera.hpp"
#include "Color.hpp"
#include "Matrix.hpp"
#include "Mesh.hpp"
#include "Renderable.hpp"
#include "Resources.hpp"
#include "Sprite.hpp"
#include "Types.hpp"
#include "df2.hpp"

typedef struct GLFWwindow GLFWwindow;

NS_BEGIN

enum BlendMode
{
    None, Multiply, Add, Subtract
};

struct Transform2D
{
    operator Mat3f() const;

    fvec2d translate = {0.f, 0.f};
    float rotate = 0.f;
    fvec2d rotationCenter = {0.0f, 0.0f};
    fvec2d scale = {1.f, 1.f};
    fvec2d shear = {0.f, 0.f};
    bool reflectX = false;
    bool reflectY = false;
};

struct VisualAppearance
{
    const Texture *texture = nullptr;
    const Mat3f *uvTransform = nullptr;
    BlendMode blendMode = BlendMode::Multiply;
    color fillColor = WHITE;
    unsigned int outlineWidth = 0;
    color outlineColor = BLACK;
};

constexpr VisualAppearance DEFAULT_APPEARANCE =
    VisualAppearance{.texture = nullptr,
        .blendMode = BlendMode::None,
        .fillColor = WHITE,
        .outlineWidth = 0,
        .outlineColor = BLACK};

class Renderer
{
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

    [[nodiscard]] isize getSize() const;

    GLFWwindow *getWindowHandle();

    template<typename T>
    inline void draw(T &obj, typename T::DrawInfoClass &&info)
    {
        obj.draw(*this, info);
    }

    template<typename T>
    inline void draw(T &obj, typename T::DrawInfoClass &info)
    {
        obj.draw(*this, info);
    }

    void drawClear(const color &color);

    void drawRect(const drect &rec,
                  const VisualAppearance &appearance = DEFAULT_APPEARANCE,
                  const Mat3f &transform = MAT3_NO_TRANSFORM<float>);

    void drawCircle(const dcircle &circ,
                    const VisualAppearance &appearance = DEFAULT_APPEARANCE,
                    const Mat3f &transform = MAT3_NO_TRANSFORM<float>);

    void drawSegment(const GLLine &seg,
                     const VisualAppearance &appearance = DEFAULT_APPEARANCE,
                     const Mat3f &transform = MAT3_NO_TRANSFORM<float>);

    void drawCross(const fvec2d &pos, float size = 0.1f,
                   const VisualAppearance &appearance = DEFAULT_APPEARANCE,
                   const Mat3f &transform = MAT3_NO_TRANSFORM<float>);

    void drawMesh(const Mesh &mesh, const Shader &shader,
                  const Shader::Uniforms &shaderUniforms = {},
                  const Shader::Textures &textures = {},
                  const Mat3f &transform = MAT3_NO_TRANSFORM<float>);

    void drawSprite(const Sprite &sprite,
                    const Mat3f &transform = MAT3_NO_TRANSFORM<float>);

private:
    void drawGenericShape(const Shader &shader, const Mesh &mesh,
                          const Mat3f &shape, const VisualAppearance &appearance,
                          const Mat3f &transform,
                          RenderMode mode = RenderMode::Triangles) const;

    static void onError(int code, const char *msg);

    static void onResize(GLFWwindow *window, int width, int height);

    bool createStaticGeometry(int resolution);

    Mesh rectShape, circleShape, cuboidShape, sphereShape, crossShape;
    Shader static2DShader, lineShader, spriteShader;

    GLFWwindow *windowHandle;
};

NS_END