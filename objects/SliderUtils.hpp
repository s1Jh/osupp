#pragma once

#include "define.hpp"
#include "Vec2.hpp"
#include "SliderTypes.hpp"
#include "Curve.hpp"

namespace NotOSU {
    class Slider;

    class SliderUtils {
    public:
        explicit SliderUtils(Slider& parent);

        [[nodiscard]] fvec2d findNormal(double t);

        /**
         * Finds point on a curve from a parameter t; t = <0;1>
         * @param t
         * @return
         */
        [[nodiscard]] fvec2d findPoint(double t);
        [[nodiscard]] fvec2d findDirection(double t);
        [[nodiscard]] fvec2d findNormal(double t);

        [[nodiscard]] float getInterpolatedLength() const;

        [[nodiscard]] const ActiveSliderPathT& getInterpolatedPath() const;

        // Turns the template path into an interpolated path based on the slider type
        void interpolatePath();

    protected:
        void interpolatePathStraight();

        [[nodiscard]] fvec2d bezierIterate(const SliderPathT& curve, double t) const;
        void interpolatePathBezier();

        void interpolatePathCatmull();

        void interpolatePathCircle();

        struct SearchResumeInfo {
            double lastCoveredLength = 0.0;
            double lastT = 0.0;
            ActiveSliderPathT::iterator resumePoint;
        };

        SearchResumeInfo pointResumeInfo, directionResumeInfo;

        unsigned int interpolationSteps;
        float templatePathLength = 0.0;
        float length = 0.0;
        ActiveSliderPathT interpolatedPath;
        Slider& parent;
    };

}
