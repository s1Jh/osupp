#include "SliderUtils.hpp"

#include "Slider.hpp"

NS_BEGIN

    void SliderUtils::interpolatePath() {
        auto& path = parent.objectTemplate->path;
        Curve<SliderPathT::iterator> templateCurve(path.begin(), path.end());
        length = 0;
        interpolatedPath.clear();

        auto lastIt = interpolatedPath.end();
        for (int i = 0; i <= interpolationSteps; i++) {
            auto t = double(i) / double(interpolationSteps);

            fvec2d thisPosition = templateCurve.get(parent.objectTemplate->sliderType, t);

            if (lastIt != interpolatedPath.end()) {
                auto segmentLength = Distance(lastIt->position, thisPosition);
                length += segmentLength; // length was already reset to zero by the calling function
                lastIt->length = segmentLength;
                lastIt->connectingLine = GLLine(lastIt->position, thisPosition);
            }

            interpolatedPath.push_back(ActiveSliderNode{thisPosition});
            lastIt = std::prev(interpolatedPath.end());
        }

        directionResumeInfo.resumePoint = interpolatedPath.begin();
        pointResumeInfo.resumePoint = interpolatedPath.begin();
    }

    fvec2d SliderUtils::findPoint(double t) {
        t = Clamp(t, 0.0, 1.0);

        // first check if the t requested isn't lower than the last t
        if (t <= pointResumeInfo.lastT) {
            // if it is, we reset the resume search state
            pointResumeInfo.resumePoint = interpolatedPath.begin();
            pointResumeInfo.lastCoveredLength = 0.0;
        }

        double coveredLength = pointResumeInfo.lastCoveredLength;

        auto lastIt = std::prev(interpolatedPath.end(), 2);
        for (auto it = pointResumeInfo.resumePoint; it != std::prev(interpolatedPath.end()); it++) {
            const auto &node = *it;
            double nextCoveredLength = coveredLength + node.length;
            // also check if we're on the last node of the path, as floating point inaccuracies may
            // cause this condition to fail
            if (t <= nextCoveredLength / length || it == lastIt) {
                // we found the right segment, now interpolate
                // we interpolate between coveredLength and nextCoveredLength
                double factorStart = coveredLength / length;
                double segmentFactor = node.length / length;

                pointResumeInfo.lastT = t;
                pointResumeInfo.resumePoint = it;
                pointResumeInfo.lastCoveredLength = coveredLength;

                t -= factorStart;
                t /= segmentFactor;
                return BiLerp(node.position, std::next(it)->position, t);
            }
            coveredLength = nextCoveredLength;
        }
        // we should never get here
        log::error("Curve calculation fell through");
        return {0.0, 0.0};
    }

    fvec2d SliderUtils::findDirection(double t) {
        t = Clamp(t, 0.0, 1.0);

        // first check if the t requested isn't lower than the last t
        if (t <= directionResumeInfo.lastT) {
            // if it is, we reset the resume search state
            directionResumeInfo.resumePoint = interpolatedPath.begin();
            directionResumeInfo.lastCoveredLength = 0.0;
        }

        // length from the start of the curve to the current node <0;1>
        double coveredLength = directionResumeInfo.lastCoveredLength;
        // the last node we will iterate
        auto lastIt = std::prev(interpolatedPath.end(), 2);

        for (auto it = directionResumeInfo.resumePoint; it != std::prev(interpolatedPath.end()); it++) {
            // add the covered length
            double nextCoveredLength = coveredLength + it->length / length;

            if (t <= nextCoveredLength) {
                // we're in the right segment
                // save the parameter t for which these resume values are valid
                directionResumeInfo.lastT = t;
                directionResumeInfo.resumePoint = it;
                directionResumeInfo.lastCoveredLength = coveredLength;

                auto next = std::next(it);
                return Normalize(next->position - it->position);
            }
            // check if we're on the last iteration of this loop, since the last node technically doesn't have
            // a direction vector, we use the vector between it and the previous node and invert the result
            if (it == lastIt) {
                auto prev = std::prev(it);
                return Normalize(it->position - prev->position);
            }
            coveredLength = nextCoveredLength;
        }
        // we should never get here
        log::error("Direction calculation fell through");
        return {0.0, 0.0};
    }

    fvec2d SliderUtils::findNormal(double t) {
        return Normal(findDirection(t));
    }

    SliderUtils::SliderUtils(Slider &parent) : parent(parent), interpolationSteps(100) {
        interpolatePath();
    }

    float SliderUtils::getInterpolatedLength() const {
        return length;
    }

    const ActiveSliderPathT &SliderUtils::getInterpolatedPath() const {
        return interpolatedPath;
    }
}