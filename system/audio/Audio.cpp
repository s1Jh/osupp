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

	std::string nameBuffer;
	while (true) {
		if (*devicesPtr == 0) {
			// null termination
			if (nameBuffer.empty())
				break;

			AudioDeviceSpec dev;
			dev.name = nameBuffer;
			DeviceListing.push_back(dev);

			nameBuffer.clear();
		} else {
			nameBuffer += *devicesPtr;
		}
		devicesPtr++;
	}

	DefaultDevice.name = alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
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
