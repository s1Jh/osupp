#include "Renderer.dpp"

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>

#include "Math.hpp"
#include "Util.hpp"
#include "Context.hpp"

NS_BEGIN

const std::vector<std::string> Renderer::StandardResolutions = {
    "720x480",
    "960x540",
    "1280x720",
    "1366x768",
    "1600x900",
    "1920x1080",
    "2560x1440",
    "3200x1800",
    "3840x2160",
};

Renderer::Renderer()
    : camera(), windowHandle(nullptr)
{}

Renderer::~Renderer()
{ destroy(); }

bool Renderer::setMode(int width, int height, bool fullscreenIn,
                       int refreshRateIn)
{
    LOG_ENTER("GFX");
    log::info("Setting mode ", width, "x", height, "@", refreshRateIn,
              "Hz fullscreen: ", fullscreenIn);

    glfwSetWindowMonitor(windowHandle, fullscreenIn ? glfwGetPrimaryMonitor() : nullptr, 50, 50, width,
                         height, refreshRateIn);

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

void Renderer::begin(const color &clearColor)
{
    glfwMakeContextCurrent(windowHandle);

    glClearColor(DECOMPOSE_COLOR_RGBA(clearColor));
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(DECOMPOSE_COLOR_RGBA(BLACK));
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::end()
{ glfwSwapBuffers(windowHandle); }

void Renderer::destroy()
{
    if (windowHandle) {
        log::debug("Terminating renderer");

        glfwDestroyWindow(windowHandle);
        windowHandle = nullptr;

        // TODO: this probably shouldn't automatically de-initialize GLFW
        glfwTerminate();
    }
}

void Renderer::onResize(GLFWwindow *window, int width, int height)
{
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
}

bool Renderer::create()
{
    auto &settings = GetContext().settings;
    resolution = settings.addSetting<std::string>("setting.gfx.resolution", "1920x1080", true, StandardResolutions);
    fullscreen = settings.addSetting<bool>("setting.gfx.fullscreen", false, true);
    refreshRate = settings.addSetting<int>("setting.gfx.refresh_rate", 60, true, 0, 120);

    auto onSettingChange = [](Renderer *self, const std::string &) -> CallbackReturn
    {
        auto dimensions = GetCharacterSeparatedValues(self->resolution.get(), 'x');

        auto width = GetParam(dimensions, 0, 1280);
        auto height = GetParam(dimensions, 1, 720);
        self->setMode(width, height, self->fullscreen.get(), self->refreshRate.get());
        return CallbackReturn::Ok;
    };

    GetContext().settings.subscribeCallback<SettingCallbacks::SettingChanged>(wrap(onSettingChange), this);

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

    auto dimensions = GetCharacterSeparatedValues(resolution.get(), 'x');

    auto width = GetParam(dimensions, 0, 1280);
    auto height = GetParam(dimensions, 1, 720);

    setMode(width, height, fullscreen.get(), refreshRate.get());

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

void Renderer::clear(const color &color)
{
    glClearColor(DECOMPOSE_COLOR_RGBA(color));
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::onError(int code, const char *msg)
{
    log::error("GLFW: [", code, "] ", msg);
}

NS_END