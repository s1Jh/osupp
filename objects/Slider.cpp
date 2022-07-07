#include "Slider.hpp"

#include <utility>

NS_BEGIN

void Slider::onUpdate(double delta)
{
    // update texture animations

    // bodyTexture.update(delta);
    ball.update(delta);
    head.update(delta);
    headRepeat.update(delta);
    tail.update(delta);
    tailRepeat.update(delta);
    hitPoint.update(delta);
}

void Slider::onLogicUpdate(double delta)
{
    double timeLength = getEndTime() - getStartTime();

    double timeRunning = session->getCurrentTime() - getStartTime();
    timeRunning = Clamp(timeRunning, 0.0, timeLength);

    progression = timeRunning / timeLength;

    auto repeats = objectTemplate->repeats;
    auto currentRepeat = (unsigned int) (timeRunning * repeats / timeLength);
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
        transferToPickup();
        SOF.position = getEndPosition();
    }
    else {
        SOF.position = curve.get<CurveType::Straight>(curvePosition);
    }
}

void Slider::onBegin()
{ started = true; }

HitResult Slider::onFinish()
{
    if (!started)
        return HitResult::Missed;

    return HitResult::Missed;
}

void Slider::onRaise()
{ currentDirection = TravelDirection::Backward; }

void Slider::onPress()
{ currentDirection = TravelDirection::Forward; }

Slider::Slider(std::shared_ptr<ObjectTemplateSlider> templ, BaseGameMode *g)
    : HitObject(std::move(templ), g), stepsPerCurveUnit(10), epsilon(0.01),
      currentDirection(TravelDirection::Forward), started(false), progression(0.0),
      curvePosition(0.0)
{

    auto &res = session->getResources();
    bodyShader = res.shaders.get(SLIDER_SHADER);

    auto skin = session->getActiveSkin();

    bodyTexture = skin->createObjectSprite(SLIDER_BODY_SPRITE);
    ball = skin->createObjectSprite(SLIDER_BALL_SPRITE);
    head = skin->createObjectSprite(SLIDER_HEAD_SPRITE);
    headRepeat = skin->createObjectSprite(SLIDER_HEAD_REPEAT_SPRITE);
    tail = skin->createObjectSprite(SLIDER_TAIL_SPRITE);
    tailRepeat = skin->createObjectSprite(SLIDER_TAIL_REPEAT_SPRITE);
    hitPoint = skin->createObjectSprite(SLIDER_HIT_POINT_SPRITE);

    /*============================================================================================================*/
    // Initialize the variables to their defaults.
    SOF = {session->getCircleSize(),
           {objectTemplate->path.front().position}};
    repeatsLeft = objectTemplate->repeats;
    // we need to cover a distance of 1, simply use v=s/t
    velocity = 1.0 / (this->getEndTime() - this->getStartTime());

    /*============================================================================================================*/
    // Create curve out of the template points for interpolating.
    auto &path = objectTemplate->path;
    Curve<SliderPathT::iterator> templateCurve(path.begin(), path.end());

    // Prepare some misc variables.
    interpolatedPath.clear();
    auto lastIt = interpolatedPath.end();
    auto middleIt = interpolatedPath.end();
    auto steps =
        Max((unsigned int) (stepsPerCurveUnit * templateCurve.getLength()), 2);

    struct Circle
    {
        dcircle AOI;
        std::vector<fvec2d> points = {};
    };
    circleMask.reserve(steps);

    auto intersects = [&](fvec2d position) -> bool
    {
        return std::any_of(
            circleMask.begin(), circleMask.end(), [&](const auto &circle)
            {
                return Distance(position, circle.position) <= circle.radius;
            });
    };
    // Interpolate over n * length steps.
    for (unsigned int i = 0; i <= steps; i++) {
        middleIt = std::prev(interpolatedPath.end(), 1);
        lastIt = std::prev(interpolatedPath.end(), 2);

        auto t = double(i) / double(steps);

        fvec2d thisPosition = templateCurve.get(objectTemplate->sliderType, t);

        meshSpine.push_back(thisPosition);

        // Optimizations
        if (middleIt != interpolatedPath.end()) {
            if (lastIt != interpolatedPath.end()) {
                // Optimization #1: If a triplet of sequential points has a small enough
                // angle,
                //                  remove the middle point of this triplet.
                auto lastPosition = lastIt->position;
                auto midPosition = middleIt->position;

                auto vec1 = Normalize(lastPosition - midPosition);
                auto vec2 = Normalize(thisPosition - midPosition);

                auto angle = Abs(Cross(vec1, vec2));
                if (angle <= 0.005) {
                    interpolatedPath.erase(middleIt);
                }
            }

            // Optimization #2: If the new point is too close to an existing point,
            // don't add it.
            if (Distance(thisPosition, middleIt->position) <= 0.1) {
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

    for (unsigned int i = 0; i < steps; i++) {
        auto t = double(i) / double(steps);
        auto position = curve.get<CurveType::Straight>(t);
        circleMask.emplace_back(float(session->getCircleSize()) -
                                    std::numeric_limits<float>::epsilon(),
                                position);
    }

    body.clear();
    body.setAttributeDescriptors({
                                     AttributeType::Vec2, // position
                                     AttributeType::Vec2, // UV
                                 });

    for (auto &mask: interpolatedPath) {
        fvec2d vector = {0.0, session->getCircleSize()};
        auto iterations = 16;
        auto angle = 2 * PI / double(iterations);

        fvec2d lastPoint;
        auto centerIndex = body.insertVertex({mask.position.x, mask.position.y});
        bool first = true;
        for (int j = 0; j <= iterations; j++) {
            auto point = vector + mask.position;

            if (!intersects(point)) {
                meshOutline.push_back(point);
                auto currentIndex = body.insertVertex({point.x, point.y});

                if (!first) {
                    body.insertIndices({centerIndex, currentIndex, currentIndex - 1});
                }

                first = false;
                lastPoint = point;
            }
            else {
                // we have broken a chain of triangles, start again
                first = true;
            }

            vector = Rotate(vector, angle);
        }
    }

    bool firstLeft = true;
    bool firstRight = true;

    fvec2d lastLeft;
    unsigned lastLeftIndex;
    fvec2d lastRight;
    unsigned lastRightIndex;
    fvec2d lastCenter;
    unsigned lastCenterIndex;

    for (unsigned int i = 0; i < steps; i++) {

        double t = double(i) / double(steps);
        auto normal = findNormal(t);
        fvec2d point = curve.get<CurveType::Straight>(t);

        auto size = session->getCircleSize();
        fvec2d left = point + normal * size;
        fvec2d right = point + normal * -size;

        auto currentCenter = body.insertVertex({point.x, point.y});

        if (!intersects(left)) {
            meshOutline.push_back(left);

            if (!firstLeft) {
                auto current = body.insertVertex({left.x, left.y});

                body.insertIndices({currentCenter, current, lastLeftIndex});
                body.insertIndices({currentCenter, lastCenterIndex, lastLeftIndex});

                lastLeftIndex = current;
            }

            firstLeft = false;
            lastLeft = left;
        }
        else {
            firstLeft = true;
        }

        if (!intersects(right)) {
            meshOutline.push_back(right);

            if (!firstRight) {
                auto current = body.insertVertex({right.x, right.y});

                body.insertIndices({currentCenter, current, lastRightIndex});
                body.insertIndices({currentCenter, lastCenterIndex, lastRightIndex});

                lastRightIndex = current;
            }

            firstRight = false;
            lastRight = right;
        }
        else {
            firstRight = true;
        }

        lastCenterIndex = currentCenter;
    }

    /*auto copy = meshOutline;

    fvec2d point = copy.front();
    copy.erase(copy.begin());
    loops.push_back(point);

    while (!copy.empty()) {
        auto closest = copy.begin();
        for (auto it = copy.begin(); it != copy.end(); it++) {
            if (Distance(*closest, point) > Distance(*it, point)) {
                closest = it;
            }
        }
        loops.push_back(*closest);
        point = *closest;
        copy.erase(closest);
    }*/

    body.upload();
}

void Slider::onDraw(Renderer &renderer)
{
    /*============================================================================================================*/
    // Prepare some constant variables.
    const auto objectTransform = session->getObjectTransform();
    float alpha = getAlpha();
    // NotOSUObjectDrawInfo info{ getAlpha(), objectTransform };

    /*============================================================================================================*/
    // Draw the body mesh behind everything else.

    /*color tint = WHITE;
    //bodyTexture.apply();
    renderer.drawMesh(
            body, *bodyShader,
            {{"tint", &tint}},
            {{0, bodyTexture.getTexture().get()}},
            objectTransform
    );
    //bodyTexture.reset();
    */

    /*============================================================================================================*/
    // Draw the start decorations.
    auto startBumperPosition = interpolatedPath.front().position;

    NotOSUObjectDrawInfo headInfo = {
        {{SOF.radius, SOF.radius}, startBumperPosition},
        alpha,
        (Mat3f) Transform2D{.rotate = startBumperAngle,
            .rotationCenter = startBumperPosition} *
            objectTransform};
    renderer.draw(head, headInfo);

    if (repeatsLeft >= 2)
        renderer.draw(headRepeat, headInfo);

    /*============================================================================================================*/
    // Draw the end decorations.
    auto endBumperPosition = interpolatedPath.back().position;

    NotOSUObjectDrawInfo tailInfo = {
        {{SOF.radius, SOF.radius}, endBumperPosition},
        alpha,
        (Mat3f) Transform2D{.rotate = endBumperAngle,
            .rotationCenter = endBumperPosition} *
            objectTransform};

    renderer.draw(tail, tailInfo);
    if (repeatsLeft >= 3)
        renderer.draw(tailRepeat, tailInfo);

    /*============================================================================================================*/
    // Draw the ball moving across the slider.
    auto ballDirection = findNormal(curvePosition);
    auto ballAngle = std::atan2(ballDirection.y, ballDirection.x);

    NotOSUObjectDrawInfo ballInfo = {
        SOF, alpha,
        (Mat3f) Transform2D{.rotate = ballAngle, .rotationCenter = SOF.position} *
            objectTransform};

    renderer.draw(ball, ballInfo);

    NotOSUObjectDrawInfo hitPointInfo = {SOF, alpha, objectTransform};
    for (const auto &node: interpolatedPath) {
        /*========================================================================================================*/
        // Draw the interpolated path. (debug only)
        renderer.drawCross(node.position,
                           float(session->getCircleSize() * 0.2),
                           {.fillColor = ORANGE}, objectTransform);

        /*========================================================================================================*/
        // Draw the bonus point.
        hitPointInfo.destination.position = node.position;
        renderer.draw(hitPoint, hitPointInfo);
    }

    /*bodyTexture.apply();
    renderer.drawMesh(
            body, *bodyShader,
            {{"tint", &tint}},
            {{0, bodyTexture.getTexture().get()}},
            objectTransform
    );
    bodyTexture.reset();*/

    color maskColor = TEAL;
    maskColor.a = 0.01;
    for (auto &circle: circleMask) {
        renderer.drawCircle(circle, {.fillColor = maskColor}, objectTransform);
    }

    int stop = int(curvePosition * (double) loops.size());

    for (int i = 0; i < stop; i++) {
        renderer.drawCross(loops[i],
                           float(session->getCircleSize() * 0.2),
                           {.fillColor = GREEN}, objectTransform);
    }

    for (auto &point: meshSpine) {
        renderer.drawCross(point, float(session->getCircleSize() * 0.1),
                           {.fillColor = GREEN}, objectTransform);
    }

    for (auto &point: meshOutline) {
        renderer.drawCross(point, float(session->getCircleSize() * 0.1),
                           {.fillColor = BLUE}, objectTransform);
    }
}

void Slider::onReset()
{
    progression = 0.0;
    SOF.position = getStartPosition();
}

fvec2d Slider::findDirection(double t)
{
    // Clamp the parameter to <0; t-ε>.
    // This is done so that if t=1, calling curve.get(t+ε) would yield the same
    // result as calling curve.get(t). because the value of t is clamped in the
    // calculation.
    t = Clamp(t, 0.0, 1.0 - epsilon);

    // We find the direction vector by pointing a vector from the requested
    // parametric position t and another position equal to t + ε, with ε being a
    // small positive offset. We normalize this vector and return.
    auto targetPosition = curve.get<CurveType::Straight>(t);
    auto offsetPosition = curve.get<CurveType::Straight>(t + epsilon);
    return Normalize(offsetPosition - targetPosition);
}

fvec2d Slider::findNormal(double t)
{ return Normal(findDirection(t)); }

fvec2d Slider::getStartPosition() const
{
    return interpolatedPath.front().position;
}

fvec2d Slider::getEndPosition() const
{
    if (objectTemplate->repeats % 2 == 1) {
        return interpolatedPath.back().position;
    }
    else {
        return interpolatedPath.front().position;
    }
}
}