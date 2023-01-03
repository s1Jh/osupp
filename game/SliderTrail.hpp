#pragma once

#include "define.hpp"

#include "SliderTypes.hpp"
#include "RenderTask.hpp"
#include "Texture.hpp"
#include "ObjectSprite.hpp"
#include "Shader.hpp"

NS_BEGIN

struct SliderTrailDrawInfo {
    bool useTexture;
    video::Texture *bakedTexture;

    video::Texture *trailTexture;

    float start;
    float end;

    float thickness;
    color tint;

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

NS_END
