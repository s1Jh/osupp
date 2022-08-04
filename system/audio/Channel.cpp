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
	alSourcef(ALSource, AL_SEC_OFFSET, position);
}

ChannelState Channel::getState() const
{
	return state;
}

SoundPriority Channel::getSoundPriority() const
{
	return currentPriority;
}

NS_END