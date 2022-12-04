/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#include "Renderer.dpp"

#include "GL.hpp"

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
{
	detail::FreeGraphicalContext(windowHandle);
}

bool Renderer::setMode(int width, int height, bool fullscreenIn,
                       int refreshRateIn)
{
    log::info("Setting mode ", width, "x", height, "@", refreshRateIn,
              "Hz fullscreen: ", fullscreenIn);

    glfwSetWindowMonitor((GLFWwindow*)windowHandle, fullscreenIn ? glfwGetPrimaryMonitor() : nullptr, 50, 50, width,
                         height, refreshRateIn);

    camera.setAspectRatio(float(height) / float(width));

    CheckGLFWh("RENDERER::SET_MODE");

    glfwShowWindow((GLFWwindow*)windowHandle);
    CheckGLFW;

    return true;
}

bool Renderer::runTasks()
{
    glfwPollEvents();
    camera.recalculateMatrix();
    return !glfwWindowShouldClose((GLFWwindow*)windowHandle);
}

void Renderer::begin(const color &clearColor)
{
    glfwMakeContextCurrent((GLFWwindow*)windowHandle);

	glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(DECOMPOSE_COLOR_RGBA(clearColor));
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::end()
{ glfwSwapBuffers((GLFWwindow*)windowHandle); }

bool Renderer::create()
{
    auto &settings = GetContext().settings;
    resolution = settings.addSetting<std::string>("setting.gfx.resolution", "1920x1080", SettingFlags::WRITE_TO_FILE,
												  StandardResolutions);
    fullscreen = settings.addSetting<bool>("setting.gfx.fullscreen", false, SettingFlags::WRITE_TO_FILE);
    refreshRate = settings.addSetting<int>("setting.gfx.refresh_rate", 60, SettingFlags::WRITE_TO_FILE, 0, 120);

    auto onSettingChange = [](Renderer *self, const std::string &name) -> CallbackReturn
    {
		if (!name.starts_with("setting.gfx."))
			return CallbackReturn::OK;

        auto dimensions = GetCharacterSeparatedValues(self->resolution.get(), 'x');

        auto width = GetParam(dimensions, 0, 1280);
        auto height = GetParam(dimensions, 1, 720);
        self->setMode(width, height, self->fullscreen.get(), self->refreshRate.get());
        return CallbackReturn::OK;
    };

    GetContext().settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(wrap(onSettingChange), this);

	windowHandle = detail::CreateWindowHandle();
	if (!windowHandle) {
		log::error("Unable to create a renderer");
		return false;
	}

    auto dimensions = GetCharacterSeparatedValues(resolution.get(), 'x');

    auto width = GetParam(dimensions, 0, 1280);
    auto height = GetParam(dimensions, 1, 720);

    setMode(width, height, fullscreen.get(), refreshRate.get());

    return true;
}

WindowHandle *Renderer::getWindowHandle()
{ return windowHandle; }

isize Renderer::getSize() const
{
    isize ret;
    glfwGetWindowSize((GLFWwindow*)windowHandle, &ret.w, &ret.h);
    return ret;
}

void Renderer::clear(const color &color)
{
    glClearColor(DECOMPOSE_COLOR_RGBA(color));
    glClear(GL_COLOR_BUFFER_BIT);
}

NS_END