#include "Renderer.dpp"

#include <GL/glew.h>

#define GLFW_DLL

#include <GLFW/glfw3.h>

#include "Math.hpp"
#include "Util.hpp"
#include "Context.hpp"

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
    auto &settings = GetContext().settings["gfx"];
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