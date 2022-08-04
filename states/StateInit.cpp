#include "StateInit.hpp"
#include "Standard.hpp"
#include "StateMainMenu.hpp"

#include "imgui/imgui.h"

NS_BEGIN

int State<GameState::Init>::update(double delta)
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
        ctx.maps.push_back(ctx.resources.get<MapInfo>(filesToLoad[i]));
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

int State<GameState::Init>::init(GameState state)
{
    log::info("Initiating game...");

    auto &ctx = GetContext();
    auto &resources = ctx.resources;

    auto pathStr = ctx.settings.addSetting<std::string>("setting.paths", "").get();
    const auto &paths = GetCharacterSeparatedValues(pathStr, ',');

    std::string skin = ctx.settings.addSetting<std::string>("setting.user.skin", "default").get();

    for (const auto &path: paths) {
        std::filesystem::path p(path);

        auto skinDirectory = p / "skins";
        resources.addSearchPath(skinDirectory);

        auto profilesDirectory = p / "profiles";
        resources.addSearchPath(profilesDirectory);
    }

    filesToLoad = resources.findFiles("songs", detail::ResourcePile<MapInfo>::allowedFileExtensions);

    ctx.activeSkin = resources.get<Skin>(skin);
    ctx.activeGameMode = std::make_unique<Standard>();

    return 0;
}

NS_END