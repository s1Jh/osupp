#include "Slider.hpp"

#include <utility>

NS_BEGIN

    void Slider::onUpdate(double delta) {
        double timeLength = getEndTime() - getStartTime();

        double timeRunning = session->getCurrentTime() - getStartTime();
        timeRunning = Clamp(timeRunning, 0.0, timeLength);

        progression = timeRunning / timeLength;

        auto repeats = objectTemplate->repeats;
        auto currentRepeat = (unsigned int)(timeRunning * repeats / timeLength);
        curvePosition = std::fmod(progression * repeats, 1.0);

        if (currentRepeat % 2 == 1) {
            currentDirection = TravelDirection::Backward;
        }
        else {
            currentDirection = TravelDirection::Forward;
        }

        if (currentDirection == TravelDirection::Backward) {
            curvePosition = 1 - curvePosition;
        }

        repeatsLeft = repeats - currentRepeat;

        if (repeatsLeft == 0.0) {
            transferToPickup(session->getCurrentTime());
            SOF.position = getEndPosition();
        } else {
            SOF.position = curve.get<CurveType::Straight>(curvePosition);
        }
    }

    void Slider::onBegin() {
        started = true;
    }

    HitResult Slider::onFinish() {
        if (!started)
            return HitResult::Missed;
    }

    void Slider::onRaise() {
        currentDirection = TravelDirection::Backward;
    }

    void Slider::onPress() {
        currentDirection = TravelDirection::Forward;
    }

    Slider::Slider(std::shared_ptr<ObjectTemplateSlider> templ, BaseGameMode *g)
            : HitObject(std::move(templ), g), stepsPerCurveUnit(10),
              progression(0.0), started(false), currentDirection(TravelDirection::Forward),
              epsilon(0.01), curvePosition(0.0) {

        /*============================================================================================================*/
        // Initialize the variables to their defaults.
        SOF = {session->getMap()->getCircleSize(), {objectTemplate->path.front().position}};
        repeatsLeft = objectTemplate->repeats;
        // we need to cover a distance of 1, simply use v=s/t
        velocity = 1.0 / (this->getEndTime() - this->getStartTime());

        /*============================================================================================================*/
        // Create curve out of the template points for interpolating.
        auto& path = objectTemplate->path;
        Curve<SliderPathT::iterator> templateCurve(path.begin(), path.end());

        // Prepare some misc variables.
        interpolatedPath.clear();
        auto lastIt = interpolatedPath.end();
        auto middleIt = interpolatedPath.end();
        auto steps = Max((unsigned int)(stepsPerCurveUnit * templateCurve.getLength()), 2);

        struct Circle {
            dcircle AOI;
            std::vector<fvec2d> points = {};
        };
        std::vector<fcircle> circleMask;
        circleMask.reserve(steps);

        // Interpolate over n * length steps.
        for (int i = 0; i <= steps; i++) {
            middleIt = std::prev(interpolatedPath.end(), 1);
            lastIt = std::prev(interpolatedPath.end(), 2);

            auto t = double(i) / double(steps);

            fvec2d thisPosition = templateCurve.get(objectTemplate->sliderType, t);

            Circle mask;
            mask.AOI = {session->getMap()->getCircleSize(), thisPosition};
            circleMask.emplace_back(session->getMap()->getCircleSize() - 0.01f, thisPosition);

            // Optimizations
            if (middleIt != interpolatedPath.end()) {
                if (lastIt != interpolatedPath.end()) {
                    // Optimization #1: If a triplet of sequential points has a small enough angle,
                    //                  remove the middle point of this triplet.
                    auto lastPosition = lastIt->position;
                    auto midPosition = middleIt->position;

                    auto vec1 = Normalize(lastPosition - midPosition);
                    auto vec2 = Normalize(thisPosition - midPosition);

                    auto angle = Abs(Cross(vec1, vec2));
                    if (angle <= 0.01) {
                        interpolatedPath.erase(middleIt);
                    }
                }

                // Optimization #2: If the new point is too close to an existing point, don't add it.
                if (Distance(thisPosition, middleIt->position) <= 0.01) {
                    continue;
                }
            }
            interpolatedPath.push_back({thisPosition, false});
        }

        /*============================================================================================================*/
        // Finish setup by setting up our interpolated curve and constant variables.
        curve.setIteratorRange(interpolatedPath.begin(), interpolatedPath.end());

        auto startBumperDirection = findNormal(0.0);
        startBumperAngle = std::atan2(startBumperDirection.y, startBumperDirection.x);
        auto endBumperDirection = findNormal(1.0);
        // Needs to be flipped, add π.
        endBumperAngle = std::atan2(endBumperDirection.y, endBumperDirection.x) + fPI;

        /*============================================================================================================*/
        // Generate the slider mesh.

        for (auto it = circleMask.begin(); it != circleMask.end(); it++) {
            auto circleSize = session->getMap()->getCircleSize();
            fvec2d vector = {0.0, circleSize};
            auto iterations = 16;
            auto angle = 2*PI / double(iterations);

            for (int j = 0; j <= iterations; j++) {
                bool intersects = false;
                auto point = vector + it->position;

                for (auto searchIt = it; searchIt != std::prev(circleMask.begin()); searchIt--) {
                    if (Distance(point, searchIt->position) < searchIt->radius) {
                        intersects = true;
                        log::debug("forward distance: ", Distance(point, searchIt->position));
                        break;
                    }
                }

                for (auto searchIt = it; searchIt != circleMask.end(); searchIt++) {
                    if (Distance(point, searchIt->position) < searchIt->radius) {
                        intersects = true;
                        log::debug("backward distance: ", Distance(point, searchIt->position));
                        break;
                    }
                }

                if (!intersects)
                    circlePoints.push_back(point);

                vector = Rotate(vector, angle);
            }
        }
    }

    void Slider::onDraw(Renderer &renderer) {
        /*============================================================================================================*/
        // Prepare some constant variables.
        const auto objectTransform = session->getObjectTransform();
        auto alpha = (float) getAlpha();

        color tint = WHITE;
        tint.a = alpha;

        /*============================================================================================================*/
        // Draw the body mesh behind everything else.

        renderer.drawMesh(
                body, StandardResources::SliderShader,
                {{"tint", &tint}},
                {{0, &StandardResources::SliderBody}},
                objectTransform
        );

        /*============================================================================================================*/
        // Draw the start decorations.
        auto startBumperPosition = interpolatedPath.front().position;

        renderer.drawRect(
                {{SOF.radius, SOF.radius}, startBumperPosition},
                {
                    .texture = &StandardResources::SliderHead,
                    .fillColor = tint
                },
                (Mat3f) Transform2D{
                    .rotate = startBumperAngle,
                    .rotationCenter = startBumperPosition
                } * objectTransform
        );

        if (repeatsLeft >= 2) {
            renderer.drawRect(
                    {{SOF.radius, SOF.radius}, startBumperPosition},
                    {
                            .texture = &StandardResources::SliderHeadRepeat,
                            .fillColor = tint
                    },
                    (Mat3f) Transform2D{
                            .rotate = startBumperAngle,
                            .rotationCenter = startBumperPosition
                    } * objectTransform
            );
        }

        /*============================================================================================================*/
        // Draw the end decorations.
        auto endBumperPosition = interpolatedPath.back().position;

        renderer.drawRect(
                {{SOF.radius, SOF.radius}, endBumperPosition},
                {
                    .texture = &StandardResources::SliderTail,
                    .fillColor = tint
                },
                (Mat3f) Transform2D{
                    .rotate = startBumperAngle,
                    .rotationCenter = endBumperPosition
                } * objectTransform
        );

        if (repeatsLeft >= 3) {
            renderer.drawRect(
                    {{SOF.radius, SOF.radius}, endBumperPosition},
                    {
                            .texture = &StandardResources::SliderTailRepeat,
                            .fillColor = tint
                    },
                    (Mat3f) Transform2D{
                            .rotate = endBumperAngle,
                            .rotationCenter = endBumperPosition
                    } * objectTransform
            );
        }

        /*============================================================================================================*/
        // Draw the ball moving across the slider.
        auto ballDirection = findNormal(curvePosition);
        auto ballAngle = std::atan2(ballDirection.y, ballDirection.x);

        renderer.drawRect(
                {{SOF.radius, SOF.radius}, SOF.position},
                {
                    .texture = &StandardResources::SliderBall,
                    .fillColor = tint
                },
                (Mat3f) Transform2D{
                    .rotate = ballAngle,
                    .rotationCenter = SOF.position
                } * objectTransform
        );

        for (const auto &node: interpolatedPath) {
            /*========================================================================================================*/
            // Draw the interpolated path. (debug only)
            renderer.drawCross(node.position, float(session->getMap()->getCircleSize() * 0.1),
                               {.fillColor = ORANGE}, objectTransform);

            /*========================================================================================================*/
            // Draw the bonus point.
            if (node.bonus) {
                renderer.drawRect(
                        {{SOF.radius, SOF.radius}, node.position},
                        {
                            .texture = &StandardResources::SliderHitPoint,
                            .fillColor = tint
                        }, objectTransform
                );
            }
        }

        for (const auto& point : leftSide) {
            renderer.drawCross(point, float(session->getMap()->getCircleSize() * 0.1),
                               {.fillColor = YELLOW}, objectTransform);
        }
        for (const auto& point : rightSide) {
            renderer.drawCross(point, float(session->getMap()->getCircleSize() * 0.1),
                               {.fillColor = GREEN}, objectTransform);
        }
        for (const auto& point : circlePoints) {
            renderer.drawCross(point, float(session->getMap()->getCircleSize() * 0.1),
                               {.fillColor = BLUE}, objectTransform);
        }
    }

    void Slider::onReset() {
        progression = 0.0;
        SOF.position = getStartPosition();
    }

    fvec2d Slider::findDirection(double t) {
        // Clamp the parameter to <0; t-ε>.
        // This is done so that if t=1, calling curve.get(t+ε) would yield the same result as calling curve.get(t).
        // because the value of t is clamped in the calculation.
        t = Clamp(t, 0.0, 1.0 - epsilon);

        // We find the direction vector by pointing a vector from the requested parametric position t and
        // another position equal to t + ε, with ε being a small positive offset. We normalize this vector and return.
        auto targetPosition = curve.get<CurveType::Straight>(t);
        auto offsetPosition = curve.get<CurveType::Straight>(t + epsilon);
        return Normalize(offsetPosition - targetPosition);
    }

    fvec2d Slider::findNormal(double t) {
        return Normal(findDirection(t));
    }

    fvec2d Slider::getStartPosition() const {
        return interpolatedPath.front().position;
    }

    fvec2d Slider::getEndPosition() const {
        if (objectTemplate->repeats % 2 == 1) {
            return interpolatedPath.back().position;
        } else {
            return interpolatedPath.front().position;
        }
    }
}