#include "Locale.hpp"

#include "Util.hpp"
#include "df2.hpp"
#include "imgui/imgui.h"

NS_BEGIN

int Locale::loadFromFile(const std::filesystem::path &path)
{
    df2 parser = df2::read(path);

    auto &clump = parser["locale"];

    dateLocale = clump["date"].str("dd/MM/yyyy");
    timeLocale = clump["time"].str("HH:mm");
    decimalSeparator = clump["decimal_sep"].str(".");
    locName = clump["name"].str("unknown");
    locCredits = clump["credits"].str("unknown");

    auto &translationClumps = clump["translations"];
    for (auto &translationClump: translationClumps) {
        if (translationClump.second.getType() == df2::EntryType::String) {
            translations[translationClump.first] =
                std::move(translationClump.second.str());
//            log::debug("[LOCALE] Added locale entry ", translationClump.first);
        }
    }
    return 0;
}

const std::string &Locale::getTimeLocale() const
{ return timeLocale; }

const std::string &Locale::getDateLocale() const
{ return dateLocale; }

const std::string &Locale::getDecimalSeparator() const
{
    return decimalSeparator;
}

const std::string &Locale::getLocName() const
{ return locName; }

const std::string &Locale::getLocCredits() const
{ return locCredits; }

const std::string &Locale::getTranslation(const std::string &key) const
{
    if (translations.find(key) != translations.end())
        return translations.at(key);
    else {
        return key;
    }
}

const std::string &Locale::operator[](const std::string &key) const
{
    return getTranslation(key);
}
void Locale::showDebugListings(bool *open) const
{
    if (ImGui::Begin(getTranslation("ui.main.localisations.title").c_str(), open)) {
        ImGui::Text("%s", getTranslation("ui.main.localisations.caption").c_str());
        ImGui::Text(getTranslation("ui.main.localisations.name_credit").c_str(), locName.c_str(), locCredits.c_str());
        ImGui::Separator();
        ImGui::Text(getTranslation("ui.main.localisations.time").c_str(), timeLocale.c_str());
        ImGui::Text(getTranslation("ui.main.localisations.date").c_str(), dateLocale.c_str());
        ImGui::Text(getTranslation("ui.main.localisations.decimal").c_str(), decimalSeparator.c_str());
        ImGui::Separator();

        if (ImGui::BeginTable("##", 2, ImGuiTableFlags_ScrollY, {0.0f, -1.0f})) {
            ImGui::TableSetupColumn(getTranslation("ui.main.localisations.key").c_str());
            ImGui::TableSetupColumn(getTranslation("ui.main.localisations.value").c_str());

            ImGui::TableHeadersRow();
            for (const auto &entry: translations) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", entry.first.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", entry.second.c_str());
            }

            ImGui::EndTable();
        }
        ImGui::End();
    }
}

NS_END