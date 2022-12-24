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

#include "GraphicsContext.hpp"
#include "Window.hpp"
#include "StandardDrawCalls.hpp"

#include <list>
#include <array>
#include <memory>

NS_BEGIN

namespace video
{

constexpr size_t RENDER_QUEUE_SIZE = 1024;

class LambdaRender
{
public:
	LambdaRender();

	struct GenericMeshCollection
	{
		Mesh rect;
		Mesh circle;
		Mesh rectMask;
		// etc...
	};

	[[nodiscard]] const GenericMeshCollection& getMeshes() const;

	Window createWindow(const WindowConfiguration& config = {});
	static bool ConfigureWindow(Window win, const WindowConfiguration& newConfig);
	static WindowConfiguration GetWindowConfig(const Window& win);

	bool ownsWindow(const Window& win);
	bool update();

	void begin();
	void finish(const Window& win);

	template <typename Arg1, typename ... Args>
	bool draw(RenderTask<Arg1, Args...> taskIn) {
		renderStackSize++;

		if (renderStackSize > renderQueue.size()) {
			renderStackSize = (int)renderQueue.size();
			return false;
		}

		renderQueue[renderStackSize - 1] = std::make_shared<RenderTask<Arg1, Args...>>(std::move(taskIn));
		return true;
	}

private:
	static bool ApplyWindowConfiguration(Window& win, const WindowConfiguration& config);
	static void ContextCloser(WindowHandle*);
	WindowHandle *createWindowHandle();

	GenericMeshCollection meshes;
	std::shared_ptr<WindowHandle> contextHolder;
	int renderStackSize{0};
	std::array<std::shared_ptr<detail::BaseRenderTask>, RENDER_QUEUE_SIZE> renderQueue;
	std::list<Window> openedWindows;
};

}

NS_END
