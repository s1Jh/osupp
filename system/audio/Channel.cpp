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

#include "Channel.hpp"

#include <AL/al.h>

NS_BEGIN

void Channel::update()
{
}

void Channel::setVolume(float fraction, float transitionTime)
{

}

void Channel::play()
{
	state = ChannelState::Playing;
	alSourcePlay(ALSource);
}

void Channel::pause()
{
	state = ChannelState::Paused;
	alSourcePause(ALSource);
}

void Channel::stop()
{
	state = ChannelState::Stopped;
	alSourceStop(ALSource);
}

void Channel::setLooping(bool isLooping)
{
	alSourcei(ALSource, AL_LOOPING, isLooping);
}

float Channel::seek(float position)
{
	// TODO: add query functions to BaseSound.
//	position = Clamp(position, 0, activeSound->getDuration());
	alSourcef(ALSource, AL_SEC_OFFSET, position);
	return position;
}

ChannelState Channel::getState() const
{
	return state;
}

SoundPriority Channel::getSoundPriority() const
{
	return currentPriority;
}

bool Channel::setSound(const std::shared_ptr<detail::BaseSound> &resource, SoundPriority priority)
{
	if (currentPriority > priority)
		return false;

	activeSound = resource;
	currentPriority = priority;
	return true;
}

bool Channel::forceSetSound(const std::shared_ptr<detail::BaseSound> &resource)
{
	activeSound = resource;
	return true;
}

NS_END