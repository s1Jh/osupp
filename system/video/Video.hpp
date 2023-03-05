#pragma once

#include "define.hpp"

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Color.hpp"
#include "Texture.hpp"
#include "EnumOperators.hpp"


#define TO_SDL(V) ((::SDL_Window*)(V))
#define FROM_SDL(V) ((WindowHandle*)(V))
#define TO_IMGUI(V) ((::ImGuiContext*)(V))
#define FROM_IMGUI(V) ((ImHandle*)(V))

namespace PROJECT_NAMESPACE {

namespace video
{

constexpr size_t RENDER_QUEUE_SIZE = 1024;
constexpr size_t RENDER_LAYERS = 32;
constexpr size_t DEFAULT_RENDER_LAYER = RENDER_LAYERS / 2;

constexpr const char *GL_VERSION_STR = "330 core";
constexpr const char *GL_VERSION_PREPROCESSOR = "#version 330 core";

#define LAYER(NAME, EXPR, ...) constexpr size_t NAME = EXPR;
    USER_LAYER_DEFINITIONS
#undef LAYER

enum class WindowMode
{
    NONE, WINDOWED, FULLSCREEN, WINDOWED_BORDERLESS
};

enum class WindowVisibility
{
    NONE, VISIBLE, TASKBAR, HIDDEN
};

constexpr isize DEFAULT_WINDOW_SIZE = {640, 480};

constexpr int DEFAULT_WINDOW_MONITOR = 0;

constexpr int DEFAULT_WINDOW_REFRESH_RATE = 60;

constexpr WindowMode DEFAULT_WINDOW_VIDEO_MODE = WindowMode::WINDOWED;

constexpr WindowVisibility DEFAULT_WINDOW_VISIBILITY = WindowVisibility::HIDDEN;

struct WindowConfiguration
{
    isize size{-1, -1};
    int refreshRate{-1};
    int monitorID{-1};
    WindowMode mode{WindowMode::NONE};
    WindowVisibility shown{WindowVisibility::NONE};
};

enum class AppearanceFlags
{
    IGNORE_CAMERA = 1 << 0,
};

enum class AnimationLayout: bool
{
    HORIZONTAL, VERTICAL
};

enum BlendMode
{
    NONE, MULTIPLY, ADD, SUBTRACT
};

// TODO: Move these somewhere more fitting later.
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

// TODO: Transform3D
struct VisualAppearance
{
    Resource<Texture> texture{nullptr};
    const Mat3f *uvTransform = nullptr;
    BlendMode blendMode = BlendMode::MULTIPLY;
    color fillColor = WHITE;
    float outlineWidth = 0.01f;
    color outlineColor = BLACK;
    float zIndex = 0.0f;
    AppearanceFlags flags = static_cast<AppearanceFlags>(0);
};

}

ENABLE_BITMASK_OPERATORS(video::AppearanceFlags)

}
