#pragma once

#include "define.hpp"
#include "bases/BaseObjectTemplate.hpp"
#include "Vec2.hpp"
#include "SliderTypes.h"

#include <memory>
#include <list>
#include <filesystem>

namespace GAME_TITLE {

    class MapInfo {
    public:
        using StorageT = std::list<std::shared_ptr<BaseObjectTemplate>>;

        bool loadMap(const std::filesystem::path &path, LoadLayers layers);

        [[nodiscard]] const StorageT &getObjectTemplates() const;

        [[nodiscard]] const std::string &getName() const;

        [[nodiscard]] const std::string &getDescription() const;

        [[nodiscard]] const std::string &getArtist() const;

        [[nodiscard]] const std::string &getDifficulty() const;

        [[nodiscard]] const std::string &getSongPath() const;

        [[nodiscard]] float getCircleSize() const;

        [[nodiscard]] float getStartOffset() const;

        [[nodiscard]] double getMapDuration() const;

        [[nodiscard]] float getHpDrain() const;

        [[nodiscard]] float getApproachTime() const;

        [[nodiscard]] float getHitWindow() const;

        [[nodiscard]] float getOverallDifficulty() const;

        [[nodiscard]] float getFadeTime() const;

        void addNote(fvec2d position, bool comboEnd, float time);

        void addSlider(const SliderPathT &points, bool comboEnd, float time, float endTime, SliderType type);

        void addSpinner(float spinRequired, float spinResistance, float time);

    private:
        void readObject(float &elementTime, char type, std::stringstream &line);

        void readMeta(char type, std::stringstream &line);

        void readEvent(float &elementTime, char type, std::stringstream &line);

        void insertElement(std::shared_ptr<BaseObjectTemplate>);

        static void readString(std::stringstream &from, std::string &to);

        // Song's name.
        std::string name;
        // Description of the song.
        std::string description;
        // Artist's name.
        std::string artist;
        // This difficulty's name.
        std::string difficulty;
        // Path to the song's audio file.
        std::string songPath;
        // Size of a circle in play field unit size.
        float circleSize = 0.2f;
        // Audio file offset.
        float startOffset = 0.0f;
        // Total length of the map, the map will end automatically after this amount of startTime has elapsed.
        double mapDuration = 120.0;
        // HP drainage per second.
        float HPDrain = 1.0f;
        // How long hit objects appear on the screen for before their startTime.
        float approachTime = 1.0f;
        // The window during which the object can be hit.
        float hitWindow = 0.3f;
        // How long the objects linger on the screen after they've been hit.
        float fadeTime = 0.2f;
        // The star difficulty of the map, used for display purposes only.
        float overallDifficulty = 0.0f;
        StorageT objectTemplates;
    };

}

