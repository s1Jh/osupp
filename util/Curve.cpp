#include "Curve.hpp"

#include "define.hpp"

#include "Util.hpp"
#include <vector>

NS_BEGIN

    CurveNode::CurveNode(fvec2d _position) : position(_position) {}

    fvec2d CurveNode::getPosition() const {
        return position;
    }

    fvec2d BaseCurveNode::getPosition() const {
        return {0.0, 0.0};
    }

NS_END