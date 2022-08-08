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

#include "AudioDevice.hpp"

#include "Math.hpp"
#include "Context.hpp"

#include <utility>
#include <AL/al.h>
#include <AL/alc.h>

NS_BEGIN

Channel &AudioDevice::getMusicChannel()
{
	return musicChannel;
}

Channel &AudioDevice::getSFXChannel()
{
	Channel *lowestPriority = &sfxChannels[0];

	// return the first free channel found, otherwise return the last channel with the lowest playing priority
	for (auto& channel : sfxChannels) {
		if (channel.getState() == ChannelState::Free)
			return channel;

		if (channel.getSoundPriority() <= lowestPriority->getSoundPriority()) {
			lowestPriority = &channel;
		}
	}

	return *lowestPriority;
}

AudioDevice::AudioDevice(const AudioDeviceSpec& specIn, unsigned int sfxChannelsIn) : spec(specIn)
{
	held = std::shared_ptr<ALContainer>(new ALContainer{nullptr, nullptr}, ALContextDeleter);
	sfxChannels.resize(Clamp(sfxChannelsIn, 1, MAX_SFX_CHANNELS));

	held->device = (detail::ALCdevice*) alcOpenDevice(specIn.name.c_str());

	if (!held->device) {
		log::error("Failed to open device: ", specIn.name);
		return;
	}

	auto freqSetting = GetContext().settings.addSetting<int>("setting.audio.frequency", 44100,
															 SettingFlags::WriteToFile, 1000, 48000).get();

	const int params[] = {AL_FREQUENCY, freqSetting, ALC_SYNC, 1, 0};

	held->context = (detail::ALCcontext*) alcCreateContext((ALCdevice*)held->device, params);

	if (!held->context) {
		log::error("Failed to create context for device: ", specIn.name);
		return;
	}
	if (!alcMakeContextCurrent((ALCcontext*) held->context)) {
		log::error("Failed to make OpenAL context current");
	}

	log::info("Successfully initiated device ", specIn.name, " @", freqSetting, "Hz");
}

AudioDevice::AudioDevice(unsigned int sfxChannelsIn)
{
	sfxChannels.resize(Clamp(sfxChannelsIn, 1, MAX_SFX_CHANNELS));
}

void AudioDevice::process()
{
	musicChannel.update();

	for (auto& channel : sfxChannels)
		channel.update();
}

void AudioDevice::ALContextDeleter(AudioDevice::ALContainer *container)
{
	log::debug("Closing audio device ", container);
	alcDestroyContext((ALCcontext*) container->context);
	alcCloseDevice((ALCdevice*) container->device);
}

NS_END

