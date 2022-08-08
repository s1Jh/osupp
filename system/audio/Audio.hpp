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

#pragma once

#include "define.hpp"

#include <string>
#include <vector>

NS_BEGIN

class AudioDevice;

constexpr unsigned int MAX_SFX_CHANNELS = 64;
constexpr unsigned int DEFAULT_SFX_CHANNELS = 16;

enum class SampleFormat {
	Mono8,
	Mono16,
	Stereo8,
	Stereo16
};

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
