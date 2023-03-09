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
#include "AudioUtil.hpp"

#include "Log.hpp"

namespace PROJECT_NAMESPACE::audio {

FFmpegCtx OpenFFmpegContext(const std::filesystem::path &path)
{
	av_log_set_level(AV_LOG_QUIET);

	FFmpegCtx ctx;

	int err = 0;

	auto invalidate = [&](const std::string& message = "") {
		if (err != 0) {
			char buf[128];
			av_make_error_string(buf, 128, err);
			log::Error("Failed creating a FFMPEG context: ", message, ", ", buf);
		}
		FreeFFmpegContext(ctx);
		ctx.valid = false;
		return ctx;
	};

#ifdef LINUX
	const char* ptr = path.c_str();
#else
	auto string = path.u8string();
	const char* ptr = (const char*)string.c_str();
#endif

	if ((err = avformat_open_input(&ctx.format, ptr, nullptr, nullptr)) != 0) {
		return invalidate("Error opening file.");
	}

	avformat_find_stream_info(ctx.format, nullptr);

	for(size_t i = 0; i < ctx.format->nb_streams; ++i) {
		if(ctx.format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			ctx.audioStreamIndex = (int)i;
			break;
		}
	}
	if(ctx.audioStreamIndex == -1) {
		avformat_close_input(&ctx.format);
		return invalidate("None of the available streams are audio streams");
	}

	ctx.codec = avcodec_find_decoder(ctx.format->streams[ctx.audioStreamIndex]->codecpar->codec_id);
	if (ctx.codec == nullptr) {
		avformat_close_input(&ctx.format);
		return invalidate("Decoder not found. The codec is not supported.");
	}

	ctx.codecCtx = avcodec_alloc_context3(ctx.codec);
	if (ctx.codecCtx == nullptr) {
		avformat_close_input(&ctx.format);
		return invalidate("Could not allocate a decoding context.");
	}

	if ((err = avcodec_parameters_to_context(ctx.codecCtx, ctx.format->streams[ctx.audioStreamIndex]->codecpar)) != 0) {
		avcodec_close(ctx.codecCtx);
		avcodec_free_context(&ctx.codecCtx);
		avformat_close_input(&ctx.format);
		return invalidate();
	}

	ctx.codecCtx->request_sample_fmt = av_get_alt_sample_fmt(ctx.codecCtx->sample_fmt, 0);

	if ((err = avcodec_open2(ctx.codecCtx, ctx.codec, nullptr)) != 0) {
		avcodec_close(ctx.codecCtx);
		avcodec_free_context(&ctx.codecCtx);
		avformat_close_input(&ctx.format);
		return invalidate();
	}

	ctx.frame = nullptr;
	if ((ctx.frame = av_frame_alloc()) == nullptr) {
		avcodec_close(ctx.codecCtx);
		avcodec_free_context(&ctx.codecCtx);
		avformat_close_input(&ctx.format);
		return invalidate();
	}

	ctx.packet = av_packet_alloc();

	ctx.sampleRate = ctx.codecCtx->sample_rate;

	return ctx;
}

size_t IterateFFmpegFrames(FFmpegCtx &ctx, size_t number, const std::function<bool(FFmpegCtx &)>& func)
{
	int err;
	size_t read = 0;

	while (read < number) {
		if ((err = av_read_frame(ctx.format, ctx.packet)) == AVERROR_EOF) {
			ctx.eof = true;
			break;
		}
		if(err != 0) {
			// Something went wrong.
			break; // Don't return, so we can clean up nicely.
		}
		// Does the packet belong to the correct stream?
		if(ctx.packet->stream_index != ctx.audioStreamIndex) {
			// Free the buffers used by the frame and reset all fields.
			av_packet_unref(ctx.packet);
			continue;
		}
		// We have a valid packet => send it to the decoder.
		if((err = avcodec_send_packet(ctx.codecCtx, ctx.packet)) == 0) {
			// The packet was sent successfully. We don't need it anymore.
			// => Free the buffers used by the frame and reset all fields.
			av_packet_unref(ctx.packet);
		} else {
			break; // Don't return, so we can clean up nicely.
		}

		// Receive and handle frames.
		// EAGAIN means we need to send before receiving again. So thats not an error.

		while((err = avcodec_receive_frame(ctx.codecCtx, ctx.frame)) == 0) {
			if (!func(ctx))
				return read;
			read++;

			av_frame_unref(ctx.frame);
		}

		if(err != AVERROR(EAGAIN)) {
			// Not EAGAIN => Something went wrong.
			break; // Don't return, so we can clean up nicely.
		}

	}
	return read;
}

bool IsPlanar(AVSampleFormat fmt)
{
	return av_sample_fmt_is_planar(fmt);
}

void FreeFFmpegContext(FFmpegCtx &ctx)
{
//	log::Debug("Destroying context ", &ctx);
	av_frame_free(&ctx.frame);
	avformat_free_context(ctx.format);
	avcodec_free_context(&ctx.codecCtx);
	av_packet_free(&ctx.packet);
}


}

