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

#include "Resource.hpp"
#include "Audio.hpp"

#include <vector>
#include <list>

NS_BEGIN

constexpr unsigned int DEFAULT_SOUND_BUFFER_CHUNKS = 4;
constexpr unsigned int DEFAULT_SOUND_BUFFER_SIZE = 1024;

class Channel;

namespace detail {

class BaseSound : public detail::Resource {
	friend class ::PROJECT_NAMESPACE::Channel;
public:
	using SampleT = StereoSample16;
	using BufferT = std::vector<SampleT>;

	[[nodiscard]] virtual SoundType getType() const = 0;
	[[nodiscard]] float getLength() const;
	[[nodiscard]] unsigned int getSampleRate() const;
	[[nodiscard]] SampleFormat getFormat() const;

protected:
	bool configure(unsigned int size, int sampleRate, SampleFormat format);

	virtual bool fillBuffer(BufferT& buffer) = 0;
	[[nodiscard]] virtual bool isAtEOF() const = 0;
	[[nodiscard]] virtual bool isStreaming() const = 0;

private:
	float length;
	unsigned int sampleRate;
	SampleFormat format;
};

}

NS_END