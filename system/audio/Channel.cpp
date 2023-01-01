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
#include <cmath>
#include <fstream>

NS_BEGIN

void Channel::generateBuffer(unsigned int buffer, const detail::BaseSound::BufferT &data)
{
    ALenum alFmt;

    if (auto ptr = activeSound.lock()) {

        switch (ptr->getFormat()) {
            case SampleFormat::MONO8: alFmt = AL_FORMAT_MONO8;
                break;
            case SampleFormat::MONO16: alFmt = AL_FORMAT_MONO16;
                break;
            case SampleFormat::STEREO8: alFmt = AL_FORMAT_STEREO8;
                break;
            case SampleFormat::STEREO16: alFmt = AL_FORMAT_STEREO16;
                break;
            default: alFmt = AL_FORMAT_STEREO8;
                break;
        }
        auto size = (ALsizei) data.size() * SampleInfo<detail::BaseSound::SampleT>::size;

        alBufferData(buffer, alFmt, data.data(), size, ptr->getSampleRate());
    }
}

void Channel::refillBuffer(unsigned int buffer)
{
    detail::BaseSound::BufferT newData;

    if (auto ptr = activeSound.lock()) {
        ptr->fillBuffer(newData);
    } else {
        return;
    }

//	log::debug("Filling buffer ", buffer, " with ", newData.size(), " samples");
    generateBuffer(buffer, newData);
    CheckALh("Filled buffer");
}

void Channel::update()
{
    if (!held) {
        return;
    }

    if (!held->ALSource) {
        return;
    }

    if (auto ptr = activeSound.lock()) {
        if (ptr->isStreaming() && state == ChannelState::PLAYING) {
            int processed;
            alGetSourcei(held->ALSource, AL_BUFFERS_PROCESSED, &processed);

            while (processed--) {
                CheckALh("before");
                unsigned int buffer;
                alSourceUnqueueBuffers(held->ALSource, 1, &buffer);
                CheckALh("Unqueued buffer");
                refillBuffer(buffer);
                alSourceQueueBuffers(held->ALSource, 1, &buffer);
                CheckALh("Queued buffer");
            }
        }

        int momentaryState;
        alGetSourcei(held->ALSource, AL_SOURCE_STATE, &momentaryState);

        switch (momentaryState) {
            case AL_PLAYING: state = ChannelState::PLAYING;
                break;
            case AL_PAUSED: state = ChannelState::PAUSED;
                break;
            case AL_STOPPED: state = ChannelState::STOPPED;
                break;
            case AL_INITIAL: state = ChannelState::FREE;
                break;
            default: break;
        }
    }
}

void Channel::setVolume(float fraction, float)
{
    if (!held) {
        return;
    }
    alSourcef(held->ALSource, AL_GAIN, std::log10(fraction + 1.0f));
}

void Channel::play()
{
    if (!held) {
        return;
    }

    state = ChannelState::PLAYING;
    alSourcePlay(held->ALSource);
}

void Channel::pause()
{
    if (!held) {
        return;
    }

    state = ChannelState::PAUSED;
    alSourcePause(held->ALSource);
}

void Channel::stop()
{
    if (!held) {
        return;
    }

    state = ChannelState::STOPPED;
    alSourceStop(held->ALSource);

    if (auto ptr = activeSound.lock()) {
        ptr->reset();
    }
}

void Channel::setLooping(bool isLooping)
{
    if (!held) {
        return;
    }

    alSourcei(held->ALSource, AL_LOOPING, isLooping);
}

float Channel::seek(float position)
{
    if (!held) {
        return 0.0f;
    }

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

bool Channel::setSound(const std::weak_ptr<detail::BaseSound> &resource, bool startPlaying, SoundPriority priority)
{
    if (!held) {
        return false;
    }
    if (currentPriority > priority) {
        return false;
    }

    activeSound = resource;

    stop();

    if (auto ptr = activeSound.lock()) {
        currentPriority = priority;

        int queued;
        alGetSourcei(held->ALSource, AL_BUFFERS_PROCESSED, &queued);

        if (queued != 0) {
            std::vector<unsigned int> trash;
            trash.resize(queued);
            alSourceUnqueueBuffers(held->ALSource, queued, trash.data());
            CheckAL;
        }

        if (!ptr->isStreaming()) {
            // we only need one buffer for samples
            setupBuffers(1);
            auto buffer = held->buffers[0];
            detail::BaseSound::BufferT data;
            ptr->fillBuffer(data);
            generateBuffer(buffer, data);
            alSourcei(held->ALSource, AL_BUFFER, buffer);

        } else {
            setupBuffers(AUDIO_STREAMING_BUFFERS);
            alSourcei(held->ALSource, AL_BUFFER, 0);
            CheckAL;

            for (unsigned int i = 0; i < AUDIO_STREAMING_BUFFERS; i++) {
                refillBuffer(held->buffers[i]);
            }

            alSourceQueueBuffers(held->ALSource, held->buffers.size(), held->buffers.data());
        }

        if (startPlaying) {
            play();
        }

        return true;
    } else {
        return false;
    }
}

bool Channel::forceSetSound(const std::weak_ptr<detail::BaseSound> &resource, bool startPlaying)
{
    if (!held) {
        return false;
    }

    activeSound = resource;
    if (startPlaying) {
        play();
    } else {
        stop();
    }

    return true;
}

void Channel::setupBuffers(int count)
{
    if (!held) {
        return;
    }

    CheckAL;

    alSourceStop(held->ALSource);
    alSourcei(held->ALSource, AL_BUFFER, 0);
    CheckALh("Unqueued");

    if (!held->buffers.empty()) {
        alDeleteBuffers((int) held->buffers.size(), held->buffers.data());
        CheckALh("Deleted old buffers");
    }

    held->buffers.resize(count, 0);
    alGenBuffers((int) held->buffers.size(), held->buffers.data());
    CheckALh("Generated new buffers");
}

bool Channel::setup()
{
    held = std::make_shared<ALStructures>();

    if (alIsSource(held->ALSource)) {
        alDeleteSources(1, &held->ALSource);
    }

    CheckAL;
    alGenSources(1, &held->ALSource);

    alSourcef(held->ALSource, AL_PITCH, 1);
    alSourcef(held->ALSource, AL_GAIN, 1.0f);
    alSource3f(held->ALSource, AL_POSITION, 0, 0, 0);
    alSource3f(held->ALSource, AL_VELOCITY, 0, 0, 0);
    alSourcei(held->ALSource, AL_LOOPING, AL_FALSE);
    CheckAL;

    setupBuffers(held->buffers.size());

    stop();
    return alIsSource(held->ALSource);
}

void Channel::ALStructureDeleter(Channel::ALStructures *struc)
{
    if (alIsSource(struc->ALSource)) {
        alDeleteSources(1, &struc->ALSource);
    }

    if (!struc->buffers.empty()) {
        alDeleteBuffers((int) struc->buffers.size(), struc->buffers.data());
    }
}

bool Channel::playSound(const std::weak_ptr<detail::BaseSound> &resource, SoundPriority priority)
{
    if (!held) {
        return false;
    }

    setLooping(false);
    return setSound(resource, true, priority);
}

Channel &Channel::loopSound(const std::weak_ptr<detail::BaseSound> &resource, SoundPriority priority)
{
    if (!held) {
        return *this;
    }

    setLooping(true);
    setSound(resource, true, priority);
    return *this;
}

NS_END