//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#include "LambdaRender.hpp"

#include "Log.hpp"
#include "Video.hpp"
#include "Math.hpp"
#include "Util.hpp"
#include "Error.hpp"

#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_error.h>

#include "Shaders.hpp"

namespace PROJECT_NAMESPACE
{

namespace video
{

// TODO: Move this somewhere more fitting later.
Transform2D::operator Mat3f() const
{
    return math::MakeTranslationMatrix(translate) *
        math::MakeRotationMatrix<float>(rotate, rotationCenter) *
        math::MakeScaleMatrix(scale, translate) * math::MakeShearMatrix(shear);
}

bool LambdaRender::update()
{
    camera.recalculateMatrix();
    if (!isOpen()) {
        return false;
    }

    // ImGui installs its callbacks which rely on a ImGui state being set
    ImGui::SetCurrentContext(TO_IMGUI(imCtx));

    SDL_Event evt;

    while (SDL_PollEvent(&evt)) {
        ImGui_ImplSDL2_ProcessEvent(&evt);
        switch (evt.type) {
            case SDL_QUIT:SDL_DestroyWindow(TO_SDL(window));
                return false;
        }
    }

    ImGui::SetCurrentContext(nullptr);

    return true;
}

bool LambdaRender::configure(const WindowConfiguration &newCfg)
{
    if (!window) {
        return false;
    }

    auto *oldCtx = ImGui::GetCurrentContext();
    ImGui::SetCurrentContext(TO_IMGUI(imCtx)); // resizing windows will cause callbacks to ImGui

    auto &winCfg = currentConfig;

    winCfg.size.w = math::Max(newCfg.size.w != -1 ? newCfg.size.w : winCfg.size.w, 1);
    winCfg.size.h = math::Max(newCfg.size.h != -1 ? newCfg.size.h : winCfg.size.h, 1);

    if ((newCfg.size.w != 1) && (newCfg.size.h != -1)) {
        camera.setAspectRatio(float(newCfg.size.h) / float(newCfg.size.w));
    }

    winCfg.refreshRate = math::Max(newCfg.refreshRate != -1 ? newCfg.refreshRate : winCfg.refreshRate, 1);
    winCfg.mode = newCfg.mode != WindowMode::NONE ? newCfg.mode : winCfg.mode;
    winCfg.shown = newCfg.shown != WindowVisibility::NONE ? newCfg.shown : winCfg.shown;

    SDL_SetWindowSize(TO_SDL(window), winCfg.size.w, winCfg.size.h);
    int mode;
    switch (winCfg.mode) {
        case WindowMode::NONE:
        case WindowMode::WINDOWED:
            mode = 0;
            break;
        case WindowMode::FULLSCREEN:
            mode = SDL_WINDOW_FULLSCREEN;
            break;
        case WindowMode::WINDOWED_BORDERLESS:
            mode = SDL_WINDOW_FULLSCREEN_DESKTOP;
            break;
    }
    SDL_SetWindowFullscreen(TO_SDL(window), mode);

    winCfg.shown = newCfg.shown;

    switch (winCfg.shown) {
        case WindowVisibility::VISIBLE:
            SDL_ShowWindow(TO_SDL(window));
            break;
        case WindowVisibility::HIDDEN:
            SDL_HideWindow(TO_SDL(window));
            break;
        default: break;
    }

    ImGui::SetCurrentContext(oldCtx);

    for (auto &layer : layers) {
        layer.initialize(winCfg.size);
    }

    return true;
}

const WindowConfiguration &LambdaRender::getConfig() const
{
    return currentConfig;
}

void LambdaRender::begin()
{
    if (!window) {
        return;
    }
    renderStackSize = 0;
}

void LambdaRender::finish()
{
    if (!window) {
        return;
    }

    SDL_GL_MakeCurrent(TO_SDL(window), glContext);
    glViewport(0, 0, currentConfig.size.w, currentConfig.size.h);

    glClearColor(DECOMPOSE_COLOR_RGBA(BLACK));
    glClearDepth(0.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for (auto &layer : layers) {
        layer.bind();
        layer.used = false;
        glClearDepth(1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_ALWAYS);
    }

    RenderLayer::unbind();

// #ifdef IMGUI
    if (imCtx) {

        ImGui::SetCurrentContext(TO_IMGUI(imCtx));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }
// #endif

    for (size_t i = 0; i < renderStackSize; i++) {
        auto &task = renderQueue[i];
        auto &l = layers[task->layer % layers.size()];

        l.bind();
        l.used = true;

        task->invoke(*this);
    }

    RenderLayer::unbind();

    layerShader.use();

    glBindVertexArray(meshes.rect.getGLData().VAO);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_ALWAYS);
    for (const auto &layer : layers) {
        if (layer.used) {
            glBindTexture(GL_TEXTURE_2D, layer.color);
            glDrawElements(
                static_cast<unsigned int>(GL_TRIANGLES), meshes.rect.getElementCount(),
                GL_UNSIGNED_INT, nullptr
            );
        }
    }

    Shader::unbind();

// #ifdef IMGUI
    if (imCtx) {
        ImGui::Render();
        auto data = ImGui::GetDrawData();
        if (data) {
            ImGui_ImplOpenGL3_RenderDrawData(data);
        }
    }
// #endif

    SDL_GL_SwapWindow(TO_SDL(window));
}

const LambdaRender::GenericMeshCollection &LambdaRender::getMeshes() const
{
    return meshes;
}

bool LambdaRender::generateStaticGeometry()
{
    log::Debug("Generating standard meshes");

    bool success = true;

    meshes.rectMask.setAttributeDescriptors(
        {
            AttributeType::VEC2 // position
        }
    );
    meshes.rectMask.insertVertices(
        {{0.5f, 0.5f},
            {0.5f, -0.5f},
            {-0.5f, -0.5f},
            {-0.5f, 0.5f}}
    );
    meshes.rectMask.insertIndices({0, 1, 2, 0, 3, 2});
    success &= meshes.rectMask.upload();

    // Create geometry for rectangular meshes
    meshes.rect.setAttributeDescriptors(
        {
            AttributeType::VEC2, // position
            AttributeType::VEC2  // uv
        }
    );
    meshes.rect.insertVertices(
        {{0.5f, 0.5f, 1.f, 1.f},
            {0.5f, -0.5f, 1.f, 0.f},
            {-0.5f, -0.5f, 0.f, 0.f},
            {-0.5f, 0.5f, 0.f, 1.f}}
    );
    meshes.rect.insertIndices({0, 1, 2, 0, 3, 2});
    success &= meshes.rect.upload();

    meshes.screenRectMask.setAttributeDescriptors(
        {
            AttributeType::VEC2 // position
        }
    );
    meshes.screenRectMask.insertVertices(
        {{1.0f, 1.0f},
            {1.0f, -1.0f},
            {-1.0f, -1.0f},
            {-1.0f, 1.0f}}
    );
    meshes.screenRectMask.insertIndices({0, 1, 2, 0, 3, 2});
    success &= meshes.screenRectMask.upload();

    // Create geometry for rectangular meshes
    meshes.screenRect.setAttributeDescriptors(
        {
            AttributeType::VEC2, // position
            AttributeType::VEC2  // uv
        }
    );
    meshes.screenRect.insertVertices(
        {{1.0f, 1.0f, 1.f, 1.f},
            {1.0f, -1.0f, 1.f, 0.f},
            {-1.0f, -1.0f, 0.f, 0.f},
            {-1.0f, 1.0f, 0.f, 1.f}}
    );
    meshes.screenRect.insertIndices({0, 1, 2, 0, 3, 2});
    success &= meshes.screenRect.upload();

    const unsigned int resolution = 32;

    double circle_rotation = 2.0_pi / (resolution - 1);
    fvec2d circle_vec = {1.f, 0.f};

    meshes.circle.setAttributeDescriptors(
        {
            AttributeType::VEC2, // position
            AttributeType::VEC2  // uv
        }
    );
    meshes.circle.insertVertex({0.0, 0.0, 0.5f, 0.5f});

    for (unsigned int i = 0; i < resolution; i++) {
        fvec2d vec = {circle_vec[0], circle_vec[1]};
        fvec2d uv = (vec + 1.0f) / 2.0f;
        meshes.circle.insertVertex({vec[0], vec[1], uv[0], uv[1]});
        meshes.circle.insertIndice(i);
        circle_vec = math::Rotate(circle_vec, circle_rotation);
    }
    meshes.circle.insertIndice(1);
    success &= meshes.circle.upload();

    if (!success) {
        log::Error("Failed to generate static geometry");
    }

    return success;
}

bool LambdaRender::init(uint8_t msLevels)
{
    log::Logger logger("RENDER", log::Severity::INF);

    if (window != nullptr) {
        SDL_DestroyWindow(TO_SDL(window));
    }
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = FROM_SDL(SDL_CreateWindow(
        TOSTRING(GAME_TITLE),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1, 1,
        SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL
    ));

    if (!window) {
        const char *err = SDL_GetError();
        error::Raise(error::Code::API_FAIL_FATAL, std::string("Failed to open a window") + err);
        return false;
    }

    glContext = SDL_GL_CreateContext(TO_SDL(window));

    if (!glContext) {
        const char *err = SDL_GetError();
        error::Raise(error::Code::API_FAIL_FATAL, std::string("Failed to create an OpenGL context") + err);
        return false;
    }

    SDL_GL_MakeCurrent(TO_SDL(window), glContext);

    logger("OpenGL version: ", (char*)glGetString(GL_VERSION));
    logger("GLSL version: ", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    logger("Renderer: ", (char*)glGetString(GL_RENDERER));
    logger("Vendor: ", (char*)glGetString(GL_VENDOR));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_GL_SetSwapInterval(0);

    layerShader.fromString(LAYER_VERTEX_SHADER, LAYER_FRAGMENT_SHADER);
    layerShader.upload();
    if (!layerShader.uploaded()) {
        error::Raise(error::Code::API_FAIL_FATAL, "Failed to create multilayer blend shader");
        return false;
    }

    return generateStaticGeometry();
}

LambdaRender::~LambdaRender()
{
    if (window) {
        SDL_DestroyWindow(TO_SDL(window));
    }
}

bool LambdaRender::initImGui()
{
    IMGUI_CHECKVERSION();

    SDL_GL_MakeCurrent(TO_SDL(window), glContext);
    imCtx = FROM_IMGUI(ImGui::CreateContext());
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsClassic();
    ImGui_ImplSDL2_InitForOpenGL(TO_SDL(window), glContext);
    ImGui_ImplOpenGL3_Init(video::GL_VERSION_PREPROCESSOR);

    return true;
}

bool LambdaRender::isOpen() const
{
    return window != nullptr;
}

const isize &LambdaRender::getSize() const
{
    return currentConfig.size;
}

irect LambdaRender::getWindowRect()
{
    if (!isOpen()) {
        return UNIT_RECT<int>;
    }
    ivec2d pos;
    SDL_GetWindowPosition(TO_SDL(window), &pos[0], &pos[1]);
    ivec2d size = currentConfig.size;

    return {size, pos};
}

Mat3f LambdaRender::getWindowMatrix()
{
    if (!isOpen()) {
        return MAT3_NO_TRANSFORM<float>;
    }
    ivec2d pos;
    SDL_GetWindowPosition(TO_SDL(window), &pos[0], &pos[1]);
    ivec2d size = currentConfig.size;

    // TODO:
    return MAT3_NO_TRANSFORM<float>;
}

LambdaRender::RenderLayer::~RenderLayer()
{
    glDeleteTextures(1, &color);
    glDeleteRenderbuffers(1, &depth);
    glDeleteFramebuffers(1, &frame);
}

bool LambdaRender::RenderLayer::initialize(const isize &size)
{
    if (init) {
        glDeleteTextures(1, &color);
        glDeleteRenderbuffers(1, &depth);
        glDeleteFramebuffers(1, &frame);
    }

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
        return false;
    }

    CheckGLh("created framebuffers");
    init = true;
    return true;
}

bool LambdaRender::RenderLayer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frame);

    return true;
}

bool LambdaRender::RenderLayer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

}

}
