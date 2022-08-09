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

#include "SoundStream.hpp"

#include <libavcodec/avcodec.h>

NS_BEGIN

bool SoundStream::load(const std::filesystem::path &path)
{
	ctx = OpenFFmpegContext(path);
	return ctx.valid && configure(0, ctx.sampleRate, SampleInfo<SampleT>::format);
}

bool SoundStream::create()
{
	ctx.valid = false;
	return true;
}

SoundType SoundStream::getType() const
{
	return SoundType::Stream;
}

bool SoundStream::fillBuffer(detail::BaseSound::BufferT &buffer)
{
	if (!ctx.valid)
		return false;

	buffer.clear();
	IterateFFmpegFrames(ctx, 200, [&](FFmpegCtx& context) {
		ExtractFFmpegSamplesAppend<SampleT>(context, buffer);
		return true;
	});
	return true;
}

bool SoundStream::isAtEOF() const
{
	return false;
}

bool SoundStream::isStreaming() const
{
	return true;
}
SoundStream::~SoundStream()
{
	FreeFFmpegContext(ctx);
}

NS_END
