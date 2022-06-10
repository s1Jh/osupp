#include "MapLoaders.hpp"

#include "define.hpp"

#include <fstream>
#include <sstream>
#include <string>

NS_BEGIN

void ReadObject(float &time, char type, std::stringstream &line, MapInfo& map);
void ReadMeta(char type, std::stringstream &line, MapInfo& map);
void ReadEvent(float &elementTime, char type, std::stringstream &line, MapInfo& map);
void ReadString(std::stringstream &from, std::string &to);

bool LoadMAP(const std::string &path, MapInfo &map) {
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

    map.clear();

    float time = 0.f;

    std::string strLine;
    while (std::getline(ifs, strLine)) {
        if (strLine.empty())
            continue;

        std::stringstream line(strLine);

        char type;
        line >> type;

        if (metaIdentifiers.find(type) != std::string::npos)
            ReadMeta(type, line, map);
        if (objectIdentifiers.find(type) != std::string::npos)
            ReadObject(time, type, line, map);
        if (eventIdentifiers.find(type) != std::string::npos)
            ReadEvent(time, type, line, map);
    }

    ifs.close();
    return true;
}

void ReadObject(float &time, char type, std::stringstream &line, MapInfo& map) {
    // TODO: Change all this
    switch (type) {
        case 'N': {
            float x, y, delta;
            int streak;
            line >> x >> y >> delta >> streak;
            time += delta;
            auto pos = fvec2d{x, y} * 2.f - fvec2d{1.f, 1.f};
            map.addNote(pos, streak, time);
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
            map.addSlider(points, streak, time, speed, CurveType::Straight, 1);
            break;
        }
        case 'P': {
            float required, delta;
            line >> required >> delta;
            time += delta;
            // TODO:
            map.addSpinner(required, 1.f, time, time);
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

void ReadMeta(char type, std::stringstream &line, MapInfo& map) {
    std::string buffer;
    float fbuffer;
    switch (type) {
        case '0':
            ReadString(line, buffer);
            log::info("Map name: ", buffer);
            map.setName(buffer);
            break;
        case '1':
            ReadString(line, buffer);
            log::info("Map description: ", buffer);
            map.setDescription(buffer);
            break;
        case '2':
            ReadString(line, buffer);
            log::info("Song artist: ", buffer);
            map.setArtist(buffer);
            break;
        case '3':
            ReadString(line, buffer);
            log::info("Difficulty description: ", buffer);
            map.setDifficulty(buffer);
            break;
        case '4':
            line >> fbuffer;
            log::info("Applying start offset of ", fbuffer);
            map.setStartOffset(fbuffer);
            break;
        case '5':
            line >> fbuffer;
            log::info("Applying approach time of ", fbuffer);
            map.setApproachTime(fbuffer);
            break;
        case '6':
            line >> fbuffer;
            log::info("Applying circle size of ", fbuffer);
            map.setCircleSize(fbuffer);
            break;
        case '7':
            line >> fbuffer;
            log::info("Applying HP drain of ", fbuffer);
            map.setHpDrain(fbuffer);
            break;
        case '8':
            line >> fbuffer;
            log::info("Applying overall difficulty of ", fbuffer);
            map.setOverallDifficulty(fbuffer);
            break;
        case '9':
            ReadString(line, buffer);
            log::info("Song path: ", buffer);
            map.setSongPath(buffer);
            break;
        default:
            break;
    }
}

void ReadEvent(float &elementTime, char type, std::stringstream &line, MapInfo& map) {

}

void ReadString(std::stringstream &from, std::string &to) {
    while (!from.eof()) {
        std::string chunk;
        from >> chunk;
        to.append(chunk + " ");
    }
}

NS_END