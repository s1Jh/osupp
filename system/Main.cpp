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

#include <cstdio>

#include <AL/alut.h>
#include <AL/al.h>
#include <AL/alc.h>

using namespace PROJECT_NAMESPACE;

#ifdef WIN32
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
#elif LINUX
int main(int argc, char **argv)
#endif
{
	alutInitWithoutContext(&argc, argv);

    log::custom("GREETING", "Hello, world!");

    log::info("Initializing ", TOSTRING(GAME_TITLE), " ver.", VERSION_MAJOR, '.', VERSION_MINOR, '.', VERSION_PATCH);
    log::info("Build: ", BUILD_TYPE, " (", BUILD_DATE, ' ', BUILD_TIME, ')');
    log::info("Target: ", PLATFORM, ", ", ARCH);

    auto &ctx = GetContext();

	auto currentPath = std::filesystem::current_path();
    df2::addAlias("GAMEDIR", currentPath);
	ctx.settings.read();
    ctx.resources.addSearchPath(currentPath);

	auto locale = ctx.settings
		.addSetting<std::string>("setting.user.locale", std::string("english.ldf"), SettingFlags::WriteToFile);
	ctx.locale.loadFromFile(locale.get());

	auto devices = GetAudioDevices();
	std::vector<std::string> deviceNames;
	for (const auto &dev : devices)
		deviceNames.push_back(dev.name);

	auto audioDev =
		ctx.settings.addSetting<std::string>("setting.audio.device", "", SettingFlags::WriteToFile, deviceNames);
	ctx.audio = GetAudioDevice(audioDev.get());

	ctx.settings.subscribeCallback<SettingCallbacks::SettingChanged>(wrap([&audioDev, &ctx](const std::string &)
																		  {
																			  ctx.audio =
																				  GetAudioDevice(audioDev.get());
																			  return CallbackReturn::Ok;
																		  }));

	if (!ctx.gfx.create()) {
		return 1;
	}
	ctx.resources.loadPersistentAssets();

	Keyboard::setViewport(ctx.gfx.getWindowHandle());
	Mouse::setViewport(ctx.gfx.getWindowHandle());

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(ctx.gfx.getWindowHandle(), true);
	ImGui_ImplOpenGL3_Init(GL_VERSION_PREPROCESSOR);

    ctx.settings.addSetting<bool>("setting.test.bool1", false, SettingFlags::None);
    ctx.settings.addSetting<bool>("setting.test.bool2", false, SettingFlags::None);
    ctx.settings.addSetting<bool>("setting.test.bool3", true, SettingFlags::None);
    ctx.settings.addSetting<bool>("setting.test.bool4", false, SettingFlags::None);
    ctx.settings.addSetting<bool>("setting.test.bool5", true, SettingFlags::None);
    ctx.settings.addSetting<bool>("setting.test.bool6", true, SettingFlags::None);

    ctx.settings.addSetting<color>("test.color1", PURPLE, SettingFlags::None);
    ctx.settings.addSetting<color>("test.color2", GREEN, SettingFlags::None);
    ctx.settings.addSetting<color>("test.color3", GRAY, SettingFlags::None);
    ctx.settings.addSetting<color>("test.color4", BLUE, SettingFlags::None);
    ctx.settings.addSetting<color>("test.color5", MAGENTA, SettingFlags::None);
    ctx.settings.addSetting<color>("test.color6", TURQUOISE, SettingFlags::None);

    ctx.settings.addSetting<color>("test.pallette", PURPLE, SettingFlags::None,
                                   std::vector<color>{PURPLE, AZURE, BEIGE, COBALT_BLUE, BRONZE, GOLD, SILVER});

    auto backFill = ctx.settings.addSetting<color>("debug.backfill", BLACK, SettingFlags::None);

    ctx.state.setState(GameState::INITIAL_STATE);

    while (ctx.state.isRunning()) {
        double delta = ctx.timing.getDelta();

        Keyboard::update();
        Mouse::update();

        if (ctx.keyboard[Key::Ctrl + Key::Q].releasing)
            ctx.state.setState(GameState::Exit);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ctx.state.update(delta);

        ctx.gfx.begin(backFill.get());
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
    std::exit(0);
}