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

#include "StateMainMenu.hpp"

#include "Log.hpp"

#include "Import.hpp"
#include "Tasks.hpp"

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace PROJECT_NAMESPACE {

void State<GameState::MainMenu>::showDebugControl()
{
    static std::vector<compat::OsuInstallation> installs;
    static bool showVersion = false;
    static bool showImportDialog = false;
    static bool showImGuiVersion = false;
    static bool showImGuiDebugger = false;
    static bool showImGuiLogger = false;
    static bool showImGuiStack = false;

    static char inputPathBuffer[1024];

    if (showVersion) {
        if (ImGui::Begin(
            "ui.main.version.title"_i18n.c_str(),
            &showVersion,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse
        )) {
            ImGui::Text("%s", TOSTRING(GAME_TITLE));
            ImGui::Separator();
            ImGui::Text(
                "ui.main.version.version"_i18n.c_str(),
                VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, PLATFORM, ARCH
            );
            ImGui::Text("ui.main.version.build"_i18n.c_str(), BUILD_TYPE, BUILD_DATE, BUILD_TIME);
            ImGui::Separator();
            ImGui::Text("Copyright (c) 2022-2025 s1Jh");
        }
        ImGui::End();
    }

    if (showImportDialog) {
        if (ImGui::Begin("ui.main.import.title"_i18n.c_str(), &showImportDialog, ImGuiWindowFlags_AlwaysAutoResize)) {
            if (installs.empty()) {
                ImGui::InputTextWithHint("##import_file", "Enter path to osu!", inputPathBuffer, 1024);
                if (ImGui::Button("Import")) {

                    log::Info("Importing from ", inputPathBuffer);
                    showImportDialog = false;
                }
            } else {
                if (ImGui::BeginListBox("Select installation")) {
                    for (const auto &install : installs) {
                        if (ImGui::Selectable((const char *) install.path.c_str())) {
                            log::Info("Importing from ", install.path);
                            showImportDialog = false;
                            compat::ImportOsuData(install.path);
                            ctx->maps.clear();
                            ctx->maps.load(ctx->paths);
                        }
                    }
                    ImGui::EndListBox();
                }

            }
        }
        ImGui::End();
    }

    if (showImGuiVersion) {
        ImGui::ShowAboutWindow(&showImGuiVersion);
    }
    if (showImGuiLogger) {
        ImGui::ShowDebugLogWindow(&showImGuiLogger);
    }
    if (showImGuiDebugger) {
        ImGui::ShowMetricsWindow(&showImGuiDebugger);
    }
    if (showImGuiStack) {
        ImGui::ShowStackToolWindow(&showImGuiStack);
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("ui.main.about.title"_i18n.c_str())) {
            ImGui::MenuItem("ui.main.about.game_version"_i18n.c_str(), nullptr, &showVersion);
            ImGui::MenuItem("ui.main.about.imgui_version"_i18n.c_str(), nullptr, &showImGuiVersion);
            if (ImGui::MenuItem("ui.main.about.import"_i18n.c_str(), nullptr, &showImportDialog)) {
                installs = compat::FindOsuInstallations();
                log::Info("Found ", installs.size(), " osu! installations");
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("ui.main.debug.title"_i18n.c_str())) {
            if (ImGui::BeginMenu("ui.main.debug.imgui"_i18n.c_str())) {
                if (ImGui::MenuItem("ui.main.debug.imgui.debugger"_i18n.c_str())) {
                    showImGuiDebugger = true;
                }
                if (ImGui::MenuItem("ui.main.debug.imgui.logger"_i18n.c_str())) {
                    showImGuiLogger = true;
                }
                if (ImGui::MenuItem("ui.main.debug.imgui.stack"_i18n.c_str())) {
                    showImGuiStack = true;
                }
                if (ImGui::BeginMenu("ui.main.debug.imgui.theme"_i18n.c_str())) {
                    if (ImGui::MenuItem("ui.main.debug.imgui.theme.light"_i18n.c_str())) {
                        ImGui::StyleColorsLight();
                    }
                    if (ImGui::MenuItem("ui.main.debug.imgui.theme.dark"_i18n.c_str())) {
                        ImGui::StyleColorsDark();
                    }
                    if (ImGui::MenuItem("ui.main.debug.imgui.theme.classic"_i18n.c_str())) {
                        ImGui::StyleColorsClassic();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (ImGui::BeginTabBar("#tabs")) {

        if (ImGui::BeginTabItem("ui.main.maps.title"_i18n.c_str())) {
            showMainMenuTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ui.main.settings.title"_i18n.c_str())) {
            showSettingsTab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ui.main.locales.title"_i18n.c_str())) {
            auto& loc = *util::DefaultLocale();
            ImGui::Text("Locale: %s", loc.getLocName().c_str());
            ImGui::Text("Author: %s", loc.getLocCredits().c_str());
            ImGui::Text("Date: %s", loc.getDateLocale().c_str());
            ImGui::Text("Time: %s", loc.getTimeLocale().c_str());
            ImGui::Text("Separator: %s", loc.getDecimalSeparator().c_str());
            ImGui::Separator();
            if (ImGui::BeginTable("##localeinfo", 2)) {
                ImGui::TableSetupColumn("Key");
                ImGui::TableSetupColumn("Value");
                ImGui::TableHeadersRow();
                for (auto& entry : loc) {
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", entry.first.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", entry.second.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ui.main.misc.title"_i18n.c_str())) {
            auto loaded = ctx->maps.size();
            auto remaining = ctx->maps.remaining();
            ImGui::Text("loaded maps: %lu", loaded);
            ImGui::Text("remaining: %lu", remaining);
            ImGui::Text("total: %lu", loaded+remaining);
            ImGui::Separator();
            ImGui::Text("active tasks: %lu", tasks::GetTaskQueueLength());
            auto infos = tasks::GetThreadStates();
            if (ImGui::BeginTable("##threadinfo", 4)) {
                for (size_t i = 0; i < infos.size(); i++) {
                    const auto& info = infos[i];

                    ImGui::Text("%lu", i);
                    ImGui::TableNextColumn();
                    ImGui::Text("PID: %lX", info.id);
                    ImGui::TableNextColumn();
                    switch (info.state) {
                        case tasks::ThreadState::Inactive:
                            ImGui::TextColored({1.0f, 1.0f, 0.0f, 1.0f}, "Inactive");
                            break;
                        case tasks::ThreadState::Sleeping:
                            ImGui::TextColored({0.6f, 0.6f, 1.0f, 1.0f}, "Sleeping");
                            break;
                        case tasks::ThreadState::Running:
                            ImGui::TextColored({1.0f, 0.6f, 0.6f, 1.0f}, "Running");
                            break;
                        case tasks::ThreadState::Dead:
                            ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, "Dead");
                            break;
                        default:
                            ImGui::TextColored({1.0f, 1.0f, 1.0f, 1.0f}, "Unknown");
                            break;
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text("%llX", info.taskPtr);
                    ImGui::TableNextRow();
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

int State<GameState::MainMenu>::update(double)
{
    ctx->maps.update();

    if (ctx->keyboard[input::Key::F1].releasing) {
        showDebugCtrl = !showDebugCtrl;
    }
    if (ctx->keyboard[input::Key::F2].releasing) {
        auto skinSetting = ctx->settings.getSetting<std::string>("setting.user.skin");
        auto path = ctx->paths.find(skinSetting.get(), Resource<Skin>::allowedExtensions);
        ctx->activeSkin = Load<Skin>(path);
        ctx->gui.create(ctx->activeSkin->getGuiMarkup());
    }

    return 0;
}

int State<GameState::MainMenu>::draw()
{
    if (showDebugCtrl) {
        ctx->gfx.draw(
            ImGuiWindow{[&]()
            { showDebugControl(); }, "ui.main.title"_i18n, &showDebugCtrl, ImGuiWindowFlags_MenuBar}
        );
    }
    return 0;
}

int State<GameState::MainMenu>::exit()
{
    return 0;
}

int State<GameState::MainMenu>::init(GameState)
{
    return 0;
}

void State<GameState::MainMenu>::showMainMenuTab()
{
    // FIXME: This doesn't refresh when skin changes
    static auto star = ctx->activeSkin->getTexture("star_icon");
    static auto difficulty = ctx->activeSkin->getTexture("difficulty_icon");
    static auto approach = ctx->activeSkin->getTexture("approach_rate_icon");
    static auto circle = ctx->activeSkin->getTexture("note_icon");
    static auto window = ctx->activeSkin->getTexture("hit_window_icon");
    static auto drain = ctx->activeSkin->getTexture("hp_drain_icon");
    static auto duration = ctx->activeSkin->getTexture("duration_icon");

    static unsigned int selected = -1;

    static ImGuiTextFilter filter;

    const float playButtonWidth = 60.f;
    auto size = (fsize) ctx->gfx.getConfig().size;

    if (ImGui::Button("ui.main.maps.exit"_i18n.c_str())) {
        stop();
    }
    ImGui::SameLine();
    if (ImGui::Button("ui.main.maps.reload"_i18n.c_str())) {
        ctx->maps.clear();
        ctx->maps.load(ctx->paths);
    }
    ImGui::SameLine();
    ImGui::Text("%s", "ui.main.maps.filter"_i18n.c_str());
    ImGui::SameLine();
    filter.Draw("##", -1);
    ImGui::Separator();

    if (ImGui::BeginTable(
        "##", 9,
        ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
        {0.0, size.h * 0.75f}
    )) {
        ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(
            "ui.main.maps.star_rating"_i18n.c_str(),
            ImGuiTableColumnFlags_WidthFixed, playButtonWidth
        );
        ImGui::TableSetupColumn("ui.main.maps.name"_i18n.c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(
            "ui.main.maps.overall_difficulty"_i18n.c_str(),
            ImGuiTableColumnFlags_WidthFixed, playButtonWidth
        );
        ImGui::TableSetupColumn(
            "ui.main.maps.approach_rate"_i18n.c_str(),
            ImGuiTableColumnFlags_WidthFixed, playButtonWidth
        );
        ImGui::TableSetupColumn(
            "ui.main.maps.circle_size"_i18n.c_str(),
            ImGuiTableColumnFlags_WidthFixed, playButtonWidth
        );
        ImGui::TableSetupColumn(
            "ui.main.maps.hit_window"_i18n.c_str(),
            ImGuiTableColumnFlags_WidthFixed, playButtonWidth
        );
        ImGui::TableSetupColumn(
            "ui.main.maps.hp_drain"_i18n.c_str(),
            ImGuiTableColumnFlags_WidthFixed, playButtonWidth
        );
        ImGui::TableSetupColumn(
            "ui.main.maps.length"_i18n.c_str(),
            ImGuiTableColumnFlags_WidthFixed, playButtonWidth
        );

        ImGui::TableHeadersRow();

        const float iconSize = 16;

        for (unsigned int i = 0; i < ctx->maps.size(); i++) {
            const auto &map = ctx->maps.at(i);

            auto filterName = map->romanisedName + " " + map->romanisedArtist + " " + map->difficulty;

            if (filter.PassFilter(filterName.c_str())) {

                // The play button
                ImGui::TableNextColumn();
                if (i == selected) {
                    if (ImGui::Button("Play!", {playButtonWidth, 0})) {
                        ctx->game.setMap(map);
                        setState(GameState::InGame);
                    }
                }
                // Song star rating
                ImGui::TableNextColumn();
                char starRating[8] = "???";
                ImGui::Text("%s", starRating);
                ImGui::SameLine();
                ImGui::Image(star->getGLData(), {iconSize, iconSize}, {0, 1}, {1, 0}, {0.8, 0.8, 0, 1});

                // Song name and selectable field
                ImGui::TableNextColumn();
                std::string itemID = "##" + std::to_string(i);
                if (ImGui::Selectable(itemID.c_str(), false, ImGuiSelectableFlags_None)) {
                    selectedMap = map;
                    auto &channel = ctx->audio.getMusicChannel();
                    radio = Load<SoundStream>(map->getDirectory() / map->songPath);
                    channel.setSound(radio.ref(), true);
                    auto bg = ctx->menuBG.lock();
                    if (bg) {
                        bg->setImageBackground(Load<video::Texture>(map->getDirectory() / map->backgroundPath));
                    }
                    selected = i;
                }
                ImGui::SameLine();
                auto name =
                    map->romanisedName + " - " + map->romanisedArtist + " (" + map->difficulty + ')';
                if (name.empty()) {
                    name = "#unknown_" + std::to_string(long(map.get()));
                }
                ImGui::Text("%s", name.c_str());

                // Song overall difficulty
                ImGui::TableNextColumn();
                ImGui::Image(difficulty->getGLData(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char overallDiff[8];
                sprintf(overallDiff, "%.1f", map->overallDifficulty);
                ImGui::Text("%s", overallDiff);

                // Song approach time
                ImGui::TableNextColumn();
                ImGui::Image(approach->getGLData(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char approachTime[8];
                sprintf(approachTime, "%.1f", map->approachTime);
                ImGui::Text("%s", approachTime);

                // Song circle size
                ImGui::TableNextColumn();
                ImGui::Image(circle->getGLData(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char circleSize[8];
                sprintf(circleSize, "%.2f", map->circleSize);
                ImGui::Text("%s", circleSize);

                // Song hit window time
                ImGui::TableNextColumn();
                ImGui::Image(window->getGLData(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char hitWindow[8];
                sprintf(hitWindow, "%.1f", map->hitWindow);
                ImGui::Text("%s", hitWindow);

                // Song HP drain
                ImGui::TableNextColumn();
                ImGui::Image(drain->getGLData(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char hpDrain[8];
                sprintf(hpDrain, "%.1f", map->HPDrain);
                ImGui::Text("%s", hpDrain);

                // Song duration
                ImGui::TableNextColumn();
                ImGui::Image(duration->getGLData(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();

                auto secsTotal = (int) map->mapDuration;
                int secs = secsTotal % 60;
                int minutes = secsTotal / 60;
                int hours = secsTotal / 3600;

                char mapDuration[32];
                if (hours == 0) {
                    sprintf(mapDuration, "%i:%02i", minutes, secs);
                } else {
                    sprintf(mapDuration, "%i:%02i:%02i", hours, minutes, secs);
                }

                ImGui::Text("%s", mapDuration);
                ImGui::TableNextRow();
            }
        }
        ImGui::EndTable();
    }
}

void State<GameState::MainMenu>::showSettingsTab()
{
    static ImGuiTextFilter filter;

    if (ImGui::Button("ui.main.settings.read"_i18n.c_str())) {
        ctx->settings.read();
    }
    ImGui::SameLine();
    if (ImGui::Button("ui.main.settings.write"_i18n.c_str())) {
        ctx->settings.write();
    }
    ImGui::SameLine();
    if (ImGui::Button("ui.main.settings.apply"_i18n.c_str())) {
        ctx->settings.apply();
    }
    ImGui::SameLine();
    ImGui::Text("%s", "ui.main.settings.filter"_i18n.c_str());
    ImGui::SameLine();
    filter.Draw("##_F", -1);
    ImGui::Separator();

    if (ImGui::BeginTable("##", 2, ImGuiTableFlags_ScrollY, {0.0f, -1.0f})) {
        ImGui::TableSetupColumn("ui.main.settings.input::Key"_i18n.c_str());
        ImGui::TableSetupColumn("ui.main.settings.value"_i18n.c_str());

        ImGui::TableHeadersRow();
        auto &locale = util::DefaultLocale();
        for (auto &setting : ctx->settings) {

            auto &ptr = setting.second;
            if (bool(ptr->flags() & SettingFlags::HIDDEN)) {
                continue;
            }

            auto localised = locale->getTranslation(setting.first);
            if (!filter.PassFilter(localised.c_str())) {
                continue;
            }

            ImGui::TableNextColumn();
            ImGui::Text("%s", localised.c_str());
            ImGui::TableNextColumn();

            std::string id = "##" + setting.first;

            if (VisitSetting<std::string>(
                ptr, wrap(
                    [&](Setting<std::string> &setting)
                    {
                        if (setting.getMetadata().options.empty()) {
                            ImGui::InputText(id.c_str(), setting.getPtr());
                            return;
                        }

                        if (ImGui::BeginListBox(id.c_str())) {
                            for (const auto &option : setting.getMetadata().options) {
                                if (ImGui::Selectable(option.c_str(), option == setting.get())) {
                                    setting.set(option);
                                }
                            }
                            ImGui::EndListBox();
                        }

                    }
                ))) {
                continue;
            }

            if (VisitSetting<float>(
                ptr, wrap(
                    [&](Setting<float> &setting)
                    {
                        auto meta = setting.getMetadata();
                        float val = setting.get();
                        ImGui::SliderFloat(id.c_str(), &val, meta.min, meta.max);
                        setting.set(val);
                    }
                ))) {
                continue;
            }

            if (VisitSetting<int>(
                ptr, wrap(
                    [&](Setting<int> &setting)
                    {
                        auto meta = setting.getMetadata();
                        int span = meta.max - meta.min;
                        int val = setting.get();
                        ImGui::InputInt(id.c_str(), &val, 1, span / 10);
                        setting.set(val);
                    }
                ))) {
                continue;
            }

            if (VisitSetting<color>(
                ptr, wrap(
                    [&](Setting<color> &setting)
                    {
                        if (setting.getMetadata().palette.empty()) {
                            color val = setting.get();
                            ImGui::ColorEdit4(id.c_str(), (float *) &val);
                            setting.set(val);
                            return;
                        }
                        if (ImGui::BeginListBox(id.c_str())) {
                            for (const color &option : setting.getMetadata().palette) {
                                color8 truncated = option;
                                std::string name = ToString(truncated);

                                ImVec4 color;
                                color.x = option.r;
                                color.y = option.g;
                                color.z = option.b;
                                color.w = option.a;

                                if (ImGui::Selectable(("##" + name).c_str(), option == setting.get())) {
                                    setting.set(option);
                                }
                                ImGui::SameLine();
                                ImGui::TextColored(color, "%s", name.c_str());
                            }
                            ImGui::EndListBox();
                        }
                    }
                ))) {
                continue;
            }

            if (VisitSetting<bool>(
                ptr, wrap(
                    [&](Setting<bool> &setting)
                    {
                        bool val = setting.get();
                        ImGui::Checkbox(id.c_str(), &val);
                        setting.set(val);
                    }
                ))) {
                continue;
            }
        }

        ImGui::EndTable();
    }
}

}