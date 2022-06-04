#include "SliderUtils.hpp"

#include "Slider.hpp"

namespace GAME_TITLE {

    void SliderUtils::interpolatePath() {
        parent.length = 0;
        switch (parent.objectTemplate->sliderType) {
            case SliderType::Straight:
                interpolatePathStraight();
                return;
            case SliderType::Bezier:
                interpolatePathBezier();
                return;
            case SliderType::Catmull:
                interpolatePathCatmull();
                return;
            case SliderType::Circle:
                interpolatePathCircle();
                return;
        }
    }

    void SliderUtils::interpolatePathStraight() {
        for (auto it = parent.objectTemplate->path.begin(); it != parent.objectTemplate->path.end(); it++) {
            auto &obj = *it;
            auto start = obj.position;

            float segmentLength = 0;
            GLLine connector;

            if (it != parent.objectTemplate->path.begin()) {
                // not at the start
            }
            if (it != std::prev(parent.objectTemplate->path.end())) {
                // not at the end
                auto nextIt = std::next(it);
                auto &nextObj = *nextIt;
                auto end = nextObj.position;
                segmentLength = Distance(start, end);
                connector = GLLine(start, end);
                parent.length += segmentLength;
            }
            parent.path.push_back({&obj, segmentLength, connector});
        }
    }

    void SliderUtils::interpolatePathBezier() {
        for (auto it = parent.objectTemplate->path.begin(); it != parent.objectTemplate->path.end(); it++) {

        }
    }

    void SliderUtils::interpolatePathCatmull() {

    }

    void SliderUtils::interpolatePathCircle() {

    }

    fvec2d SliderUtils::findPoint(double t) const {
        t = Clamp(t, 0.0, 1.0);
        switch (parent.objectTemplate->sliderType) {
            case SliderType::Straight:
                return _findPointStraight(t);
            case SliderType::Bezier:
                return _findPointBezier(t);
            case SliderType::Catmull:
                return _findPointCatmull(t);
            case SliderType::Circle:
                return _findPointCircle(t);
        }
    }

    fvec2d SliderUtils::findPointStraight(double t) const {
        t /= 2.0;   // FIXME: t is on <0;0.5> for this algorithm for some reason
        double coveredLength = 0.0;
        for (auto it = parent.path.begin(); it != std::prev(parent.path.end()); it++) {
            const auto &node = *it;
            double nextCoveredLength = coveredLength + node.length;
            if (t <= nextCoveredLength / parent.length) {
                // we found the right segment, now interpolate
                // we interpolate between coveredLength and nextCoveredLength
                double factorStart = coveredLength / parent.length;
                double segmentFactor = node.length / parent.length;
                t -= factorStart;
                t /= segmentFactor;
                return BiLerp(node.info->position, std::next(it)->info->position, t);
            }
            coveredLength = nextCoveredLength;
        }
    }

    fvec2d SliderUtils::findPointBezier(double t) const {
        return fvec2d();
    }

    fvec2d SliderUtils::findPointCatmull(double t) const {
        return fvec2d();
    }

    fvec2d SliderUtils::findPointCircle(double t) const {
        return fvec2d();
    }

    fvec2d SliderUtils::findDirection(double t) const {
        t = Clamp(t, 0.0, 1.0);
        switch (parent.objectTemplate->sliderType) {
            case SliderType::Straight:
                return _findDirectionStraight(t);
            case SliderType::Bezier:
                return _findDirectionBezier(t);
            case SliderType::Catmull:
                return _findDirectionCatmull(t);
            case SliderType::Circle:
                return _findDirectionCircle(t);
        }
    }

    fvec2d SliderUtils::findDirectionStraight(double t) const {
        t /= 2.0;   // FIXME: t is on <0;0.5> for this algorithm for some reason
        double coveredLength = 0.0;
        for (auto it = parent.path.begin(); it != std::prev(parent.path.end()); it++) {
            coveredLength += it->length / parent.length;
            if (t <= coveredLength) {
                // we're in the right segment
                auto next = std::next(it);
                return Normalize(next->info->position - it->info->position);
            }
        }
    }

    fvec2d SliderUtils::findDirectionBezier(double t) const {
        return fvec2d();
    }

    fvec2d SliderUtils::findDirectionCatmull(double t) const {
        return fvec2d();
    }

    fvec2d SliderUtils::findDirectionCircle(double t) const {
        return fvec2d();
    }

    fvec2d SliderUtils::findNormal(double t) const {
        return Normal(findDirection(t));
    }

    SliderUtils::SliderUtils(Slider &parent) : parent(parent) {}

}