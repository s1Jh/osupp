#pragma once

#include "define.hpp"

#include "SliderTypes.hpp"
#include "RenderTask.hpp"
#include "Texture.hpp"
#include "ObjectSprite.hpp"
#include "Shader.hpp"

NS_BEGIN

struct SliderTrailDrawInfo {
    float start;
    float end;
    bool useTexture;
    float thickness;
    video::Texture *bakedTexture;
    Resource<video::Shader> shader;
    float alpha;
    ObjectSprite *sprite;
    Mat3f transform;
};

template<>
void Draw(
    video::LambdaRender &renderer,
    const math::Curve<SliderPathT::iterator> &trail,
    const SliderTrailDrawInfo &info
);

using DrawSliderTrail = video::RenderTask<const math::Curve<SliderPathT::iterator> &, const SliderTrailDrawInfo&>;

NS_END
