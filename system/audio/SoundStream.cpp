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

template<> const std::vector<std::string> Resource<SoundStream>::allowedExtensions = {".wav", ".mp3", ".ogg"};

template<>
Resource<SoundStream> Load(const std::filesystem::path &path)
{
	Resource<SoundStream> r;
	r->ctx = OpenFFmpegContext(path);

	if (r->ctx.valid && r->configure(0, r->ctx.sampleRate, SampleInfo<SoundStream::SampleT>::format))
		return r;

	return nullptr;
}

template<>
Resource<SoundStream> Create()
{
	Resource<SoundStream> r;
	r->ctx.valid = false;
	return r;
}

SoundType SoundStream::getType() const
{
	return SoundType::STREAM;
}

bool SoundStream::fillBuffer(detail::BaseSound::BufferT &buffer)
{
	if (!ctx.valid)
		return false;

	buffer.clear();
	IterateFFmpegFrames(ctx, 100, [&](FFmpegCtx& context) {
		ExtractFFmpegSamplesAppend<SampleT>(context, buffer);
		return true;
	});
	return true;
}

bool SoundStream::isAtEOF() const
{
	return ctx.eof && ctx.valid;
}

bool SoundStream::isStreaming() const
{
	return true;
}

SoundStream::~SoundStream()
{
	FreeFFmpegContext(ctx);
}

void SoundStream::reset()
{
	if (ctx.valid)
		av_seek_frame(ctx.format, ctx.audioStreamIndex, 0, 0);
}

NS_END
