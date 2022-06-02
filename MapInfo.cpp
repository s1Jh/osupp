#include "MapInfo.hpp"

#include "Util.hpp"
#include "HitObjects.h"

#include <fstream>

namespace GAME_TITLE {

    const MapInfo::StorageT &MapInfo::getObjectTemplates() const {
        return objectTemplates;
    }

    bool MapInfo::loadMap(const std::filesystem::path &path, LoadLayers layers) {
        LOG_ENTER("MAP::LOADMAP");
        const std::string metaIdentifiers = "0123456789";
        const std::string objectIdentifiers = "NSPO";
        const std::string eventIdentifiers = "E";


        log::info("Loading map: ", path);
        std::ifstream ifs(path);

        if (!ifs.is_open()) {
            ifs.close();
            log::error("Failed to load map, could not open the file");
            return false;
        }

        objectTemplates.clear();

        float time = 0.f;

        std::string strLine;
        while (std::getline(ifs, strLine)) {
            if (strLine.empty())
                continue;

            std::stringstream line(strLine);

            char type;
            line >> type;

            if (metaIdentifiers.find(type) != std::string::npos && bool(layers & LoadLayers::Meta))
                readMeta(type, line);
            if (objectIdentifiers.find(type) != std::string::npos && bool(layers & LoadLayers::Objects))
                readObject(time, type, line);
            if (eventIdentifiers.find(type) != std::string::npos && bool(layers & LoadLayers::Event))
                readEvent(time, type, line);
        }

        ifs.close();
        return true;
    }

    void MapInfo::addNote(fvec2d position, bool comboEnd, float time) {
        auto object = std::make_shared<ObjectTemplateNote>();
        object->startTime = time;
        object->endTime = time;
        object->position = position;
        object->parameters |= comboEnd ? HitObjectParams::ComboEnd : OBJECT_NO_PARAMS;
        insertElement(object);
    }

    void MapInfo::addSlider(const SliderPathT &points, bool comboEnd, float time, float endTime, SliderType type) {
        auto object = std::make_shared<ObjectTemplateSlider>();
        object->startTime = time;
        object->path = points;
        object->endTime = endTime;
        object->sliderType = type;

        for (auto it = object->path.begin(); it != std::prev(object->path.end()); it++) {
            it->connectingLine = GLLine(it->position, std::next(it)->position);
        }

        insertElement(object);
    }

    void MapInfo::addSpinner(float spinRequired, float spinResistance, float time) {
        auto object = std::make_shared<ObjectTemplateSpinner>();
        object->startTime = time;
        object->spinRequired = spinRequired;
        object->spinResistance = spinResistance;
        // TODO: parameter
        object->endTime = time + 5.0f;
        insertElement(object);
    }

    void MapInfo::readObject(float &time, char type, std::stringstream &line) {
        // TODO: Change all this
        switch (type) {
            case 'N': {
                float x, y, delta;
                int streak;
                line >> x >> y >> delta >> streak;
                time += delta;
                auto pos = fvec2d{x, y} * 2.f - fvec2d{1.f, 1.f};
                addNote(pos, streak, time);
                break;
            }
            case 'S': {
                float delta, speed;
                int streak, count;
                line >> count >> delta >> speed;
                time += delta;

                SliderPathT points;
                for (int i = 0; i < count; i++) {
                    float x, y;
                    bool needed;
                    line >> x >> y >> needed;
                    auto pos = fvec2d{x, y} * 2.f - fvec2d{1.f, 1.f};
                    points.push_front({pos, needed});
                }

                points.reverse();
                addSlider(points, streak, time, speed, SliderType::Straight);
                break;
            }
            case 'P': {
                float required, delta;
                line >> required >> delta;
                time += delta;

                addSpinner(required, 1.f, time);
                break;
            }
            case 'O': {
                line >> time;
                break;
            }
            default:
                break;
        }
    }

    void MapInfo::readMeta(char type, std::stringstream &line) {
        switch (type) {
            case '0':
                readString(line, name);
                log::info("Map name: ", name);
                break;
            case '1':
                readString(line, description);
                log::info("Map description: ", description);
                break;
            case '2':
                readString(line, artist);
                log::info("Map name: ", artist);
                break;
            case '3':
                readString(line, difficulty);
                log::info("Map description: ", difficulty);
                break;
            case '4':
                line >> startOffset;
                log::info("Applying start offset of ", startOffset);
                break;
            case '5':
                line >> approachTime;
                log::info("Applying aproach startTime of ", approachTime);
                break;
            case '6':
                line >> circleSize;
                log::info("Applying circle size of ", circleSize);
                break;
            case '7':
                line >> HPDrain;
                log::info("Applying HP drain of ", HPDrain);
                break;
            case '8':
                line >> overallDifficulty;
                log::info("Applying overall difficulty of ", overallDifficulty);
                break;
            case '9':
                readString(line, songPath);
                log::info("Song path: ", name);
                break;
            default:
                break;
        }
    }

    void MapInfo::readEvent(float &elementTime, char type, std::stringstream &line) {

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

    void MapInfo::readString(std::stringstream &from, std::string &to) {
        while (!from.eof()) {
            std::string chunk;
            from >> chunk;
            to.append(chunk + " ");
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
}