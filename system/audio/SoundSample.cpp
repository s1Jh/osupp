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

namespace PROJECT_NAMESPACE {

template<> const std::vector<std::string> Resource<SoundSample>::allowedExtensions = {".wav", ".mp3", ".ogg"};

template<>
Resource<SoundSample> Load(const std::filesystem::path &path)
{
	Resource<SoundSample> r;

	auto ctx = OpenFFmpegContext(path);

	if (!ctx.valid) {
		return nullptr;
	}

	auto& data = r->data;
	data.clear();

	IterateFFmpegFrames(ctx, -1, [&](FFmpegCtx& ctx) {
		ExtractFFmpegSamplesAppend<SoundSample::SampleT>(ctx, data);
		return true;
	});

	FreeFFmpegContext(ctx);

	if (!r->configure(data.size(), ctx.sampleRate, SampleInfo<SoundSample::SampleT>::format)) {
		return nullptr;
	}

	return r;
}

//template<>
//Resource<SoundSample> Create()
//{
//	Resource<SoundSample> r;
//	int secs = 1;
//	int sampleRate = 44100;
//	SampleFormat format = SampleFormat::MONO8;
//	int size = secs * (int)sampleRate;
//
//	r->data.resize(size);
//	if (!r->configure(size, sampleRate, format)) {
//		return nullptr;
//	}
//
//	return r;
//}

SoundType SoundSample::getType() const
{
	return SoundType::SAMPLE;
}

bool SoundSample::fillBuffer(detail::BaseSound::BufferT &buffer)
{
	buffer = data;
	return true;
}

bool SoundSample::isAtEOF() const
{
	return true;
}

bool SoundSample::isStreaming() const
{
	return false;
}

}
