#pragma once

#include "define.hpp"

#include "SliderTypes.hpp"
#include "RenderTask.hpp"
#include "Texture.hpp"
#include "ObjectSprite.hpp"
#include "Shader.hpp"

namespace PROJECT_NAMESPACE {

struct SliderTrailDrawInfo {
    bool useTexture{false};
    Resource<video::Texture> bakedTexture{nullptr};

    Resource<video::Texture> trailTexture{nullptr};

    float start{0.f};
    float end{1.f};

    float thickness{1.f};
    color tint{};

    Mat3f transform{};
};

video::Texture DrawTrailToTexture(
    video::LambdaRender &renderer,
    math::Curve<SliderPathT::iterator> &trail,
    SliderTrailDrawInfo info
);

template<>
void Draw(
    video::LambdaRender &renderer,
    const math::Curve<SliderPathT::iterator> &trail,
    const SliderTrailDrawInfo &info
);

using DrawSliderTrail = video::RenderTask<const math::Curve<SliderPathT::iterator> &, const SliderTrailDrawInfo&>;

}
