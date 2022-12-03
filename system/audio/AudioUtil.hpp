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

#include <filesystem>
#include <fstream>
#include <functional>

#include "define.hpp"
#include "Audio.hpp"
#include "Log.hpp"
#include "Math.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}

NS_BEGIN

struct FFmpegCtx {
	bool valid{true};
	bool eof{false};

	int audioStreamIndex{-1};
	const AVCodec* codec{nullptr};
	AVPacket* packet{nullptr};
	AVFrame* frame{nullptr};
	AVFormatContext *format{nullptr};
	AVCodecContext* codecCtx{nullptr};

	int sampleRate{0};
};


FFmpegCtx OpenFFmpegContext(const std::filesystem::path& path);
size_t IterateFFmpegFrames(FFmpegCtx& context, size_t number, const std::function<bool(FFmpegCtx&)>&);
void FreeFFmpegContext(FFmpegCtx& ctx);

bool IsPlanar(AVSampleFormat fmt);

template <typename SampleT>
void ExtractFFmpegSamplesAppend(FFmpegCtx& ctx, std::vector<SampleT>& read)
{
	const SampleFormat fmt = SampleInfo<SampleT>::format;

	auto extractDataFromStream = [&](size_t channel, size_t offset) -> float {
		channel--;
		auto avFormat = (AVSampleFormat)ctx.frame->format;
		auto **data = ctx.frame->extended_data;

		uint8_t *stream;
		if (!IsPlanar(avFormat)) {
			// Format is interleaved
			stream = data[0];
#ifdef WINDOWS
			offset = offset * ctx.frame->channels + channel;
#else
			offset = offset * ctx.frame->ch_layout.nb_channels + channel;
#endif
		} else {
			// Format is planar
			stream = data[channel];
		}

		auto convertToRange = []<typename T>(T value) -> float {
			auto converted = float(value) / float(std::numeric_limits<T>::max());

			return converted;
		};

		switch (avFormat) {
		case AVSampleFormat::AV_SAMPLE_FMT_U8:
		case AVSampleFormat::AV_SAMPLE_FMT_U8P: {
			auto value = reinterpret_cast<uint8_t*>(stream)[offset];
			auto signedValue = int8_t(int(value) - 127);
			return convertToRange(signedValue);
		}
		case AVSampleFormat::AV_SAMPLE_FMT_S16:
		case AVSampleFormat::AV_SAMPLE_FMT_S16P: {
			auto value = reinterpret_cast<int16_t*>(stream)[offset];
			return convertToRange(value);
		}
		case AVSampleFormat::AV_SAMPLE_FMT_S32:
		case AVSampleFormat::AV_SAMPLE_FMT_S32P: {
			auto value = reinterpret_cast<int32_t*>(stream)[offset];
			return convertToRange(value);
		}
		case AVSampleFormat::AV_SAMPLE_FMT_S64:
		case AVSampleFormat::AV_SAMPLE_FMT_S64P: {
			auto value = reinterpret_cast<int64_t*>(stream)[offset];
			return convertToRange(value);
		}

		case AVSampleFormat::AV_SAMPLE_FMT_FLT:
		case AVSampleFormat::AV_SAMPLE_FMT_FLTP:
			return reinterpret_cast<float*>(stream)[offset];

		case AVSampleFormat::AV_SAMPLE_FMT_DBL:
		case AVSampleFormat::AV_SAMPLE_FMT_DBLP:
			return (float)reinterpret_cast<double*>(stream)[offset];

		default:
			log::error("Unknown sample format: ", avFormat);
			return 0;
		}
	};

	/**
	 * Converts a float value in range [1;-1] to a unsigned 8 or 16 bit integer.
	 */
	auto crushData = [&](float dataIn) -> auto {
		if constexpr ((fmt & SampleFormat::TYPE) == SampleFormat::TYPE8) {
			// Crush data to uint8_t
			dataIn = (dataIn + 1.0f) / 2.0f;
			dataIn = math::Clamp(dataIn, 0.0f + std::numeric_limits<float>::epsilon(),
						   1.0f - std::numeric_limits<float>::epsilon());

			return uint8_t(std::ceil(dataIn * std::numeric_limits<uint8_t>::max()));

		} else if constexpr ((fmt & SampleFormat::TYPE) == SampleFormat::TYPE16) {
			// Crush data to int16_t
			dataIn = math::Clamp(dataIn, -1.0f + std::numeric_limits<float>::epsilon(),
						   1.0f - std::numeric_limits<float>::epsilon());

			return int16_t(dataIn * std::numeric_limits<int16_t>::max());

		} else {
			WRAP_CONSTEXPR_ASSERTION("Incorrect sample format type");
		}
	};

	for (int i = 1; i < ctx.frame->nb_samples; i++) {
		if constexpr(bool(fmt & SampleFormat::IS_STEREO)) {
#ifdef WINDOWS
			auto channelCount = ctx.frame->channels;
#else
			auto channelCount = ctx.frame->ch_layout.nb_channels;
#endif
			const size_t rightId = math::Min(2, channelCount);
			const size_t leftId = math::Min(1, channelCount);

			float left = extractDataFromStream(leftId, i);
			float right = extractDataFromStream(rightId, i);
			read.emplace_back(SampleT{crushData(left), crushData(right)});
		}
		else {
			const size_t centerId = 1;

			float mono = extractDataFromStream(centerId, i);
			read.emplace_back(crushData(mono));
		}
	}
}

template <typename SampleT>
std::vector<SampleT> ExtractFFmpegSamples(FFmpegCtx& ctx) {
	std::vector<SampleT> read;
	ExtractFFmpegSamplesAppend(ctx, read);
	return read;
}

NS_END