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

#include "Sound.hpp"
#include "Audio.hpp"

namespace PROJECT_NAMESPACE {

// Forward decl, since we include Channel.hpp in AudioDevice.hpp
class AudioDevice;

class Channel
{
	friend class AudioDevice;
public:

	bool forceSetSound(const std::weak_ptr<detail::BaseSound>& resource, bool play = false);
	bool setSound(const std::weak_ptr<detail::BaseSound>& resource, bool play = false,
				  SoundPriority priority = SoundPriority::MEDIUM);

	bool playSound(const std::weak_ptr<detail::BaseSound>& resource,
				  SoundPriority priority = SoundPriority::MEDIUM);

	Channel& loopSound(const std::weak_ptr<detail::BaseSound>& resource,
				   SoundPriority priority = SoundPriority::MEDIUM);

	[[nodiscard]] ChannelState getState() const;
	[[nodiscard]] SoundPriority getSoundPriority() const;

	void play();
	void pause();
	void stop();
	float seek(float position);
	void setVolume(float fraction, float transitionTime = 0);
	void setLooping(bool isLooping);

protected:
	bool setup();

private:
	void refillBuffer(unsigned int buffer);
	void generateBuffer(unsigned int buffer, const detail::BaseSound::BufferT &data);
	void setupBuffers(int count);
	void update();

	struct ALStructures {
		std::vector<unsigned int> buffers;
		unsigned int ALSource{0};
	};

	static void ALStructureDeleter(ALStructures* struc);

	std::shared_ptr<ALStructures> held;
	std::weak_ptr<detail::BaseSound> activeSound;
	SoundPriority currentPriority{SoundPriority::MEDIUM};
	ChannelState state{ChannelState::FREE};
};

}
