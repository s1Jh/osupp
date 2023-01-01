#pragma once

#include "define.hpp"

#include "Mesh.hpp"
#include "Video.hpp"
#include "Line.hpp"
#include "LambdaRender.hpp"

NS_BEGIN

namespace video::helpers
{

void DrawGeneric2DShape(
    const LambdaRender &renderer,
    const Mesh &mesh, const Mat3f &shape,
    const VisualAppearance &appearance,
    const Mat3f &transform, RenderMode mode
);

void DrawLineSegment(
    const LambdaRender &renderer, const dline &seg,
    const VisualAppearance &appearance, const Mat3f &transform
);

}

NS_END
