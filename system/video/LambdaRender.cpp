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

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#define GLFW_DLL
#include <GLFW/glfw3.h>

NS_BEGIN

namespace video
{

// TODO: Move this somewhere more fitting later.
Transform2D::operator Mat3f() const
{
    return math::MakeTranslationMatrix(translate) *
        math::MakeRotationMatrix<float>(rotate, rotationCenter) *
        math::MakeScaleMatrix(scale) * math::MakeShearMatrix(shear);
}

void OnGLFWResize(WindowHandle *window, int width, int height)
{
	glfwMakeContextCurrent(TO_GLFW(window));
	glViewport(0, 0, width, height);
}

bool LambdaRender::update()
{
    camera.recalculateMatrix();
    if (!window.isOpen()) {
        return false;
    }

	// ImGui installs its callbacks which rely on a ImGui state being set
    ImGui::SetCurrentContext(window.imCtx);
    glfwPollEvents();
    ImGui::SetCurrentContext(nullptr);

    if (!window.wantsToBeOpen || glfwWindowShouldClose(TO_GLFW(window.handle))) {
        glfwDestroyWindow(TO_GLFW(window.handle));
        window.open = false;
    }

	return window.open;
}

bool LambdaRender::configure(const WindowConfiguration &newConfig)
{
    if (!window.handle) {
        return false;
    }

    camera.setAspectRatio(float(newConfig.size.h) / float(newConfig.size.w));

    return ApplyWindowConfiguration(window, newConfig);
}

const WindowConfiguration& LambdaRender::getConfig() const
{
	return window.config;
}

bool LambdaRender::ApplyWindowConfiguration(Window &win, const WindowConfiguration &config)
{
	if (!win.handle) {
		return false;
	}

    auto* oldCtx = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(win.imCtx); // resizing windows will cause callbacks to ImGui

	auto &winCfg = win.config;
	const auto &newCfg = config;

	winCfg.size.w = math::Max(newCfg.size.w != -1 ? newCfg.size.w : winCfg.size.w, 1);
	winCfg.size.h = math::Max(newCfg.size.h != -1 ? newCfg.size.h : winCfg.size.h, 1);

	winCfg.refreshRate = math::Max(newCfg.refreshRate != -1 ? newCfg.refreshRate : winCfg.refreshRate, 1);
	winCfg.mode = newCfg.mode != WindowMode::NONE ? newCfg.mode : winCfg.mode;
	winCfg.shown = newCfg.shown != WindowVisibility::NONE ? newCfg.shown : winCfg.shown;

	glfwSetWindowMonitor(TO_GLFW(win.handle),
						 winCfg.mode == WindowMode::FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr,
						 50,
						 50,
						 winCfg.size.w,
						 winCfg.size.h,
						 winCfg.refreshRate);

	winCfg.shown = newCfg.shown;

	switch (winCfg.shown) {
	case WindowVisibility::VISIBLE: glfwShowWindow(TO_GLFW(win.handle));
		break;
	case WindowVisibility::HIDDEN: glfwHideWindow(TO_GLFW(win.handle));
		break;
	default: break;
	}

    ImGui::SetCurrentContext(oldCtx);

	return true;
}

void LambdaRender::begin()
{
    if (!window.isOpen()) {
        return;
    }
	renderStackSize = 0;
}

void LambdaRender::finish()
{
    if (!window.isOpen()) {
        return;
    }

	glfwMakeContextCurrent(TO_GLFW(window.handle));
	glViewport(0, 0, window.config.size.w, window.config.size.h);

	glClearColor(DECOMPOSE_COLOR_RGBA(BLACK));
	glClearDepth(0.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

// #ifdef IMGUI
    if (window.imCtx) {
        ImGui::SetCurrentContext(window.imCtx);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
// #endif

	for (size_t i = 0; i < renderStackSize; i++) {
		auto &task = renderQueue[i];
		task->invoke(*this);
	}

// #ifdef IMGUI
    if (window.imCtx) {
        ImGui::Render();
        auto data = ImGui::GetDrawData();
        if (data) {
            ImGui_ImplOpenGL3_RenderDrawData(data);
        }
    }
// #endif

	glfwSwapBuffers(TO_GLFW(window.handle));
}

const LambdaRender::GenericMeshCollection &LambdaRender::getMeshes() const
{
	return meshes;
}

bool LambdaRender::GenerateStaticGeometry(GenericMeshCollection& meshes)
{
    log::debug("Generating standard meshes");

    bool success = true;

    meshes.rectMask.setAttributeDescriptors({
        AttributeType::VEC2 // position
    });
    meshes.rectMask.insertVertices({{1.f, 1.f},
                                              {1.f, -1.f},
                                              {-1.f, -1.f},
                                              {-1.f, 1.f}});
    meshes.rectMask.insertIndices({0, 1, 2, 0, 3, 2});
    success &= meshes.rectMask.upload();

    // Create geometry for rectangular meshes
    meshes.rect.setAttributeDescriptors({
                                                   AttributeType::VEC2, // position
                                                   AttributeType::VEC2  // uv
                                               });
    meshes.rect.insertVertices({{1.f, 1.f, 1.f, 1.f},
                                          {1.f, -1.f, 1.f, 0.f},
                                          {-1.f, -1.f, 0.f, 0.f},
                                          {-1.f, 1.f, 0.f, 1.f}});
    meshes.rect.insertIndices({0, 1, 2, 0, 3, 2});
    success &= meshes.rect.upload();

    const unsigned int resolution = 32;

    double circle_rotation = 2.0_pi / (resolution - 1);
    fvec2d circle_vec = {1.f, 0.f};

    meshes.circle.setAttributeDescriptors({
                                                     AttributeType::VEC2, // position
                                                     AttributeType::VEC2  // uv
                                                 });
    meshes.circle.insertVertex({0.0, 0.0, 0.5f, 0.5f});

    for (unsigned int i = 0; i < resolution; i++) {
        fvec2d vec = {circle_vec.x, circle_vec.y};
        fvec2d uv = (vec + 1.0f) / 2.0f;
        meshes.circle.insertVertex({vec.x, vec.y, uv.x, uv.y});
        meshes.circle.insertIndice(i);
        circle_vec = math::Rotate(circle_vec, circle_rotation);
    }
    meshes.circle.insertIndice(1);
    success &= meshes.circle.upload();

    if (!success) {
        log::error("Failed to generate static geometry");
    }

    return success;
}

bool LambdaRender::init()
{
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
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

    window.handle = FROM_GLFW(glfwCreateWindow(1, 1, TOSTRING(GAME_TITLE), nullptr, nullptr));
    if (!window.handle) {
        log::error("Failed to open window");
        return false;
    }
    glfwMakeContextCurrent(TO_GLFW(window.handle));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(0);
    glfwSetInputMode(TO_GLFW(window.handle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetWindowSizeCallback(TO_GLFW(window.handle), reinterpret_cast<GLFWwindowsizefun>(OnGLFWResize));
    window.open = true;

    return GenerateStaticGeometry(meshes);
}

LambdaRender::~LambdaRender()
{
    if (window.handle) {
        window.open = false;
        glfwDestroyWindow(TO_GLFW(window.handle));
    }
}

const Window &LambdaRender::getWindow() const
{
    return window;
}

bool LambdaRender::initImGui()
{
    IMGUI_CHECKVERSION();

    glfwMakeContextCurrent(TO_GLFW(window.handle));
    window.imCtx = ImGui::CreateContext();
    ImGui::SetCurrentContext(window.imCtx);
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(TO_GLFW(window.handle), true);
    ImGui_ImplOpenGL3_Init(video::GL_VERSION_PREPROCESSOR);

    return true;
}

}

NS_END
