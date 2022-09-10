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

#include "define.hpp"

#include "Context.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SoundSample.hpp"
#include "Timer.hpp"
#include "Resource.hpp"

#ifdef WINDOWS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

using namespace PROJECT_NAMESPACE;

NS_BEGIN
template <>
Resource<int> Load(const std::filesystem::path &p) {
	log::debug(p);
	Resource<int> r;
	*(r.held) = 3;
	return r;
}

NS_END

#if defined(WINDOWS) && defined(RELEASE)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
#else
int main()
#endif
{
	auto res = Load<int>("f");

	auto res2 = res;

	*res2 = 5;

	auto def = Default<int>();

    log::custom("GREETING", "Hello, world!");

    log::info("Initializing ", TOSTRING(GAME_TITLE), " ver.", VERSION_MAJOR, '.', VERSION_MINOR, '.', VERSION_PATCH);
    log::info("Build: ", BUILD_TYPE, " (", BUILD_DATE, ' ', BUILD_TIME, ')');
    log::info("Target: ", PLATFORM, ", ", ARCH);

    auto &ctx = GetContext();

	auto currentPath = std::filesystem::current_path();
    df2::addAlias("GAMEDIR", currentPath.string());
	ctx.settings.read();
    ctx.resources.addSearchPath(currentPath);

	if (!StartTimerThread()) {
		log::error("Failed to start timers");
		return -1;
	}

	auto locale = ctx.settings
		.addSetting<std::string>("setting.user.locale", std::string("english.ldf"), SettingFlags::WRITE_TO_FILE);
	ctx.locale.loadFromFile(locale.get());
	log::info("Set locale ", ctx.locale.getLocName());

	auto devices = GetAudioDevices();
	std::vector<std::string> deviceNames;
	for (const auto &dev : devices)
		deviceNames.push_back(dev.name);

	auto audioDev =
		ctx.settings.addSetting<std::string>("setting.audio.device", "", SettingFlags::WRITE_TO_FILE, deviceNames);
	ctx.audio = GetAudioDevice(audioDev.get());
	log::info("Configured audio");

	ctx.settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(wrap([&audioDev, &ctx](const std::string &)
																		  {
																			  ctx.audio =
																				  GetAudioDevice(audioDev.get());
																			  return CallbackReturn::OK;
																		  }));

	if (!ctx.gfx.create()) {
		return -2;
	}
//	ctx.resources.loadPersistentAssets();

	ctx.keyboard.setViewport(ctx.gfx.getWindowHandle());
	ctx.mouse.setViewport(ctx.gfx.getWindowHandle());

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(TO_GLFW(ctx.gfx.getWindowHandle()), true);
	ImGui_ImplOpenGL3_Init(GL_VERSION_PREPROCESSOR);

    ctx.state.setState(GameState::INITIAL_STATE);

    while (ctx.state.isRunning()) {
        double delta = ctx.timing.getDelta();

		ctx.keyboard.update();
		ctx.mouse.update();
		ctx.audio.process();

        if (ctx.keyboard[Key::CTRL + Key::Q].releasing)
            ctx.state.setState(GameState::EXIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ctx.gfx.begin();

		ctx.state.update(delta);
        ctx.state.draw();

        ImGui::Render();
        auto data = ImGui::GetDrawData();
        if (data)
            ImGui_ImplOpenGL3_RenderDrawData(data);

        ctx.gfx.end();

        if (!ctx.gfx.runTasks())
            ctx.state.exit();

        ctx.state.process();

        ctx.timing.await();
    }

    log::custom("GREETING", "Goodbye, world!");
    ctx.settings.write();
	StopTimerThread();
    std::exit(0);
}