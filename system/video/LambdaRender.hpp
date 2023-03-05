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

#include "StandardDrawCalls.hpp"
#include "Camera.hpp"
#include "Matrix.hpp"

#include <list>
#include <array>
#include <vector>
#include <memory>

namespace osupp::video
{
    // TODO: Camera
    class LambdaRender
    {
        typedef void* WindowHandle;
        typedef void* GLContext;
        typedef void* ImHandle;
    public:
        struct GenericMeshCollection
        {
            Mesh rect;
            Mesh circle;
            Mesh rectMask;
            Mesh screenRect;
            Mesh screenRectMask;
            // etc...
        };

        ~LambdaRender();

        bool init(uint8_t msLevels = 0);
        bool initImGui();

        bool configure(const WindowConfiguration &newConfig);

        [[nodiscard]] const GenericMeshCollection &getMeshes() const;
        [[nodiscard]] const WindowConfiguration &getConfig() const;
        [[nodiscard]] const isize& getSize() const;

        [[nodiscard]] bool isOpen() const;
        [[nodiscard]] irect getWindowRect();
        [[nodiscard]] Mat3f getWindowMatrix();

        bool update();

        void begin();
        void finish();

        template <typename Arg1, typename... Args>
        bool draw(RenderTask<Arg1, Args...> taskIn, uint8_t layer = DEFAULT_RENDER_LAYER)
        {
            renderStackSize++;

            if (renderStackSize > renderQueue.size())
            {
                renderStackSize = (int)renderQueue.size();
                return false;
            }

            taskIn.layer = layer;

            renderQueue[renderStackSize - 1] = std::make_shared<RenderTask<Arg1, Args...>>(std::move(taskIn));
            return true;
        }

        Camera2D camera;

    private:
        struct RenderLayer
        {
        public:
            ~RenderLayer();

            bool initialize(const isize &resolution);

            bool bind();

            static bool unbind();

            bool init = false;
            bool used = false;
            unsigned int frame = 0, color = 0, depth = 0;
        };

        bool generateStaticGeometry();

        Shader layerShader{};
        GenericMeshCollection meshes{};
        WindowConfiguration currentConfig{};
        GLContext glContext;
        ImHandle imCtx{nullptr};
        WindowHandle window{nullptr};
        size_t renderStackSize{0};
        std::array<std::shared_ptr<detail::BaseRenderTask>, RENDER_QUEUE_SIZE> renderQueue{};
        std::array<RenderLayer, RENDER_LAYERS> layers{};
    };

}
