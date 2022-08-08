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

//
// Created by sijh on 30.07.22.
//

#include "Audio.hpp"
#include "AudioDevice.hpp"

#include <mutex>
#include <vector>
#include <algorithm>
#include <AL/al.h>
#include <AL/alc.h>

NS_BEGIN

static std::mutex DeviceMutex;
static std::vector<AudioDeviceSpec> DeviceListing;
static AudioDeviceSpec DefaultDevice;

int RebuildDeviceList();

int RebuildDeviceList()
{
	DeviceListing.clear();

	auto* devicesPtr = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

	int found = 0;
	std::string nameBuffer;
	while (true) {
		if (*devicesPtr == 0) {
			// null termination
			if (nameBuffer.empty())
				break;

			AudioDeviceSpec dev;
			dev.name = nameBuffer;
			DeviceListing.push_back(dev);
			found++;

			nameBuffer.clear();
		} else {
			nameBuffer += *devicesPtr;
		}
		devicesPtr++;
	}

	DefaultDevice.name = alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);

	return found;
}

AudioDeviceSpec GetDefaultAudioDevice()
{
	std::scoped_lock<std::mutex> lock(DeviceMutex);
	RebuildDeviceList();
	return DefaultDevice;
}

std::vector<AudioDeviceSpec> GetAudioDevices()
{
	std::scoped_lock<std::mutex> lock(DeviceMutex);
	RebuildDeviceList();
	return DeviceListing;
}

AudioDevice GetAudioDevice(const std::string& name)
{
	decltype(DeviceListing)::iterator found;
	{
		std::scoped_lock<std::mutex> lock(DeviceMutex);
		RebuildDeviceList();
		found = std::find_if(DeviceListing.begin(), DeviceListing.end(),
								  [&name](const AudioDeviceSpec &spec)
								  { return spec.name == name; });
	}

	AudioDeviceSpec spec;
	if (found == DeviceListing.end()) {
		spec = GetDefaultAudioDevice();
	} else {
		spec = *found;
	}

	return AudioDevice{spec};
}

NS_END
