#include "MapLoaders.hpp"

#include "MapInfo.hpp"
#include "Vec2.hpp"
#include "define.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

NS_BEGIN
bool LoadNote(std::vector<std::string> params, MapInfo &map);

bool LoadSlider(std::vector<std::string> params, MapInfo &map);

bool LoadSpinner(std::vector<std::string> params, MapInfo &map);

std::string ReadRestOfStream(std::stringstream &stream);

std::vector<std::string> GetCharacterSeparatedValues(std::string in, char sep);

fvec2d PosConversion(fvec2d v);

float TimeConversion(int t);

fvec2d PosConversion(fvec2d v)
{
    fvec2d coord = {(float) v.x / 512.f, (float) v.y / 384.f};
    coord *= 2.f;
    coord -= 1.0f;
    return coord;
}

float TimeConversion(int t)
{ return float(t) / 1000.f; }

/*enum Output
{
    Success,
    CouldNotOpen,
    NotOsuFile,
    CouldNotSave,
    Unknown
};

std::string convertFailureEnum(Output reason)
{
    switch (reason)
    {
        case CouldNotOpen:
            return "Could not open the file.\nNote that special characters in
the file path may also result in this error. If that is the case, make sure the
file and the directory it is in doesn't have any non ASCII characters in it's
name."; case CouldNotSave: return "Could not create the output file.\nCheck if
you have write permissions to the directory where the map importer located.";
        case NotOsuFile:
            return "File does not appear to be a OSU! map file\nThis file does
not appear to start with the osu map file header."; case Unknown: return
"Unknown reason.\nPlease, contact the developer with the version and the map
file you were trying to convert."; case Success: return "wat"; default: return
"Failed to decode a failure reason.\nA error in the program caused a invalid
error, please restart the program and try again.";
    }
}*/

std::vector<std::string> GetCharacterSeparatedValues(std::string in, char sep)
{
    std::vector<std::string> results;

    std::stringstream ss(in);
    std::string str;
    while (getline(ss, str, sep)) {
        results.push_back(str);
    }

    return results;
}

std::string ReadRestOfStream(std::stringstream &stream)
{
    std::string text;
    while (!stream.eof()) {
        std::string chunk;
        stream >> chunk;
        text.append(chunk + " ");
    }
    return text;
}

enum Section
{
    Metadata, HitObjects, General, TimingPoints, DontCare
};

bool LoadNote(std::vector<std::string> params, MapInfo &map)
{
    fvec2d pos =
        PosConversion(fvec2d{std::stof(params[0]), std::stof(params[1])});

    float time = TimeConversion(std::stoi(params[2]));

    // TODO: Handle combo ends, since osu saves these as combo starts
    map.addNote(pos, false, time);

    return true;
}

bool LoadSlider(std::vector<std::string> params, MapInfo &map)
{
    SliderPathT path;

    fvec2d pos =
        PosConversion(fvec2d{std::stof(params[0]), std::stof(params[1])});
    path.push_back(SliderNode{.position = pos});

    float time = TimeConversion(std::stoi(params[2]));
    auto curveParams = GetCharacterSeparatedValues(params[5], '|');
    int count = curveParams.size();

    for (int i = 1; i < curveParams.size(); i++) {
        std::string param = curveParams[i];

        auto components = GetCharacterSeparatedValues(param, ':');

        pos = PosConversion(
            fvec2d{std::stof(components[0]), std::stof(components[1])});
        path.push_back(SliderNode{.position = pos});
    }

    // map.addSlider(path, false, time, );

    return true;
}

bool LoadSpinner(std::vector<std::string> params, MapInfo &map)
{}

bool LoadOSU(std::filesystem::path pathIn, MapInfo &map)
{
    std::ifstream ifs(pathIn);

    if (!ifs.is_open()) {
        return false;
    }

    std::string strline, identifier;

    std::getline(ifs, strline);
    std::istringstream line(strline);

    line >> identifier;
    if (identifier != "osu") {
        ifs.close();
        return false;
    }

    Section section = DontCare;

    while (std::getline(ifs, strline)) {
        if (strline.empty())
            continue;

        line = std::istringstream(strline);

        if (section != DontCare) {
            switch (section) {
                case HitObjects: {
                    auto params = GetCharacterSeparatedValues(strline, ',');

                    int type = std::stoi(params[3]);

                    if (type & (1 << 0))
                        LoadNote(params, map);
                    if (type & (1 << 1))
                        LoadSlider(params, map);
                    if (type & (1 << 3))
                        LoadSpinner(params, map);
                }
                    break;
                case General:
                    break;

                case Metadata:
                    break;
                case TimingPoints:
                    break;

                case DontCare:
                default:
                    break;
            }
        }

        if (strline[0] == '[') {
            if (strline == "[HitObjects]") {
                section = HitObjects;
            }
            else if (strline == "[General]") {
                section = General;
            }
            else if (strline == "[Metadata]") {
                section = Metadata;
            }
            else if (strline == "[TimingPoints]") {
                section = TimingPoints;
            }
            else {
                section = DontCare;
            }
        }
    }

    ifs.close();
    return true;
}

NS_END
