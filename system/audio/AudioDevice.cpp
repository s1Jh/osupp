#include "AudioDevice.hpp"

#include "Math.hpp"

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
	sfxChannels.resize(Clamp(sfxChannelsIn, 1, MAX_SFX_CHANNELS));

	device = (detail::ALCdevice*) alcOpenDevice(nullptr);

	if (!device) {
		log::error("Failed to open device: ", specIn.name);
		return;
	}

	const int params[] = {AL_FREQUENCY, 48000, 0};

	context = (detail::ALCcontext*) alcCreateContext((ALCdevice*)device, params);

	if (!context) {
		log::error("Failed to create context for device: ", specIn.name);
		return;
	}
	if (!alcMakeContextCurrent((ALCcontext*) context)) {
		log::error("Failed to make OpenAL context current");
	}

	log::info("Successfully initiated device ", specIn.name);
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

AudioDevice::~AudioDevice()
{
	alcDestroyContext((ALCcontext*) context);
	alcCloseDevice((ALCdevice*) device);
}

NS_END

