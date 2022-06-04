#pragma once

#include "define.hpp"

namespace NotOSU {
    class Slider;

    class SliderUtils {
    public:
        explicit SliderUtils(Slider& parent);

        [[nodiscard]] fvec2d findNormal(double t) const;

        [[nodiscard]] fvec2d findPoint(double t) const;

        [[nodiscard]] fvec2d findDirection(double t) const;

        // Turns the template path into an interpolated path based on the slider type
        void interpolatePath();

    protected:
        void interpolatePathStraight();

        void interpolatePathBezier();

        void interpolatePathCatmull();

        void interpolatePathCircle();

        [[nodiscard]] fvec2d findPointStraight(double t) const;

        [[nodiscard]] fvec2d findPointBezier(double t) const;

        [[nodiscard]] fvec2d findPointCatmull(double t) const;

        [[nodiscard]] fvec2d findPointCircle(double t) const;

        [[nodiscard]] fvec2d findDirectionStraight(double t) const;

        [[nodiscard]] fvec2d findDirectionBezier(double t) const;

        [[nodiscard]] fvec2d findDirectionCatmull(double t) const;

        [[nodiscard]] fvec2d findDirectionCircle(double t) const;

        Slider& parent;
    };

}
