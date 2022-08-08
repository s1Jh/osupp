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

namespace detail {

class BaseSound : public detail::Resource {
public:
	using SampleT = uint8_t;
	using BufferT = std::vector<SampleT>;

	[[nodiscard]] virtual SoundType getType() const = 0;

protected:
	virtual bool fillBuffer(BufferT& buffer) const = 0;
	[[nodiscard]] virtual bool isAtEOF() const = 0;
	[[nodiscard]] virtual SampleFormat getSampleFormat() const = 0;
	[[nodiscard]] virtual unsigned int getSampleRate() const = 0;

	bool setupBuffers(unsigned int size, unsigned int number);

	/**
	 * This function will generate a specified number of OpenAL buffers
	 * that contain subsequent parts of streamed audio.
	 *
	 * New buffers of the specified size will be generated until EOF is reached,
	 * if no new buffers need to be generated, false is returned. This may occur
	 * if only one buffer is used for the whole sample, only one buffer will ever
	 * be generated during the first call, subsequent calls will return false and
	 * no new buffers will be generated.
	 *
	 * @param number The number of OpenAL buffers to generate.
	 * @return Whether new buffers have been generated.
	 */
	bool generateAdditionalBuffers(std::vector<unsigned int>& retBuffers);
private:
	unsigned int bufferLimit;
	unsigned int bufferSize;
	unsigned int activeBuffer{0};
	std::list<BufferT> buffers;
};

}

NS_END