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
#include "Files.hpp"

#include "imgui/imgui.h"

namespace PROJECT_NAMESPACE {

int State<GameState::Init>::update(double)
{
    setState(GameState::MainMenu);
	return 0;
}

int State<GameState::Init>::draw()
{
    return 0;
}

int State<GameState::Init>::exit()
{
    ctx->gfx.configure(video::WindowConfiguration{.shown = video::WindowVisibility::VISIBLE});
    return 0;
}

int State<GameState::Init>::init(GameState)
{
    log::Info("Initiating game...");

    auto pathStr = ctx->settings.addSetting<std::string>("setting.paths", "", SettingFlags::WRITE_TO_FILE).get();
    const auto &paths = GetCharacterSeparatedValues(pathStr, ',');

    for (const auto &path : paths) {
        std::filesystem::path p(path);
        ctx->paths.addPath(p);
    }

    log::Info("Searching these paths:");
    for (const auto& p : ctx->paths) {
        log::Info('\t', p);
    }

    ctx->maps.load(ctx->paths);

    auto skinPaths = ctx->paths.findAll("skins", Resource<Skin>::allowedExtensions);
    std::vector<std::string> skinNames;
    for (const auto &path : skinPaths) {
        skinNames.push_back(path.stem().string());
    }

    std::string skin =
        ctx->settings.addSetting<std::string>("setting.user.skin", "default", DEFAULT_SETTING_FLAGS, skinNames).get();

    auto path = ctx->paths.find(skin, Resource<Skin>::allowedExtensions);

    ctx->activeSkin = Load<Skin>(path);

    ctx->gui.create(ctx->activeSkin->getGuiMarkup());

    return 0;
}

}