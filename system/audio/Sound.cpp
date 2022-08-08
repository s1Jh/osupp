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

#include "Sound.hpp"
#include "Math.hpp"

#include <AL/al.h>

NS_BEGIN

namespace detail
{

bool BaseSound::setupBuffers(unsigned int size, unsigned int number)
{
	bufferLimit = Max(number, 1);
	bufferSize = Max(size, 0xff);

	return true;
}

bool BaseSound::generateAdditionalBuffers(std::vector<unsigned int> &retBuffers)
{
	retBuffers.clear();
	for (unsigned int i = 0; i < bufferLimit; i++) {
		// Return false only if we reach EOF and there have been no new buffers
		if (isAtEOF())
			return !retBuffers.empty();

		BufferT one;
		one.resize(bufferSize, 0);

		if (fillBuffer(one)) {
			ALuint buffer;
			alGenBuffers(1, &buffer);

			auto fmt = getSampleFormat();
			ALenum alFmt;
			switch (fmt) {
			case SampleFormat::Mono8: alFmt = AL_FORMAT_MONO8; break;
			case SampleFormat::Mono16: alFmt = AL_FORMAT_MONO16; break;
			case SampleFormat::Stereo8: alFmt = AL_FORMAT_STEREO8; break;
			case SampleFormat::Stereo16: alFmt = AL_FORMAT_STEREO16; break;
			default: alFmt = AL_FORMAT_STEREO8; break;
			}
			alBufferData(buffer, alFmt, one.data(), (int)one.size(), (int)getSampleRate());
			retBuffers.push_back(buffer);
		} else {
			log::error("Failed to generate sound chunk!");
		}
	}
	return true;
}
}

NS_END