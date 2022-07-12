#include "MapInfo.hpp"

#include "HitObjects.hpp"
#include "MapLoaders.hpp"
#include "Util.hpp"

#include <fstream>

NS_BEGIN

const MapInfo::StorageT &MapInfo::getObjectTemplates() const
{
    return objectTemplates;
}

void MapInfo::addNote(fvec2d position, bool comboEnd, double time)
{
    LOG_ENTER();

    auto object = std::make_shared<ObjectTemplateNote>();
    object->startTime = time;
    object->endTime = time;
    object->position = position;
    object->parameters |= comboEnd ? HitObjectParams::ComboEnd : HitObjectParams::None;
    insertElement(object);
}

void MapInfo::addSlider(const SliderPathT &points, bool comboEnd, double time,
                        double endTime, CurveType type, unsigned int repeats)
{
    LOG_ENTER();

    auto object = std::make_shared<ObjectTemplateSlider>();
    object->startTime = time;
    object->parameters |= comboEnd ? HitObjectParams::ComboEnd : HitObjectParams::None;
    object->path = points;
    object->endTime = endTime;
    object->sliderType = type;
    object->repeats = Max(repeats, 1);
    insertElement(object);
}

void MapInfo::addSpinner(float spinRequired, float spinResistance, double time,
                         double endTime)
{
    LOG_ENTER();

    auto object = std::make_shared<ObjectTemplateSpinner>();
    object->startTime = time;
    object->spinRequired = spinRequired;
    object->spinResistance = spinResistance;
    object->endTime = endTime;
    insertElement(object);
}

void MapInfo::insertElement(std::shared_ptr<BaseObjectTemplate> obj)
{
    LOG_ENTER();

    auto insertPoint =
        std::find_if(objectTemplates.begin(), objectTemplates.end(),
                     [&obj](const std::shared_ptr<BaseObjectTemplate> &checked)
                     {
                         return checked->startTime <= obj->startTime;
                     });

    if (insertPoint == objectTemplates.end()) {
        objectTemplates.push_back(obj);
    }
    else {
        objectTemplates.insert(insertPoint, obj);
    }
}

const std::string &MapInfo::getName() const
{ return name; }

const std::string &MapInfo::getDescription() const
{ return description; }

const std::string &MapInfo::getArtist() const
{ return artist; }

const std::string &MapInfo::getDifficulty() const
{ return difficulty; }

const std::string &MapInfo::getSongPath() const
{ return songPath; }

float MapInfo::getCircleSize() const
{ return circleSize; }

float MapInfo::getStartOffset() const
{ return startOffset; }

double MapInfo::getMapDuration() const
{ return mapDuration; }

float MapInfo::getHpDrain() const
{ return HPDrain; }

float MapInfo::getApproachTime() const
{ return approachTime; }

float MapInfo::getHitWindow() const
{ return hitWindow; }

float MapInfo::getOverallDifficulty() const
{ return overallDifficulty; }

float MapInfo::getFadeTime() const
{ return fadeTime; }

void MapInfo::clear()
{ objectTemplates.clear(); }

void MapInfo::setName(const std::string &name)
{ MapInfo::name = name; }

void MapInfo::setDescription(const std::string &description)
{
    MapInfo::description = description;
}

void MapInfo::setArtist(const std::string &artist)
{ MapInfo::artist = artist; }

void MapInfo::setDifficulty(const std::string &difficulty)
{
    MapInfo::difficulty = difficulty;
}

void MapInfo::setSongPath(const std::string &songPath)
{
    MapInfo::songPath = songPath;
}

void MapInfo::setCircleSize(float circleSize)
{
    MapInfo::circleSize = circleSize;
}

void MapInfo::setStartOffset(float startOffset)
{
    MapInfo::startOffset = startOffset;
}

void MapInfo::setMapDuration(double mapDuration)
{
    MapInfo::mapDuration = mapDuration;
}

void MapInfo::setHpDrain(float hpDrain)
{ HPDrain = hpDrain; }

void MapInfo::setApproachTime(float approachTime)
{
    MapInfo::approachTime = approachTime;
}

void MapInfo::setHitWindow(float hitWindow)
{ MapInfo::hitWindow = hitWindow; }

void MapInfo::setFadeTime(float fadeTime)
{ MapInfo::fadeTime = fadeTime; }

void MapInfo::setOverallDifficulty(float overallDifficulty)
{
    MapInfo::overallDifficulty = overallDifficulty;
}

bool MapInfo::load(const std::filesystem::path &path)
{
    LOG_ENTER();

    if (path.extension() == ".osu")
        return LoadOSU(path, *this);
    else if (path.extension() == ".map")
        return LoadMAP(path, *this);
    else {
        log::error("Unrecognized file type");
        return false;
    }

}

bool MapInfo::create()
{ return true; }

const std::string &MapInfo::getRomanisedName() const
{
    return romanisedName;
}

void MapInfo::setRomanisedName(const std::string &romanisedName)
{
    MapInfo::romanisedName = romanisedName;
}

const std::string &MapInfo::getRomanisedArtist() const
{
    return romanisedArtist;
}
void MapInfo::setRomanisedArtist(const std::string &romanisedArtist)
{
    MapInfo::romanisedArtist = romanisedArtist;
}
const std::string &MapInfo::getSource() const
{
    return source;
}
void MapInfo::setSource(const std::string &source)
{
    MapInfo::source = source;
}
const std::vector<std::string> &MapInfo::getTags() const
{
    return tags;
}
void MapInfo::setTags(const std::vector<std::string> &tags)
{
    MapInfo::tags = tags;
}
const std::string &MapInfo::getAuthor() const
{
    return author;
}
void MapInfo::setAuthor(const std::string &author)
{
    MapInfo::author = author;
}

NS_END