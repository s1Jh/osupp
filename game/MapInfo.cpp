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

#include "MapInfo.hpp"

#include "HitObjects.hpp"
#include "MapLoaders.hpp"
#include "Util.hpp"

#include <fstream>

NS_BEGIN

template<> const std::vector<std::string> Resource<MapInfo>::allowedExtensions = {".map", ".osu"};

const MapInfo::StorageT &MapInfo::getObjectTemplates() const
{
	return objectTemplates;
}

void MapInfo::addNote(const fvec2d &position, bool comboEnd, double time)
{
	auto object = std::make_shared<ObjectTemplateNote>();
	object->startTime = time;
	object->endTime = time;
	object->position = position;
	object->parameters |= comboEnd ? HitObjectParams::COMBO_END : HitObjectParams::NONE;
	insertElement(object);
}

void MapInfo::addSlider(const SliderPathT &points, bool comboEnd, double time,
						double endTime, math::CurveType type, unsigned int repeats)
{
	auto object = std::make_shared<ObjectTemplateSlider>();
	object->startTime = time;
	object->parameters |= comboEnd ? HitObjectParams::COMBO_END : HitObjectParams::NONE;
	object->path = points;
	object->endTime = endTime;
	object->sliderType = type;
	object->repeats = math::Max(repeats, 1);
	insertElement(object);
}

void MapInfo::addSpinner(float spinRequired, float spinResistance, double time,
						 double endTime, const fvec2d &position)
{
	auto object = std::make_shared<ObjectTemplateSpinner>();
	object->startTime = time;
	object->spinRequired = spinRequired;
	object->spinResistance = spinResistance;
	object->endTime = endTime;
	object->position = position;
	if (position != fvec2d{0, 0}) {
		object->free = true;
	}
	insertElement(object);
}

void MapInfo::insertElement(std::shared_ptr<BaseObjectTemplate> obj)
{
    auto insertPoint =
        std::find_if(
            objectTemplates.begin(), objectTemplates.end(),
            [&obj](const std::shared_ptr<BaseObjectTemplate> &checked)
            {
                return checked->startTime > obj->startTime;
            }
        );

    if (insertPoint == objectTemplates.end()) {
        objectTemplates.push_back(obj);
    } else {
        objectTemplates.insert(std::prev(insertPoint), obj);
    }
}

void MapInfo::clear()
{ objectTemplates.clear(); }

template<>
Resource<MapInfo> Load(const std::filesystem::path &path)
{
	Resource<MapInfo> r;

	r->directory = path.parent_path();

	bool success;

	if (path.extension() == ".osu") {
		success = LoadOSU(path, *r);
	} else if (path.extension() == ".map") {
		success = LoadMAP(path, *r);
	} else {
		log::error("Unrecognized file type");
		return nullptr;
	}

	if (success) {
		return r;
	}

	log::error("Failed to load map");
	return nullptr;
}

const std::filesystem::path &MapInfo::getDirectory() const
{
	return directory;
}

NS_END