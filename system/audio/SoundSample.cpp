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

#include "SoundSample.hpp"
#include "Log.hpp"
#include "Util.hpp"

#include "AudioUtil.hpp"

NS_BEGIN

bool SoundSample::load(const std::filesystem::path &path)
{
	auto ctx = OpenFFmpegContext(path);
	if (!ctx.valid)
		return false;

	data.clear();

	IterateFFmpegFrames(ctx, -1, [&](FFmpegCtx& ctx) {
		ExtractFFmpegSamplesAppend<SampleT>(ctx, data);
		return true;
	});

	FreeFFmpegContext(ctx);

	return configure(data.size(), ctx.sampleRate, SampleInfo<SampleT>::format);
}

bool SoundSample::create()
{
	int secs = 1;
	float sampleRate = 44100;
	SampleFormat format = SampleFormat::Mono8;
	int size = secs * (int)sampleRate;

	data.resize(size);
	return configure(size, sampleRate, format);
}

SoundType SoundSample::getType() const
{
	return SoundType::Sample;
}

bool SoundSample::fillBuffer(detail::BaseSound::BufferT &buffer)
{
	buffer = data;
}

bool SoundSample::isAtEOF() const
{
	return true;
}

bool SoundSample::isStreaming() const
{
	return false;
}

NS_END
