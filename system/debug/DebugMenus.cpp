#include "DebugMenus.hpp"

#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "Locale.hpp"
#include "Tasks.hpp"

namespace PROJECT_NAMESPACE
{

void DrawDebugMenu()
{
    static bool showVersion = false;
    static bool showImGuiVersion = false;
    static bool showImGuiDebugger = false;
    static bool showImGuiLogger = false;
    static bool showImGuiStack = false;

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
            // auto loaded = ctx->maps.size();
            // auto remaining = ctx->maps.remaining();
            // ImGui::Text("loaded maps: %lu", loaded);
            // ImGui::Text("remaining: %lu", remaining);
            // ImGui::Text("total: %lu", loaded+remaining);
            // ImGui::Separator();
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

}