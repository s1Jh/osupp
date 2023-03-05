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
#include "EnumOperators.hpp"
#include "Settings.hpp"

#include <string>
#include <vector>

namespace PROJECT_NAMESPACE {

class AudioDevice;

constexpr unsigned int MAX_SFX_CHANNELS = 64;
constexpr unsigned int DEFAULT_SFX_CHANNELS = 16;
constexpr unsigned int AUDIO_STREAMING_BUFFERS = 4;

enum class SampleFormat {
	IS_STEREO = 0b001,
	TYPE = 0b110,

	TYPE8 = 0 << 1,
	TYPE16 = 1 << 1,

	MONO8 = TYPE8,
	MONO16 = TYPE16,

	STEREO8 = TYPE8 | IS_STEREO,
	STEREO16 = TYPE16 | IS_STEREO,
};

ENABLE_BITMASK_OPERATORS(SampleFormat)

template <typename SampleT>
using MonoSample = SampleT;

template <typename SampleT>
struct StereoSample {
	SampleT left, right;
};

using MonoSample8 = MonoSample<uint8_t>;
using MonoSample16 = MonoSample<int16_t>;
using StereoSample8 = StereoSample<uint8_t>;
using StereoSample16 = StereoSample<int16_t>;

template <typename T>
struct SampleInfo {
	const static bool enable = false;
};

#define DECLARE_MONO_SAMPLE_INFO(Type, EnumType) \
template <> \
struct SampleInfo<MonoSample<Type>> { \
	const static bool enable = true; \
	const static SampleFormat format = EnumType; \
	const static size_t size = sizeof(MonoSample<Type>); \
};

#define DECLARE_STEREO_SAMPLE_INFO(Type, EnumType) \
template <> \
struct SampleInfo<StereoSample<Type>> { \
	const static bool enable = true; \
	const static SampleFormat format = EnumType; \
	const static size_t size = sizeof(StereoSample<Type>); \
};

DECLARE_MONO_SAMPLE_INFO(uint8_t, SampleFormat::MONO8)
DECLARE_MONO_SAMPLE_INFO(int16_t, SampleFormat::MONO16)

DECLARE_STEREO_SAMPLE_INFO(uint8_t, SampleFormat::STEREO8)
DECLARE_STEREO_SAMPLE_INFO(int16_t, SampleFormat::STEREO16)

enum class SoundType {
	SAMPLE, STREAM, RECORD
};

enum class SoundPriority {
	VERY_LOW, LOW, MEDIUM, HIGH, VERY_HIGH
};

enum class ChannelState {
	FREE, PLAYING, PAUSED, STOPPED
};

enum class AudioDeviceType {
	UNRECOGNIZED, DISPLAY_PORT, HANDSET, HDMI, HEADPHONES, HEADSET, LINE, MICROPHONE, NETWORK, SPDIF, SPEAKERS
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
extern AudioDevice GetAudioDevice(const std::string& name, Settings& settings);


}
