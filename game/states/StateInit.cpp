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

#include "StateInit.hpp"
#include "StateMainMenu.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Timer.hpp"

NS_BEGIN

int State<GameState::Init>::update(double)
{
    const int batchSize = 50;

    auto end = Min(lastLoaded + batchSize, filesToLoad.size());
    int i = lastLoaded;

    if (ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        float percent = float(lastLoaded) / float(filesToLoad.size()) * 100.f;
        ImGui::Text("Loading files %i - %i (%.1f%%)", i, end, percent);
        ImGui::End();
    }

    for (; i < end; i++) {
        ctx->maps.push_back(ctx->resources.get<MapInfo>(filesToLoad[i].string()));
    }
    lastLoaded = i++;

    if ((size_t) lastLoaded == filesToLoad.size())
        return (int) GameState::MainMenu;

    return 0;
}

int State<GameState::Init>::draw()
{
    float percent = float(lastLoaded) / float(filesToLoad.size());
    auto color = WHITE;
    color.r = percent;
    color.b = 1.0f - percent;
    color.g = percent;
    ctx->gfx.clear(color);
    return 0;
}

int State<GameState::Init>::exit()
{
    return 0;
}

int State<GameState::Init>::init(GameState)
{
    log::custom("GREETING", "Hello, world!");

    log::info("Initializing ", TOSTRING(GAME_TITLE), " ver.", VERSION_MAJOR, '.', VERSION_MINOR, '.', VERSION_PATCH);
    log::info("Build: ", BUILD_TYPE, " (", BUILD_DATE, ' ', BUILD_TIME, ')');
    log::info("Target: ", PLATFORM, ", ", ARCH);

    auto currentPath = std::filesystem::current_path();
    df2::addAlias("GAMEDIR", currentPath.string());
    ctx->settings.read();
    ctx->resources.addSearchPath(currentPath);

    if (!StartTimerThread()) {
        log::error("Failed to start timers");
        return -1;
    }

    auto locale = ctx->settings.addSetting<std::string>(
            "setting.user.locale", std::string("english.ldf"), SettingFlags::WRITE_TO_FILE
    );
    ctx->locale.loadFromFile(locale.get());
    log::info("Set locale ", ctx->locale.getLocName());

    auto devices = GetAudioDevices();
    std::vector<std::string> deviceNames;
    for (const auto &dev : devices)
        deviceNames.push_back(dev.name);

    auto audioDev =
            ctx->settings.addSetting<std::string>("setting.audio.device", "", SettingFlags::WRITE_TO_FILE, deviceNames);
    ctx->audio = GetAudioDevice(audioDev.get());
    log::info("Configured audio");

    ctx->settings.subscribeCallback<SettingCallbacks::SETTING_CHANGED>(wrap([&audioDev, this](const std::string &n)
    {
       if (n != "setting.audio.device")
           return CallbackReturn::OK;

       ctx->audio = GetAudioDevice(audioDev.get());
       return CallbackReturn::OK;
    }));

    if (!ctx->gfx.create()) {
        return -2;
    }
//	ctx.resources.loadPersistentAssets();

    ctx->keyboard.setViewport(ctx->gfx.getWindowHandle());
    ctx->mouse.setViewport(ctx->gfx.getWindowHandle());

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(TO_GLFW(ctx->gfx.getWindowHandle()), true);
    ImGui_ImplOpenGL3_Init(GL_VERSION_PREPROCESSOR);

    auto &resources = ctx->resources;

    auto pathStr = ctx->settings.addSetting<std::string>("setting.paths", "", SettingFlags::WRITE_TO_FILE).get();
    const auto &paths = GetCharacterSeparatedValues(pathStr, ',');

    std::string skin = ctx->settings.addSetting<std::string>("setting.user.skin", "default").get();

    for (const auto &path: paths) {
        std::filesystem::path p(path);

        auto skinDirectory = p / "skins";
        resources.addSearchPath(skinDirectory);

        auto profilesDirectory = p / "profiles";
        resources.addSearchPath(profilesDirectory);
    }

    filesToLoad = resources.findFiles("songs", Resource<MapInfo>::allowedExtensions);

    ctx->activeSkin = resources.get<Skin>(skin);

    return 0;
}

NS_END