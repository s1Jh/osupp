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

class ObjectSprite;
class ObjectDrawInfo;

namespace video {
class LambdaRender;
}

template<>
void Draw(video::LambdaRender& renderer, const ObjectSprite&, const ObjectDrawInfo&);

namespace video
{


constexpr size_t RENDER_QUEUE_SIZE = 1024;

class LambdaRender
{
public:
	struct GenericMeshCollection
	{
		Mesh rect;
		Mesh circle;
		Mesh rectMask;
		// etc...
	};

    ~LambdaRender();

    bool init();
    bool initImGui();

	[[nodiscard]] const GenericMeshCollection& getMeshes() const;
	bool configure(const WindowConfiguration& newConfig);
	[[nodiscard]] const WindowConfiguration& getConfig() const;
    [[nodiscard]] const Window& getWindow() const;

	bool update();

	void begin();
	void finish();

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
    static bool GenerateStaticGeometry(GenericMeshCollection& meshes);

	GenericMeshCollection meshes;
    Window window;
	size_t renderStackSize{0};
	std::array<std::shared_ptr<detail::BaseRenderTask>, RENDER_QUEUE_SIZE> renderQueue;
};

}

NS_END
