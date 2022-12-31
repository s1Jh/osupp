//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#pragma once

#include "define.hpp"

#include "Size.hpp"
#include "GraphicsContext.hpp"
#include "imgui_internal.h"

#include <atomic>
#include <memory>

NS_BEGIN

namespace video
{

enum class WindowMode
{
	NONE, WINDOWED, FULLSCREEN, WINDOWED_BORDERLESS
};

enum class WindowVisibility
{
	NONE, VISIBLE, TASKBAR, HIDDEN
};

constexpr isize DEFAULT_WINDOW_SIZE = {640, 480};

constexpr int DEFAULT_WINDOW_MONITOR = 0;

constexpr int DEFAULT_WINDOW_REFRESH_RATE = 60;

constexpr WindowMode DEFAULT_WINDOW_VIDEO_MODE = WindowMode::WINDOWED;

constexpr WindowVisibility DEFAULT_WINDOW_VISIBILITY = WindowVisibility::HIDDEN;

struct WindowConfiguration
{
	isize size{-1, -1};
	int refreshRate{-1};
	int monitorID{-1};
	WindowMode mode{WindowMode::NONE};
	WindowVisibility shown{WindowVisibility::NONE};
};

struct Window
{
	friend class LambdaRender;
public:
	[[nodiscard]] inline bool isOpen() const { return open; }
	inline void close() { wantsToBeOpen = false; }
	[[nodiscard]] inline const WindowConfiguration& getConfig() const { return config; }
    [[nodiscard]] inline const isize& size() const { return config.size; }
    [[nodiscard]] inline const WindowHandle* getHandle() const { return handle; }

private:
	Window() = default;
	std::atomic<bool> open{false};
	std::atomic<bool> wantsToBeOpen{true};
	WindowHandle *handle{nullptr};
	ImGuiContext *imCtx{nullptr};
	WindowConfiguration config {
		DEFAULT_WINDOW_SIZE,
		DEFAULT_WINDOW_REFRESH_RATE,
		DEFAULT_WINDOW_MONITOR,
		DEFAULT_WINDOW_VIDEO_MODE,
		DEFAULT_WINDOW_VISIBILITY
	};
};

}

NS_END
