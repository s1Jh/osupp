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

NS_BEGIN

int State<GameState::Init>::update(double)
{
    const int batchSize = 50;
    auto &ctx = GetContext();

    auto end = Min(lastLoaded + batchSize, filesToLoad.size());
    int i = lastLoaded;

    if (ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        float percent = float(lastLoaded) / float(filesToLoad.size()) * 100.f;
        ImGui::Text("Loading files %i - %i (%.1f%%)", i, end, percent);
        ImGui::End();
    }

    for (; i < end; i++) {
        ctx.maps.push_back(ctx.resources.get<MapInfo>(filesToLoad[i].string()));
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
    GetContext().gfx.clear(color);
    return 0;
}

int State<GameState::Init>::exit()
{
    return 0;
}

int State<GameState::Init>::init(GameState)
{
    log::info("Initiating game...");

    auto &ctx = GetContext();
    auto &resources = ctx.resources;

    auto pathStr = ctx.settings.addSetting<std::string>("setting.paths", "", SettingFlags::WRITE_TO_FILE).get();
    const auto &paths = GetCharacterSeparatedValues(pathStr, ',');

    std::string skin = ctx.settings.addSetting<std::string>("setting.user.skin", "default").get();

    for (const auto &path: paths) {
        std::filesystem::path p(path);

        auto skinDirectory = p / "skins";
        resources.addSearchPath(skinDirectory);

        auto profilesDirectory = p / "profiles";
        resources.addSearchPath(profilesDirectory);
    }

    filesToLoad = resources.findFiles("songs", Resource<MapInfo>::allowedExtensions);

    ctx.activeSkin = resources.get<Skin>(skin);

    return 0;
}

NS_END