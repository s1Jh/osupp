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

#include "MapLoaders.hpp"

#include "MapInfo.hpp"
#include "Vec2.hpp"
#include "define.hpp"
#include "Util.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

NS_BEGIN

class OSUMapLoader
{
public:
    struct HitObject
    {
        fvec2d position;
        double time;
        int type;
        int hitSound;
        std::vector<std::string> objectParams;
        std::vector<std::string> hitSample;
    };

    struct TimingPoint
    {
        double time;
        double beatLength;
        int meter;
        int sampleSet;
        int sampleIndex;
        int volume;
        int effects;
    };

    enum Section
    {
        METADATA, HIT_OBJECTS, GENERAL, TIMING_POINTS, DONT_CARE, COLOURS, DIFFICULTY, EDITOR, EVENTS
    };

    static fvec2d PosConversion(fvec2d v)
    {
        fvec2d coord = {(float) v.x / 512.f, (float) v.y / 384.f};
        coord *= 2.f;
        coord -= 1.0f;
        coord.y *= -1;
        coord.x *= (512.f / 384.f);
        return coord;
    }

    static float TimeConversion(int t)
    { return float(t) / 1000.f; }

    static std::string ReadRestOfStream(std::stringstream &stream)
    {
        std::string text;
        while (!stream.eof()) {
            std::string chunk;
            stream >> chunk;
            text.append(chunk + " ");
        }
        return text;
    }

    bool read(const std::filesystem::path &path)
    {
        std::ifstream ifs(path);

        if (!ifs.is_open()) {
            log::error("Unable to open file");
            return false;
        }

        std::string strLine;
        std::istringstream line(strLine);

        Section section = DONT_CARE;

        std::stringstream slurped;
        slurped << ifs.rdbuf();
        ifs.close();

        while (!slurped.eof()) {
            std::getline(slurped, strLine);
            Trim(strLine);

            if (strLine.empty()) {
                continue;
            }

            if (strLine.starts_with('[')) {
                Trim(strLine);
                if (strLine == "[HitObjects]") {
                    section = HIT_OBJECTS;
                } else if (strLine == "[General]") {
                    section = GENERAL;
                } else if (strLine == "[Metadata]") {
                    section = METADATA;
                } else if (strLine == "[TimingPoints]") {
                    section = TIMING_POINTS;
                } else if (strLine == "[Editor]") {
                    section = EDITOR;
                } else if (strLine == "[Difficulty]") {
                    section = DIFFICULTY;
                } else if (strLine == "[Events]") {
                    section = EVENTS;
                } else if (strLine == "[Colours]") {
                    section = COLOURS;
                } else {
                    section = DONT_CARE;
                }
                continue;
            }

            line = std::istringstream(strLine);

            switch (section) {
                case HIT_OBJECTS: {
                    HitObject object{};

                    // x, y, time, type, hitSound, objectParams, hitSample
                    auto params = GetCharacterSeparatedValues(strLine, ',');
                    object.position.x = GetParam<float>(params, 0, 0);
                    object.position.y = GetParam<float>(params, 1, 0);
                    object.position = PosConversion(object.position);
                    object.time = TimeConversion(GetParam<int>(params, 2, 0));
                    object.type = GetParam<int>(params, 3, 1);
                    object.hitSound = GetParam<int>(params, 4, 0);
                    if (params.size() > 5) {
                        if (params.back().find(':') != std::string::npos) {
                            object.hitSample = GetCharacterSeparatedValues(params.back(), ':');
                            object.objectParams = std::vector(std::next(params.begin(), 5), std::prev(params.end()));
                        } else {
                            object.objectParams = std::vector(std::next(params.begin(), 5), params.end());
                        }
                    }

                    hitObjectParams.push_back(std::move(object));
                    break;
                }
                case TIMING_POINTS: {
                    TimingPoint point{};

                    // time, beatLength, meter, sampleSet, sampleIndex, volume, uninherited, effects
                    auto params = GetCharacterSeparatedValues(strLine, ',');

                    point.time = TimeConversion(GetParam<int>(params, 0, 0));
                    point.beatLength = GetParam<double>(params, 1, 100);
                    point.meter = GetParam<int>(params, 2, 4);
                    point.sampleSet = GetParam<int>(params, 3, 0);
                    point.sampleIndex = GetParam<int>(params, 4, 0);
                    point.volume = GetParam<int>(params, 5, 100);
                    point.effects = GetParam<int>(params, 7, 0);

                    bool unInherited = GetParam<int>(params, 6, 1);

                    if (unInherited) {
                        point.beatLength = TimeConversion((int) point.beatLength);
                        uninheritedTimingPoints.push_back(point);
                    } else {
                        inheritedTimingPoints.push_back(point);
                    }

                    break;
                }
                case GENERAL:
                case METADATA:
                case DIFFICULTY: {
                    auto values = GetCharacterSeparatedValues(strLine, ':');
                    if (values.size() >= 2) {
                        auto &key = values[0];
                        auto &value = values[1];

                        Trim(key);
                        Trim(value);

                        meta[key] = value;
                    }
                    break;
                }
                    // TODO:
                case EVENTS: {
                    auto params = GetCharacterSeparatedValues(strLine, ',');

                    int type = GetParam<int>(params, 0, -1);
                    if (type == -1) {
                        // the event type, might be a string
                        auto videoException = GetParam<std::string>(params, 0, "");
                        if (videoException == "Video") {
                            type = 1;
                        } else {
                            log::error("Event type could not be determined");
                        }
                    }
                    double startTime = TimeConversion(GetParam<int>(params, 1, 0));
                    if (params.size() > 2) {
                        auto extraParams = std::vector(std::next(params.begin(), 2), std::prev(params.end()));
                    }
                }
                case EDITOR:break;

                case DONT_CARE:
                default:break;
            }
        }

        return true;
    }

    template<typename T>
    T getField(const std::string &name, T backup)
    {
        if (meta.contains(name)) {
            if constexpr (std::is_same_v<T, std::string>) {
                return meta[name];
            } else if constexpr (std::is_same_v<T, const char *>) {
                return meta[name].c_str();
            } else if constexpr (std::is_arithmetic_v<T>) {
                return static_cast<T>(std::strtod(meta[name].c_str(), nullptr));
            } else {
                return backup;
            }
        }

        return backup;
    }

    void operator()(MapInfo &map)
    {
        /*
        https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_(file_format)

        General:
            Option 	                    Value type 	Description
            ------------------------------------------------------------------------------------------------------------
            AudioFilename 	            String 	    Location of the audio file relative to the current folder.
            AudioLeadIn 	            Integer 	Milliseconds of silence before the audio starts playing
            AudioHash 	                String 	    Deprecated.
            PreviewTime 	            Integer 	Time in milliseconds when the audio preview should start.
            Countdown 	                Integer 	Speed of the countdown before the first hit object
                                                    (0 = no countdown, 1 = normal, 2 = half, 3 = double).
            SampleSet 	                String 	    Sample set that will be used if timing points do not override it
                                                    (Normal, Soft, Drum).
            StackLeniency 	            Decimal 	Multiplier for the threshold in time when hit objects placed close
                                                    together stack (0–1).
            Mode 	                    Integer     Game mode (0 = osu!, 1 = osu!taiko, 2 = osu!catch, 3 = osu!mania).
            LetterboxInBreaks 	        0 or 1 	    Whether breaks have a letter-boxing effect.
            StoryFireInFront 	        0 or 1 	    Deprecated.
            UseSkinSprites          	0 or 1 	    Whether the storyboard can use the user's skin images.
            AlwaysShowPlayfield 	    0 or 1 	    Deprecated.
            OverlayPosition 	        String 	    Draw order of hit circle overlays compared to hit numbers
                                                    (NoChange = use skin setting, Below = draw overlays under numbers,
                                                    Above = draw overlays on top of numbers).
            SkinPreference 	            String 	    Preferred skin to use during gameplay.
            EpilepsyWarning 	        0 or 1 	    Whether a warning about flashing colours should be shown at
                                                    the beginning of the map.
            CountdownOffset 	        Integer 	Time in beats that the countdown starts before the first hit object.
            SpecialStyle 	            0 or 1 	    Whether the "N+1" style key layout is used for osu!mania.
            WidescreenStoryboard 	    0 or 1 	    Whether the storyboard allows widescreen viewing.
            SamplesMatchPlaybackRate 	0 or 1 	    Whether sound samples will change rate when playing with
                                                    speed-changing mods.
            ------------------------------------------------------------------------------------------------------------

        Metadata
            Option 	        Value type 	                        Description
            ------------------------------------------------------------------------------------------------------------
            Title 	        String 	                            Romanised song title
            TitleUnicode 	String 	                            Song title
            Artist 	        String 	                            Romanised song artist
            ArtistUnicode 	String 	                            Song artist
            Creator 	    String 	                            Beatmap creator
            Version 	    String 	                            Difficulty name
            Source 	        String 	                            Original media the song was produced for
            Tags 	        Space-separated list of strings 	Search terms
            BeatmapID 	    Integer 	                        Difficulty ID
            BeatmapSetID 	Integer 	                        Beatmap ID
            ------------------------------------------------------------------------------------------------------------

        Difficulty
            Option 	            Value type  Description
            ------------------------------------------------------------------------------------------------------------
            HPDrainRate 	    Decimal 	HP setting (0–10)
            CircleSize 	        Decimal 	CS setting (0–10)
            OverallDifficulty 	Decimal 	OD setting (0–10)
            ApproachRate 	    Decimal 	AR setting (0–10)
            SliderMultiplier 	Decimal 	Base slider velocity in hundreds of osu! pixels per beat
            SliderTickRate 	    Decimal 	Amount of slider ticks per beat
            ------------------------------------------------------------------------------------------------------------
         */

        map.setHpDrain(getField("HPDrainRate", 10.0f));

        double circleSizeLevel = getField("CircleSize", 5.0f);
        auto circleSize = float(-0.0179411764705882 * circleSizeLevel + 0.220392156862745);
        map.setCircleSize(circleSize);

        map.setOverallDifficulty(getField("OverallDifficulty", 5.0f));

        float approachLevel = getField("ApproachRate", 5.0f);
        float approachTime =
            1.8f - math::Min(approachLevel, 5) * 0.12f - (approachLevel > 5 ? (approachLevel - 5) * 0.15f : 0);
        map.setApproachTime(approachTime);

        sliderMultiplier = getField("SliderMultiplier", 1.0f);

        map.setSongPath(getField("AudioFilename", "audio.mp3"));
        map.setStartOffset(TimeConversion(getField("AudioLeadIn", 0)));
        map.setName(getField("TitleUnicode", ""));
        map.setRomanisedName(getField("Title", ""));
        map.setArtist(getField("ArtistUnicode", ""));
        map.setRomanisedArtist(getField("Artist", ""));
        map.setSource(getField("Source", ""));
        map.setAuthor(getField("Creator", ""));
        map.setDifficulty(getField("Version", ""));
        std::string tags = getField("Tags", "");
        map.setTags(GetCharacterSeparatedValues(tags, ' '));

        for (auto it = hitObjectParams.begin(); it != hitObjectParams.end(); it++) {
            const auto &object = *it;

            bool comboEnd = false;
            if (it != std::prev(hitObjectParams.end())) {
                // if the next object in line is a combo start, consider this the combo end
                comboEnd = std::next(it)->type & 1 << 2;
            }

            if (object.type & 1 << 0) {
                // note
                map.addNote(object.position, comboEnd, object.time);
            } else if (object.type & 1 << 1) {
                // slider
                SliderPathT path;
                path.push_back(SliderNode{object.position, false});

                auto curveParams = GetCharacterSeparatedValues(GetParam<std::string>(object.objectParams, 0, ""), '|');

                for (unsigned int i = 1; i < curveParams.size(); i++) {
                    const auto &param = curveParams[i];
                    auto components = GetCharacterSeparatedValues(param, ':');
                    fvec2d position = {
                        GetParam<float>(components, 0, 0),
                        GetParam<float>(components, 1, 0)
                    };
                    position = PosConversion(position);
                    path.push_back(SliderNode{position, false});
                }

                auto length = GetParam<double>(object.objectParams, 2, 0);

                int repeats = math::Max(GetParam<int>(object.objectParams, 1, 1), 1);

                double SV = getSliderVelocity(object.time);
                double beat = getBeatLength(object.time);
                double duration = length / (sliderMultiplier * 100.0 * SV) * beat;
                duration *= repeats;
                double endTime = object.time + duration;

                math::CurveType type;

                auto curveType = GetParam<std::string>(curveParams, 0, "");

                // TODO: uncomment once all types of curve interpolation are finished
                switch (curveType.front()) {
                    case 'B':type = math::CurveType::BEZIER;
                        break;
                    case 'C':type = math::CurveType::BEZIER;
                        break;
                    case 'L':type = math::CurveType::STRAIGHT;
                        break;
                    case 'P':type = math::CurveType::BEZIER;
                        break;
                    default:type = math::CurveType::STRAIGHT;
                        break;
                }

                map.addSlider(path, comboEnd, object.time, endTime, type, repeats);
            } else if (object.type & 1 << 3) {
                // spinner
                // x,y,time,type,hitSound,endTime,hitSample
                map.addSpinner(
                    0, 0, object.time,
                    TimeConversion(GetParam<int>(object.objectParams, 0, int(object.time * 1000.0))));
            }

        }
    }

private:
    float getSliderVelocity(double time)
    {
        for (auto it = inheritedTimingPoints.begin(); it != inheritedTimingPoints.end(); it++) {
            if (it != std::prev(inheritedTimingPoints.end())) {
                if (math::InRangeIE(time, it->time, std::next(it)->time)) {
                    return 1.f / ((float(it->beatLength) * -1.f) / 100.f);
                }
            } else {
                return 1.f / ((float(it->beatLength) * -1.f) / 100.f);
            }
        }
        return 1.0f;
    }

    double getBeatLength(double time)
    {
        for (auto it = uninheritedTimingPoints.begin(); it != uninheritedTimingPoints.end(); it++) {
            if (it != std::prev(uninheritedTimingPoints.end())) {
                if (math::InRangeIE(time, it->time, std::next(it)->time)) {
                    return it->beatLength;
                }
            } else {
                return it->beatLength;
            }
        }
        return 0.1f;
    }

    float sliderMultiplier = 1.0f;
    std::vector<HitObject> hitObjectParams;
    std::vector<TimingPoint> inheritedTimingPoints;
    std::vector<TimingPoint> uninheritedTimingPoints;
    std::map<std::string, std::string> meta;
};

bool LoadOSU(const std::filesystem::path &pathIn, MapInfo &map)
{
    OSUMapLoader loader;
    if (loader.read(pathIn)) {
        loader(map);
        return true;
    }
    return false;
}
NS_END
