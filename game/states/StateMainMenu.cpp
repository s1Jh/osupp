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
#include "Texture.hpp"

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
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
    static bool showLocalisations = false;
    static bool showImGuiVersion = false;
    static bool showImGuiDebugger = false;
    static bool showImGuiLogger = false;
    static bool showImGuiStack = false;
	static bool showResourceDebug = false;

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
    if (showImGuiLogger)
        ImGui::ShowDebugLogWindow(&showImGuiLogger);
    if (showImGuiDebugger)
        ImGui::ShowMetricsWindow(&showImGuiDebugger);
    if (showImGuiStack)
        ImGui::ShowStackToolWindow(&showImGuiStack);

    if (showAssetPurge) {
        if (ImGui::Begin(ctx.locale["ui.main.purge.title"].c_str(),
                         &showAssetPurge,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Modal)) {
            ImGui::Text(ctx.locale["ui.main.purge.message"].c_str(), purged);
            ImGui::End();
        }
    }
    if (showLocalisations) {
        ctx.locale.showDebugListings(&showLocalisations);
    }

	if (showResourceDebug) {
		ctx.resources.drawDebugDialog(&showResourceDebug);
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
            if (ImGui::MenuItem(ctx.locale["ui.main.debug.localisations"].c_str())) {
                showLocalisations = true;
            }
			if (ImGui::MenuItem(ctx.locale["ui.main.debug.resources"].c_str())) {
				showResourceDebug = true;
			}

            if (ImGui::BeginMenu(ctx.locale["ui.main.debug.imgui"].c_str())) {
                if (ImGui::MenuItem(ctx.locale["ui.main.debug.imgui.debugger"].c_str())) {
                    showImGuiDebugger = true;
                }
                if (ImGui::MenuItem(ctx.locale["ui.main.debug.imgui.logger"].c_str())) {
                    showImGuiLogger = true;
                }
                if (ImGui::MenuItem(ctx.locale["ui.main.debug.imgui.stack"].c_str())) {
                    showImGuiStack = true;
                }
                if (ImGui::BeginMenu(ctx.locale["ui.main.debug.imgui.theme"].c_str())) {
                    if (ImGui::MenuItem(ctx.locale["ui.main.debug.imgui.theme.light"].c_str())) {
                        ImGui::StyleColorsLight();
                    }
                    if (ImGui::MenuItem(ctx.locale["ui.main.debug.imgui.theme.dark"].c_str())) {
                        ImGui::StyleColorsDark();
                    }
                    if (ImGui::MenuItem(ctx.locale["ui.main.debug.imgui.theme.classic"].c_str())) {
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

int State<GameState::MainMenu>::update(double)
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

int State<GameState::MainMenu>::init(GameState)
{
    return 0;
}

void State<GameState::MainMenu>::showMainMenuTab()
{
    // FIXME: This doesn't properly work with skins
    static auto star = GetContext().resources.get<Texture>("star_icon");
    static auto difficulty = GetContext().resources.get<Texture>("difficulty_icon");
    static auto approach = GetContext().resources.get<Texture>("approach_rate_icon");
    static auto circle = GetContext().resources.get<Texture>("note_icon");
    static auto window = GetContext().resources.get<Texture>("hit_window_icon");
    static auto drain = GetContext().resources.get<Texture>("hp_drain_icon");
    static auto duration = GetContext().resources.get<Texture>("duration_icon");

    static unsigned int selected = -1;

    static ImGuiTextFilter filter;

    const float playButtonWidth = 60.f;
    auto size = (fsize) GetContext().gfx.getSize();

    if (ImGui::Button(ctx.locale["ui.main.maps.reload"].c_str())) {
        ctx.maps.clear();
        ctx.resources.purgeUnusedFiles();
        ctx.resources.getDirectory("songs", ctx.maps);
    }

    ImGui::SameLine();
    if (ImGui::Button(ctx.locale["ui.main.maps.exit"].c_str()))
        ctx.state.exit();
    ImGui::SameLine();
    ImGui::Text("%s", ctx.locale["ui.main.maps.filter"].c_str());
    ImGui::SameLine();
    filter.Draw("##", -1);
    ImGui::Separator();

    if (ImGui::BeginTable("##", 9,
                          ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                          {0.0, size.h * 0.75f})) {
        ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.star_rating"].c_str(),
                                ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.name"].c_str(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.overall_difficulty"].c_str(),
                                ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.approach_rate"].c_str(),
                                ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.circle_size"].c_str(),
                                ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.hit_window"].c_str(),
                                ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.hp_drain"].c_str(),
                                ImGuiTableColumnFlags_WidthFixed, playButtonWidth);
        ImGui::TableSetupColumn(ctx.locale["ui.main.maps.length"].c_str(),
                                ImGuiTableColumnFlags_WidthFixed, playButtonWidth);

        ImGui::TableHeadersRow();

        const float iconSize = 16;

        for (unsigned int i = 0; i < ctx.maps.size(); i++) {
            const auto &map = ctx.maps[i];

            auto filterName = map->getRomanisedName() + " " + map->getRomanisedArtist();

            if (filter.PassFilter(filterName.c_str())) {

                // The play button
                ImGui::TableNextColumn();
                if (i == selected) {
                    if (ImGui::Button("Play!", {playButtonWidth, 0})) {
                        ctx.game.setMap(map);
                        ctx.state.setState(GameState::InGame);
                    }
                }
                // Song star rating
                ImGui::TableNextColumn();
                char starRating[8] = "???";
                ImGui::Text("%s", starRating);
                ImGui::SameLine();
                ImGui::Image(star->getID(), {iconSize, iconSize}, {0, 1}, {1, 0}, {0.8, 0.8, 0, 1});

                // Song name and selectable field
                ImGui::TableNextColumn();
                std::string itemID = "##" + std::to_string(i);
                if (ImGui::Selectable(itemID.c_str(), false, ImGuiSelectableFlags_None)) {
					selectedMap = map;
					auto& channel = ctx.audio.getMusicChannel();
					auto music = ctx.resources.get<SoundStream>(map->getSongPath(), map->getDirectory(), false);
					channel.setSound(music.ref(), true);
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
                ImGui::Image(difficulty->getID(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char overallDiff[8];
                sprintf(overallDiff, "%.1f", map->getOverallDifficulty());
                ImGui::Text("%s", overallDiff);

                // Song approach time
                ImGui::TableNextColumn();
                ImGui::Image(approach->getID(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char approachTime[8];
                sprintf(approachTime, "%.1f", map->getApproachTime());
                ImGui::Text("%s", approachTime);

                // Song circle size
                ImGui::TableNextColumn();
                ImGui::Image(circle->getID(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char circleSize[8];
                sprintf(circleSize, "%.2f", map->getCircleSize());
                ImGui::Text("%s", circleSize);

                // Song hit window time
                ImGui::TableNextColumn();
                ImGui::Image(window->getID(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char hitWindow[8];
                sprintf(hitWindow, "%.1f", map->getHitWindow());
                ImGui::Text("%s", hitWindow);

                // Song HP drain
                ImGui::TableNextColumn();
                ImGui::Image(drain->getID(), {iconSize, iconSize}, {0, 1}, {1, 0});
                ImGui::SameLine();
                char hpDrain[8];
                sprintf(hpDrain, "%.1f", map->getHpDrain());
                ImGui::Text("%s", hpDrain);

                // Song duration
                ImGui::TableNextColumn();
                ImGui::Image(duration->getID(), {iconSize, iconSize}, {0, 1}, {1, 0});
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
    }
}

void State<GameState::MainMenu>::showSettingsTab()
{
    static ImGuiTextFilter filter;

    if (ImGui::Button(ctx.locale["ui.main.settings.read"].c_str())) {
        ctx.settings.read();
    }
    ImGui::SameLine();
    if (ImGui::Button(ctx.locale["ui.main.settings.write"].c_str())) {
        ctx.settings.write();
    }
    ImGui::SameLine();
    if (ImGui::Button(ctx.locale["ui.main.settings.apply"].c_str())) {
        ctx.settings.apply();
    }
    ImGui::SameLine();
    ImGui::Text("%s", ctx.locale["ui.main.settings.filter"].c_str());
    ImGui::SameLine();
    filter.Draw("##_F", -1);
    ImGui::Separator();

    if (ImGui::BeginTable("##", 2, ImGuiTableFlags_ScrollY, {0.0f, -1.0f})) {
        ImGui::TableSetupColumn(ctx.locale["ui.main.settings.key"].c_str());
        ImGui::TableSetupColumn(ctx.locale["ui.main.settings.value"].c_str());

        ImGui::TableHeadersRow();
        for (auto &setting: ctx.settings) {
            auto localised = ctx.locale[setting.first];
            if (!filter.PassFilter(localised.c_str())) {
                continue;
            }
            ImGui::TableNextColumn();
            ImGui::Text("%s", localised.c_str());
            ImGui::TableNextColumn();

            auto &ptr = setting.second;
            std::string id = "##" + setting.first;

			if (VisitSetting<std::string>(ptr, wrap([&](Setting<std::string>& setting) {
				if (setting.getMetadata().options.empty()) {
					ImGui::InputText(id.c_str(), setting.getPtr());
					return;
				}

				if (ImGui::BeginListBox(id.c_str())) {
					for (const auto &option: setting.getMetadata().options) {
						if (ImGui::Selectable(option.c_str(), option == setting.get())) {
							setting.set(option);
						}
					}
					ImGui::EndListBox();
				}

			}))) continue;

			if (VisitSetting<float>(ptr, wrap([&](Setting<float>& setting) {
				auto meta = setting.getMetadata();
				float val = setting.get();
				ImGui::SliderFloat(id.c_str(), &val, meta.min, meta.max);
				setting.set(val);
			}))) continue;
			
			if (VisitSetting<int>(ptr, wrap([&](Setting<int>& setting) {
				auto meta = setting.getMetadata();
				int span = meta.max - meta.min;
				int val = setting.get();
				ImGui::InputInt(id.c_str(), &val, 1, span / 10);
				setting.set(val);
			}))) continue;

			if (VisitSetting<color>(ptr, wrap([&](Setting<color>& setting) {
				if (setting.getMetadata().palette.empty()) {
					color val = setting.get();
					ImGui::ColorEdit4(id.c_str(), (float *) &val);
					setting.set(val);
					return;
				}
				if (ImGui::BeginListBox(id.c_str())) {
					for (const color &option: setting.getMetadata().palette) {
						color8 truncated = option;
						std::string name = (std::string) truncated;

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
			}))) continue;

			if (VisitSetting<bool>(ptr, wrap([&](Setting<bool>& setting) {
				bool val = setting.get();
				ImGui::Checkbox(id.c_str(), &val);
				setting.set(val);
			}))) continue;
        }

        ImGui::EndTable();
    }
}

NS_END