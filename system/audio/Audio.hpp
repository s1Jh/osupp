#pragma once

#include "define.hpp"

#include <string>
#include <vector>

NS_BEGIN

class AudioDevice;

constexpr unsigned int MAX_SFX_CHANNELS = 64;
constexpr unsigned int DEFAULT_SFX_CHANNELS = 16;

enum class SoundType {
	Sample, Stream, Record
};

enum class SoundPriority {
	VeryLow, Low, Medium, High, VeryHigh
};

enum class ChannelState {
	Free, Playing, Paused, Stopped
};

enum class AudioDeviceType {
	Unrecognized,
	DisplayPort,
	Handset,
	HDMI,
	Headphones,
	Headset,
	Line,
	Microphone,
	Network,
	SPDIF,
	Speakers
};

struct AudioDeviceSpec {
	int id;
	std::string name;
	std::string driver;
	bool available;
	bool systemDefault;
	bool input;
	bool loopBack;
	AudioDeviceType type;
};

extern std::vector<AudioDeviceSpec> GetAudioDevices();
extern AudioDeviceSpec GetDefaultAudioDevice();
extern AudioDevice GetAudioDevice(const std::string& name);


NS_END
