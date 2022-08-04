#pragma once

#include "define.hpp"

#include <string>
#include <vector>

#include "Channel.hpp"
#include "Audio.hpp"

#include <AL/al.h>

NS_BEGIN

namespace detail {
typedef struct ALCdevice ALCdevice;
typedef struct ALCcontext ALCcontext;
}

class AudioDevice {
public:
	explicit AudioDevice(unsigned int sfxChannelsIn = DEFAULT_SFX_CHANNELS);
	explicit AudioDevice(const AudioDeviceSpec&, unsigned int sfxChannelsIn = DEFAULT_SFX_CHANNELS);

	void process();

	Channel& getMusicChannel();
	Channel& getSFXChannel();

	AudioDeviceSpec spec;

private:
	struct ALContainer {
		detail::ALCdevice *device;
		detail::ALCcontext *context;
	};
	static void ALContextDeleter(ALContainer*);
	std::shared_ptr<ALContainer> held;
	Channel musicChannel{};
	std::vector<Channel> sfxChannels;
};

NS_END