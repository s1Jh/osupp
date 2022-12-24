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
#include "Math.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

NS_BEGIN

namespace video
{

void OnGLFWResize(WindowHandle *window, int width, int height)
{
	glfwMakeContextCurrent(TO_GLFW(window));
	glViewport(0, 0, width, height);
}

void OnGLFWError(int code, const char *msg)
{
	log::error("[GLFW] ", code, ": ", msg);
}

LambdaRender::LambdaRender()
{
	detail::InitContext();
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	contextHolder = std::shared_ptr<WindowHandle>(detail::CreateWindowHandle(), &ContextCloser);
}

void LambdaRender::ContextCloser(WindowHandle * win)
{
	glfwDestroyWindow(TO_GLFW(win));
	detail::DestroyContext();
}

bool LambdaRender::ownsWindow(const Window &win)
{
	return std::find(openedWindows.begin(), openedWindows.end(), win) != openedWindows.end();
}

bool LambdaRender::update()
{
	// ImGui installs its callbacks which rely on a ImGui state being set
	if (!openedWindows.empty()) {
		const auto &any = openedWindows.front();
		ImGui::SetCurrentContext(any->imCtx);
		glfwPollEvents();
		ImGui::SetCurrentContext(nullptr);
	}

	for (auto &window : openedWindows) {
		if (!window->wantsToBeOpen || glfwWindowShouldClose(TO_GLFW(window->handle))) {
			log::info("Closing handle ", window->handle);
			detail::FreeWindowHandle(window->handle);
			window->open = false;
		}
	}

	openedWindows.remove_if([](auto &window) -> bool
							{
								return !window->open;
							});

	return openedWindows.empty();
}


WindowHandle* LambdaRender::createWindowHandle()
{
	WindowHandle *handle = nullptr;

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

	handle = FROM_GLFW(glfwCreateWindow(1, 1, TOSTRING(GAME_TITLE), nullptr, TO_GLFW(contextHolder.get())));

	glfwSetWindowSizeCallback(TO_GLFW(handle), reinterpret_cast<GLFWwindowsizefun>(OnGLFWResize));

	IMGUI_CHECKVERSION();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(0);
	glfwSetInputMode(TO_GLFW(handle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	return handle;
}

bool LambdaRender::ConfigureWindow(Window win, const WindowConfiguration &newConfig)
{
	if (!win) {
		return false;
	}

	return ApplyWindowConfiguration(win, newConfig);
}

WindowConfiguration LambdaRender::GetWindowConfig(const Window &win)
{
	if (win) {
		return win->config;
	}
	return {};
}

Window LambdaRender::createWindow(const WindowConfiguration &config)
{
	if (!contextHolder) {
		return nullptr;
	}

	Window newWindow(new WindowImpl());

	newWindow->handle = detail::CreateWindowHandle();

	IMGUI_CHECKVERSION();

	newWindow->imCtx = ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(TO_GLFW(newWindow->handle), true);
	ImGui_ImplOpenGL3_Init(GL_VERSION_PREPROCESSOR);

	ImGui::SetCurrentContext(nullptr);

	ApplyWindowConfiguration(newWindow, config);

	openedWindows.push_back(newWindow);
	return newWindow;
}

bool LambdaRender::ApplyWindowConfiguration(Window &win, const WindowConfiguration &config)
{
	if (!win->handle) {
		return false;
	}

	ImGui::SetCurrentContext(win->imCtx); // resizing windows will cause callbacks to ImGui

	auto &winCfg = win->config;
	const auto &newCfg = config;

	winCfg.size.w = math::Max(newCfg.size.w != -1 ? newCfg.size.w : winCfg.size.w, 1);
	winCfg.size.h = math::Max(newCfg.size.h != -1 ? newCfg.size.h : winCfg.size.h, 1);

	winCfg.refreshRate = math::Max(newCfg.refreshRate != -1 ? newCfg.refreshRate : winCfg.refreshRate, 1);
	winCfg.mode = newCfg.mode != WindowMode::NONE ? newCfg.mode : winCfg.mode;
	winCfg.shown = newCfg.shown != WindowVisibility::NONE ? newCfg.shown : winCfg.shown;

	glfwSetWindowMonitor(TO_GLFW(win->handle),
						 winCfg.mode == WindowMode::FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr,
						 50,
						 50,
						 winCfg.size.w,
						 winCfg.size.h,
						 winCfg.refreshRate);

	winCfg.shown = newCfg.shown;

	switch (winCfg.shown) {
	case WindowVisibility::VISIBLE: glfwShowWindow(TO_GLFW(win->handle));
		break;
	case WindowVisibility::HIDDEN: glfwHideWindow(TO_GLFW(win->handle));
		break;
	default: break;
	}

	ImGui::SetCurrentContext(nullptr);

	return true;
}

void LambdaRender::begin()
{
	renderStackSize = 0;
}

void LambdaRender::finish(const Window& window)
{
	if (!contextHolder) {
		return;
	}

	if (!window->open) {
		return;
	}

	glfwMakeContextCurrent(TO_GLFW(window->handle));

	glViewport(0, 0, window->config.size.w, window->config.size.h);

	glClearColor(DECOMPOSE_COLOR_RGBA(BLACK));
	glClearDepth(0.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ImGui::SetCurrentContext(window->imCtx);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (int i = 0; i < renderStackSize; i++) {
		auto &task = renderQueue[i];
		task->invoke();
	}

	ImGui::Render();
	auto data = ImGui::GetDrawData();
	if (data) {
		ImGui_ImplOpenGL3_RenderDrawData(data);
	}

	glfwSwapBuffers(TO_GLFW(window->handle));
	glfwMakeContextCurrent(nullptr);
}

const LambdaRender::GenericMeshCollection &LambdaRender::getMeshes() const
{
	return meshes;
}

}

NS_END
