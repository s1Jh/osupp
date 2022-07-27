#pragma once

#include "BaseObjectTemplate.hpp"
#include "Resource.hpp"
#include "SliderTypes.hpp"
#include "Vec2.hpp"
#include "define.hpp"

#include <filesystem>
#include <list>
#include <memory>

NS_BEGIN

class MapInfo: public detail::Resource
{
public:
    using StorageT = std::list<std::shared_ptr<BaseObjectTemplate>>;

    bool load(const std::filesystem::path &path) override;

    bool create() override;

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

    [[nodiscard]] const std::string &getRomanisedArtist() const;

    [[nodiscard]] const std::string &getRomanisedName() const;

    [[nodiscard]] const std::string &getSource() const;

    [[nodiscard]] const std::vector<std::string> &getTags() const;

    [[nodiscard]] const std::string &getAuthor() const;

    void setName(const std::string &name);

    void setDescription(const std::string &description);

    void setArtist(const std::string &artist);

    void setDifficulty(const std::string &difficulty);

    void setSongPath(const std::string &songPath);

    void setCircleSize(float circleSize);

    void setStartOffset(float startOffset);

    void setMapDuration(double mapDuration);

    void setHpDrain(float hpDrain);

    void setRomanisedName(const std::string &romanisedName);

    void setRomanisedArtist(const std::string &romanisedArtist);

    void setSource(const std::string &source);

    void setTags(const std::vector<std::string> &tags);

    void setApproachTime(float approachTime);

    void setHitWindow(float hitWindow);

    void setFadeTime(float fadeTime);

    void setOverallDifficulty(float overallDifficulty);

    void setAuthor(const std::string &author);

    void clear();

    void addNote(const fvec2d &position, bool comboEnd, double time);

    void addSlider(const SliderPathT &points, bool comboEnd, double time,
                   double endTime, CurveType type, unsigned int repeats = 1);

    void addSpinner(float spinRequired, float spinResistance, double time,
                    double endTime, const fvec2d &position = {0, 0});

private:
    void insertElement(std::shared_ptr<BaseObjectTemplate>);

    // Song's name.
    std::string name;
    // Song's name written out in latin.
    std::string romanisedName;
    // Description of the song.
    std::string description;
    // Artist's name.
    std::string artist;
    // Artist's name written out in latin.
    std::string romanisedArtist;
    // The place in which the song was originally used.
    std::string source;
    // The place in which the song was originally used.
    std::string author;

    // Tags to be used for searching.
    std::vector<std::string> tags;
    // This difficulty's name.
    std::string difficulty;
    // Path to the song's audio file.
    std::string songPath;
    // Size of a circle in play field unit size.
    float circleSize = 0.2f;
    // Audio file offset.
    float startOffset = 0.0f;
    // Total length of the map, the map will end automatically after this amount
    // of startTime has elapsed.
    double mapDuration = 120.0;
    // HP drainage per second.
    float HPDrain = 1.0f;
    // How long hit objects appear on the screen for before their startTime.
    float approachTime = 1.0f;
    // The window during which the object can be hit.
    float hitWindow = 0.3f;
    // How long the objects linger on the screen after they've been hit.
    float fadeTime = 0.5f;
    // The star difficulty of the map, used for display purposes only.
    float overallDifficulty = 0.0f;
    StorageT objectTemplates;
};

using MapInfoP = std::shared_ptr<MapInfo>;

NS_END