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