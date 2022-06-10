#include "MapInfo.hpp"

#include "Util.hpp"
#include "HitObjects.hpp"

#include <fstream>

NS_BEGIN

const MapInfo::StorageT &MapInfo::getObjectTemplates() const {
    return objectTemplates;
}

void MapInfo::addNote(fvec2d position, bool comboEnd, float time) {
    auto object = std::make_shared<ObjectTemplateNote>();
    object->startTime = time;
    object->endTime = time;
    object->position = position;
    object->parameters |= comboEnd ? HitObjectParams::ComboEnd : OBJECT_NO_PARAMS;
    insertElement(object);
}

void MapInfo::addSlider(const SliderPathT &points, bool comboEnd, float time, float endTime, CurveType type,
                        unsigned int repeats) {
    auto object = std::make_shared<ObjectTemplateSlider>();
    object->startTime = time;
    object->path = points;
    object->endTime = endTime;
    object->sliderType = type;
    object->repeats = Max(repeats, 1);
    insertElement(object);
}

void MapInfo::addSpinner(float spinRequired, float spinResistance, float time, float endTime) {
    auto object = std::make_shared<ObjectTemplateSpinner>();
    object->startTime = time;
    object->spinRequired = spinRequired;
    object->spinResistance = spinResistance;
    object->endTime = endTime;
    insertElement(object);
}

void MapInfo::insertElement(std::shared_ptr<BaseObjectTemplate> obj) {
    auto insertPoint = std::find_if(
            objectTemplates.begin(), objectTemplates.end(),
            [&obj](const std::shared_ptr<BaseObjectTemplate> &checked) {
                return checked->startTime <= obj->startTime;
            }
    );

    if (insertPoint == objectTemplates.end()) {
        objectTemplates.push_back(obj);
    } else {
        objectTemplates.insert(insertPoint, obj);
    }
}

const std::string &MapInfo::getName() const {
    return name;
}

const std::string &MapInfo::getDescription() const {
    return description;
}

const std::string &MapInfo::getArtist() const {
    return artist;
}

const std::string &MapInfo::getDifficulty() const {
    return difficulty;
}

const std::string &MapInfo::getSongPath() const {
    return songPath;
}

float MapInfo::getCircleSize() const {
    return circleSize;
}

float MapInfo::getStartOffset() const {
    return startOffset;
}

double MapInfo::getMapDuration() const {
    return mapDuration;
}

float MapInfo::getHpDrain() const {
    return HPDrain;
}

float MapInfo::getApproachTime() const {
    return approachTime;
}

float MapInfo::getHitWindow() const {
    return hitWindow;
}

float MapInfo::getOverallDifficulty() const {
    return overallDifficulty;
}

float MapInfo::getFadeTime() const {
    return fadeTime;
}

void MapInfo::clear() {
    objectTemplates.clear();
}

void MapInfo::setName(const std::string &name) {
    MapInfo::name = name;
}

void MapInfo::setDescription(const std::string &description) {
    MapInfo::description = description;
}

void MapInfo::setArtist(const std::string &artist) {
    MapInfo::artist = artist;
}

void MapInfo::setDifficulty(const std::string &difficulty) {
    MapInfo::difficulty = difficulty;
}

void MapInfo::setSongPath(const std::string &songPath) {
    MapInfo::songPath = songPath;
}

void MapInfo::setCircleSize(float circleSize) {
    MapInfo::circleSize = circleSize;
}

void MapInfo::setStartOffset(float startOffset) {
    MapInfo::startOffset = startOffset;
}

void MapInfo::setMapDuration(double mapDuration) {
    MapInfo::mapDuration = mapDuration;
}

void MapInfo::setHpDrain(float hpDrain) {
    HPDrain = hpDrain;
}

void MapInfo::setApproachTime(float approachTime) {
    MapInfo::approachTime = approachTime;
}

void MapInfo::setHitWindow(float hitWindow) {
    MapInfo::hitWindow = hitWindow;
}

void MapInfo::setFadeTime(float fadeTime) {
    MapInfo::fadeTime = fadeTime;
}

void MapInfo::setOverallDifficulty(float overallDifficulty) {
    MapInfo::overallDifficulty = overallDifficulty;
}

NS_END