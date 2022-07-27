#include "StateMainMenu.hpp"

#include "Log.hpp"
#include "Texture.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

NS_BEGIN

State<GameState::MainMenu>::State()
    : ctx(GetContext())
{}

void State<GameState::MainMenu>::showDebugControl()
{
    static unsigned int purged = 0;
    static bool showVersion = false;
    static bool showAssetPurge = false;
    static bool showImGuiVersion = false;

    if (showVersion) {
        if (ImGui::Begin(ctx.locale["ui.main.version.title"].c_str(),
                         &showVersion,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
            ImGui::Text("%s", TOSTRING(GAME_TITLE));
            ImGui::Separator();
            ImGui::Text(ctx.locale["ui.main.version.version"].c_str(),
                        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, PLATFORM, ARCH);
            ImGui::Text(ctx.locale["ui.main.version.build"].c_str(), BUILD_TYPE, BUILD_DATE, BUILD_TIME);
            ImGui::Separator();
            ImGui::Text("Copyright (c) 2022-2025 s1Jh");
            ImGui::End();
        }
    }

    if (showImGuiVersion)
        ImGui::ShowAboutWindow(&showImGuiVersion);

    if (showAssetPurge) {
        if (ImGui::Begin(ctx.locale["ui.main.purge.title"].c_str(),
                         &showAssetPurge,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Modal)) {
            ImGui::Text(ctx.locale["ui.main.purge.message"].c_str(), purged);
            ImGui::End();
        }
    }

    auto size = (fsize) GetContext().gfx.getSize();
    ImGui::SetNextWindowSize({size.w, size.h});
    ImGui::SetNextWindowPos({0, 0});
    if (!ImGui::Begin(ctx.locale["ui.main.title"].c_str(), nullptr,
                      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu(ctx.locale["ui.main.about.title"].c_str())) {
            ImGui::MenuItem(ctx.locale["ui.main.about.game_version"].c_str(), nullptr, &showVersion);
            ImGui::MenuItem(ctx.locale["ui.main.about.imgui_version"].c_str(), nullptr, &showImGuiVersion);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ctx.locale["ui.main.debug.title"].c_str())) {
            if (ImGui::MenuItem(ctx.locale["ui.main.debug.purge"].c_str())) {
                showAssetPurge = true;
                purged = ctx.resources.purgeUnusedFiles();
            }

            if (ImGui::BeginMenu(ctx.locale["ui.main.debug.theme"].c_str())) {
                if (ImGui::MenuItem(ctx.locale["ui.main.debug.theme.light"].c_str())) {
                    ImGui::StyleColorsLight();
                }
                if (ImGui::MenuItem(ctx.locale["ui.main.debug.theme.dark"].c_str())) {
                    ImGui::StyleColorsDark();
                }
                if (ImGui::MenuItem(ctx.locale["ui.main.debug.theme.classic"].c_str())) {
                    ImGui::StyleColorsClassic();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (ImGui::BeginTabBar("#tabs")) {

        if (ImGui::BeginTabItem(ctx.locale["ui.main.maps.title"].c_str())) {
            showMainMenuTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ctx.locale["ui.main.settings.title"].c_str())) {
            showSettingsTab();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }


    ImGui::End();
}

int State<GameState::MainMenu>::update(double delta)
{
    return 0;
}

int State<GameState::MainMenu>::draw()
{
    showDebugControl();
    return 0;
}

int State<GameState::MainMenu>::exit()
{
    return 0;
}

int State<GameState::MainMenu>::init(GameState state)
{
    log::debug("Entering main menu");
    return 0;
}

void State<GameState::MainMenu>::showMainMenuTab()
{
    // FIXME: This doesn't properly work with skins
    static TextureP star = GetContext().resources.get<Texture>("star_icon");
    static TextureP difficulty = GetContext().resources.get<Texture>("difficulty_icon");
    static TextureP approach = GetContext().resources.get<Texture>("approach_rate_icon");
    static TextureP circle = GetContext().resources.get<Texture>("note_icon");
    static TextureP window = GetContext().resources.get<Texture>("hit_window_icon");
    static TextureP drain = GetContext().resources.get<Texture>("hp_drain_icon");
    static TextureP duration = GetContext().resources.get<Texture>("duration_icon");

    static unsigned int selected = -1;

    static ImGuiTextFilter filter;

    const float playButtonWidth = 40.f;
    auto size = (fsize) GetContext().gfx.getSize();

    ImGui::Text("%s", ctx.locale["ui.main.maps.caption"].c_str());

    if (ImGui::BeginTable("##", 9,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                          {0.0, size.h * 0.75f})) {
        ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn("SR");
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.name"].c_str(), ImGuiTableColumnFlags_WidthStretch, 100.0f);
        ImGui::TableSetupColumn("OD");
        ImGui::TableSetupColumn("AR");
        ImGui::TableSetupColumn("CS");
        ImGui::TableSetupColumn("HW");
        ImGui::TableSetupColumn("HP");
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.length"].c_str());

        ImGui::TableHeadersRow();
        for (unsigned int i = 0; i < ctx.maps.size(); i++) {
            const auto &map = ctx.maps[i];

            auto filterName = map->getRomanisedName() + " " + map->getRomanisedArtist();

            if (filter.PassFilter(filterName.c_str())) {

                // The play button
                ImGui::TableNextColumn();
                if (i == selected) {
                    if (ImGui::Button("Play!", {playButtonWidth, 0})) {
                        ctx.activeGameMode->setMap(map);
                        ctx.state.setState(GameState::InGame);
                    }
                }
                // Song star rating
                ImGui::TableNextColumn();
                char starRating[8] = "#UND";
                ImGui::Text("%s", starRating);
                ImGui::SameLine();
                ImGui::Image((ImTextureID) star->getID(), {10, 10}, {1, 1}, {0, 0}, {0.8, 0.8, 0, 1});

                // Song name and selectable field
                ImGui::TableNextColumn();
                std::string itemID = "##" + std::to_string(i);
                if (ImGui::Selectable(itemID.c_str(), false, ImGuiSelectableFlags_None)) {
                    selected = i;
                }
                ImGui::SameLine();
                auto name =
                    map->getRomanisedName() + " - " + map->getRomanisedArtist() + " (" + map->getDifficulty() + ')';
                if (name.empty())
                    name = "#unknown_" + std::to_string(long(map.get()));
                ImGui::Text("%s", name.c_str());

                // Song overall difficulty
                ImGui::TableNextColumn();
                ImGui::Image((ImTextureID) difficulty->getID(), {10, 10}, {1, 1}, {0, 0});
                ImGui::SameLine();
                char overallDiff[8];
                sprintf(overallDiff, "%.1f", map->getOverallDifficulty());
                ImGui::Text("%s", overallDiff);

                // Song approach time
                ImGui::TableNextColumn();
                ImGui::Image((ImTextureID) approach->getID(), {10, 10}, {1, 1}, {0, 0});
                ImGui::SameLine();
                char approachTime[8];
                sprintf(approachTime, "%.1f", map->getApproachTime());
                ImGui::Text("%s", approachTime);

                // Song circle size
                ImGui::TableNextColumn();
                ImGui::Image((ImTextureID) circle->getID(), {10, 10}, {1, 1}, {0, 0});
                ImGui::SameLine();
                char circleSize[8];
                sprintf(circleSize, "%.2f", map->getCircleSize());
                ImGui::Text("%s", circleSize);

                // Song hit window time
                ImGui::TableNextColumn();
                ImGui::Image((ImTextureID) window->getID(), {10, 10}, {1, 1}, {0, 0});
                ImGui::SameLine();
                char hitWindow[8];
                sprintf(hitWindow, "%.1f", map->getHitWindow());
                ImGui::Text("%s", hitWindow);

                // Song HP drain
                ImGui::TableNextColumn();
                ImGui::Image((ImTextureID) drain->getID(), {10, 10}, {1, 1}, {0, 0});
                ImGui::SameLine();
                char hpDrain[8];
                sprintf(hpDrain, "%.1f", map->getHpDrain());
                ImGui::Text("%s", hpDrain);

                // Song duration
                ImGui::TableNextColumn();
                ImGui::Image((ImTextureID) duration->getID(), {10, 10}, {1, 1}, {0, 0});
                ImGui::SameLine();

                auto secsTotal = (int) map->getMapDuration();
                int secs = secsTotal % 60;
                int minutes = secsTotal / 60;
                int hours = secsTotal / 3600;

                char mapDuration[32];
                if (hours == 0)
                    sprintf(mapDuration, "%i:%02i", minutes, secs);
                else
                    sprintf(mapDuration, "%i:%02i:%02i", hours, minutes, secs);

                ImGui::Text("%s", mapDuration);
                ImGui::TableNextRow();
            }
        }
        ImGui::EndTable();
        ImGui::Text("%s", ctx.locale["ui.main.maps.filter"].c_str());
        ImGui::SameLine();
        filter.Draw("##", -1);
        ImGui::Separator();
    }

    if (ImGui::Button(ctx.locale["ui.main.maps.reload"].c_str())) {
        ctx.maps.clear();
        auto total = ctx.resources.purgeUnusedFiles();
        ctx.resources.getDirectory("songs", ctx.maps);
    }

    ImGui::SameLine();
    if (ImGui::Button(ctx.locale["ui.main.maps.exit"].c_str()))
        ctx.state.exit();
}

void State<GameState::MainMenu>::iterateSettingsSearch(
    std::unordered_map<std::string, std::string> &entries, const df2 &clump, const std::string &previousName)
{
    std::string thisName = previousName + clump.name();

    for (const auto &setting: clump) {
        const auto &value = setting.second;
        auto push = [&](const std::string &value)
        {
            entries[thisName + '.' + setting.first] = value;
        };

        std::string strRepr;
        switch (setting.second.getType()) {
            case df2::EntryType::Clump:
                iterateSettingsSearch(entries, setting.second, thisName);
                break;
            case df2::EntryType::Color:
                push(std::string("#")
                         + std::to_string(value.col().r) + ','
                         + std::to_string(value.col().g) + ','
                         + std::to_string(value.col().b) + ','
                         + std::to_string(value.col().a));
                break;
            case df2::EntryType::Vector:
                push(std::string("(") + std::to_string(value.vec().x) + ';' + std::to_string(value.vec().y) + ')');
                break;

            case df2::EntryType::Integer:
                push(std::to_string(value.integer()));
                break;
            case df2::EntryType::Real:
                push(std::to_string(value.real()));
                break;
            case df2::EntryType::Boolean:
                push(value.boolean() ? "true" : "false");
                break;
            case df2::EntryType::String:
                push(value.str());
                break;
            default:
                break;
        }
    }
}

void State<GameState::MainMenu>::showSettingsTab()
{
    static ImGuiTextFilter filter;

    std::unordered_map<std::string, std::string> entries;
    iterateSettingsSearch(entries, ctx.settings, "setting.");

    if (ImGui::Button(ctx.locale["ui.main.settings.read"].c_str())) {
        ctx.settings = df2::read(CONFIG_PATH);
    }
    ImGui::SameLine();
    if (ImGui::Button(ctx.locale["ui.main.settings.write"].c_str())) {
        df2::write(ctx.settings, CONFIG_PATH);
    }
    ImGui::SameLine();
    ImGui::Text("%s", ctx.locale["ui.main.settings.filter"].c_str());
    ImGui::SameLine();
    filter.Draw("##", -1);
    ImGui::Separator();

    if (ImGui::BeginTable("##", 2, ImGuiTableFlags_ScrollY, {0.0f, -1.0f})) {
        ImGui::TableSetupColumn(ctx.locale["ui.main.settings.key"].c_str());
        ImGui::TableSetupColumn(ctx.locale["ui.main.settings.value"].c_str());

        ImGui::TableHeadersRow();
        for (const auto &entry: entries) {
            if (filter.PassFilter(entry.first.c_str())) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", ctx.locale[entry.first].c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", entry.second.c_str());
            }
        }

        ImGui::EndTable();
    }

}

NS_END