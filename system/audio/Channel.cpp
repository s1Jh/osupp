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
#include "Log.hpp"
#include "Util.hpp"

#include <AL/al.h>

NS_BEGIN

void GenerateALBuffer(unsigned int buffer, SampleFormat format, int sampleRate, const detail::BaseSound::BufferT &data) {
	ALenum alFmt;
	switch (format) {
	case SampleFormat::Mono8: alFmt = AL_FORMAT_MONO8; break;
	case SampleFormat::Mono16: alFmt = AL_FORMAT_MONO16; break;
	case SampleFormat::Stereo8: alFmt = AL_FORMAT_STEREO8; break;
	case SampleFormat::Stereo16: alFmt = AL_FORMAT_STEREO16; break;
	default: alFmt = AL_FORMAT_STEREO8; break;
	}
	auto size = (ALsizei)data.size() * SampleInfo<detail::BaseSound::SampleT>::size;

	alBufferData(buffer, alFmt, data.data(), size, sampleRate);
}

void Channel::update()
{
	if (!activeSound || !held->ALSource)
		return;

	if (activeSound->isStreaming() && state == ChannelState::Playing) {
		int processed;
		alGetSourcei(held->ALSource, AL_BUFFERS_PROCESSED, &processed);

		while (processed--) {
			unsigned int buffer;
			alSourceUnqueueBuffers(held->ALSource, 1, &buffer);
			detail::BaseSound::BufferT newData;
			activeSound->fillBuffer(newData);
			GenerateALBuffer(buffer, activeSound->getFormat(), (int)activeSound->getSampleRate(), newData);
			alSourceQueueBuffers(held->ALSource, 1, &buffer);
		}
	}
}

void Channel::setVolume(float fraction, float transitionTime)
{

}

void Channel::play()
{
	state = ChannelState::Playing;
	alSourcePlay(held->ALSource);
}

void Channel::pause()
{
	state = ChannelState::Paused;
	alSourcePause(held->ALSource);
}

void Channel::stop()
{
	state = ChannelState::Stopped;
	alSourceStop(held->ALSource);
}

void Channel::setLooping(bool isLooping)
{
	alSourcei(held->ALSource, AL_LOOPING, isLooping);
}

float Channel::seek(float position)
{
	alSourcef(held->ALSource, AL_SEC_OFFSET, position);
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

bool Channel::setSound(const std::shared_ptr<detail::BaseSound> &resource, bool startPlaying, SoundPriority priority)
{
	if (currentPriority > priority)
		return false;

	activeSound = resource;

	if (!activeSound) {
		stop();
		state = ChannelState::Stopped;
		return false;
	}

	currentPriority = priority;
	stop();

	if (!activeSound->isStreaming()) {
		// we only need one buffer for samples
		setupBuffers(1);
		auto buffer = held->buffers[0];
		detail::BaseSound::BufferT data;
		activeSound->fillBuffer(data);
		GenerateALBuffer(buffer, activeSound->getFormat(), (int)activeSound->getSampleRate(), data);
		alSourcei(held->ALSource, AL_BUFFER, buffer);
	} else {
		int queued;
		alGetSourcei(held->ALSource, AL_BUFFERS_QUEUED, &queued);
		std::vector<unsigned int> trash;
		trash.resize(queued);
		if (queued != 0) {
			alSourceUnqueueBuffers(held->ALSource, queued, trash.data());
			CheckAL;
		}

		setupBuffers(AUDIO_STREAMING_BUFFERS);
		alSourcei(held->ALSource, AL_BUFFER, 0);
		CheckAL;

		for (unsigned int i = 0; i < AUDIO_STREAMING_BUFFERS; i++) {
			detail::BaseSound::BufferT newData;
			activeSound->fillBuffer(newData);

			GenerateALBuffer(held->buffers[i], activeSound->getFormat(), (int)activeSound->getSampleRate(), newData);
			CheckAL;
		}

		alSourceQueueBuffers(held->ALSource, held->buffers.size(), held->buffers.data());
	}

	if (startPlaying)
		play();

	return true;
}

bool Channel::forceSetSound(const std::shared_ptr<detail::BaseSound> &resource, bool startPlaying)
{
	activeSound = resource;
	if (startPlaying)
		play();
	else
		stop();

	return true;
}

void Channel::setupBuffers(int count)
{
	CheckAL;
	if (!held->buffers.empty()) {
		alDeleteBuffers((int)held->buffers.size(), held->buffers.data());
	}

	held->buffers.resize(count, 0);
	alGenBuffers((int)held->buffers.size(), held->buffers.data());
	CheckALh("Reconfigured held->buffers");
}

bool Channel::setup()
{
	held = std::make_shared<ALStructures>();

	if (alIsSource(held->ALSource)) {
		alDeleteSources(1, &held->ALSource);
	}

	CheckAL;
	alGenSources(1, &held->ALSource);
	log::debug("Created source ", held->ALSource);
	alSourcef(held->ALSource, AL_PITCH, 1);
	alSourcef(held->ALSource, AL_GAIN, 1.0f);
	alSource3f(held->ALSource, AL_POSITION, 0, 0, 0);
	alSource3f(held->ALSource, AL_VELOCITY, 0, 0, 0);
	alSourcei(held->ALSource, AL_LOOPING, AL_FALSE);
	CheckAL;

	setupBuffers(held->buffers.size());

	stop();
}

void Channel::ALStructureDeleter(Channel::ALStructures *struc)
{
	if (alIsSource(struc->ALSource)) {
		log::debug("Destroying source ", struc->ALSource);
		alDeleteSources(1, &struc->ALSource);
	}

	if (!struc->buffers.empty()) {
		alDeleteBuffers((int)struc->buffers.size(), struc->buffers.data());
	}
}

NS_END