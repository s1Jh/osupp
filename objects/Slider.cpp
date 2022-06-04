#include "Slider.hpp"
#include "SliderUtils.hpp"

#include <utility>

namespace GAME_TITLE {

    void Slider::onUpdate(double delta) {
        if (session->getCurrentTime() >= getStartTime() && !started) {
            // if we're still in ready past the hit window, start automagically
            started = true;
        }

        if (started) {
            double timeLength = getEndTime() - getStartTime();
            double timeRunning = session->getCurrentTime() - getStartTime();
            progression = timeRunning / timeLength;
            SOF.position = util.findPoint(progression);

            if (progression >= targetProgression) {
                transferToPickup(session->getCurrentTime());
            }
        }
    }

    void Slider::onBegin() {
        started = true;
    }

    HitResult Slider::onFinish() {
        return HitResult::Hit50;
    }

    void Slider::onRaise() {

    }

    void Slider::onPress() {

    }

    Slider::Slider(std::shared_ptr<ObjectTemplateSlider> t, BaseGameMode *g)
            : HitObject(std::move(t), g), util(*this),
            length(0.0), progression(0.0), targetProgression(1.0), started(false) {
        SOF = {session->getMap()->getCircleSize(), {objectTemplate->path.front().position}};

        util.interpolatePath();

        velocity = 1.0 / (this->getEndTime() - this->getStartTime());

        body.setAttributeDescriptors({
                                             AttributeType::Vec2,    // position
                                             AttributeType::Vec2     // UV
                                     });

        for (auto it = objectTemplate->path.begin(); it != objectTemplate->path.end(); it++) {
            auto& obj = *it;
            auto start = obj.position;

            float segmentLength = 0;
            GLLine connector;

            if (it != objectTemplate->path.begin()) {
                // not at the start
            }
            if (it != std::prev(objectTemplate->path.end())) {
                // not at the end
                auto nextIt = std::next(it);
                auto& nextObj = *nextIt;
                auto end = nextObj.position;
                segmentLength = Distance(start, end);
                connector = GLLine(start, end);
                length += segmentLength;

                // mesh generation
                fvec2d normal = {
                        (start.y - end.y),
                        -(start.x - end.x),
                };

                normal = Normalize(normal);
                normal *= session->getMap()->getCircleSize();

                auto v1 = start + normal;
                auto v2 = end + normal;
                auto v3 = start - normal;
                auto v4 = end - normal;

                auto offset = body.insertVertices({
                                                          {start.x, start.y, 0.0f, 0.0f},
                                                          {v1.x, v1.y, 1.0f, 0.0f},
                                                          {v2.x, v2.y, 1.0f, 1.0f},
                                                          {end.x, end.y, 0.0f, 1.0f},
                                                          {v3.x, v3.y, 1.0f, 0.0f},
                                                          {v4.x, v4.y, 1.0f, 1.0f},
                                                  });

                body.insertIndices({
                                           0, 1, 3,
                                           3, 2, 1,
                                           4, 0, 3,
                                           3, 4, 5
                                   }, offset);
            }

            path.push_back(ActiveSliderNode{&obj, segmentLength, connector});
        }

        body.upload();
    }

    void Slider::onDraw(Renderer &renderer) {
        const auto objectTransform = session->getObjectTransform();
        color tint = WHITE;
        tint.a = getAlpha();

        renderer.drawMesh(
                body, StandardResources::SliderShader,
                {{"color", tint}},
                {{0, &StandardResources::SliderBody}},
                objectTransform
        );

        auto direction = util.findNormal(progression);
        auto angle = std::atan2(direction.y, direction.x);

        renderer.drawRect(
                {{SOF.radius, SOF.radius}, SOF.position},
                {.texture = &StandardResources::SliderBall, .fillColor = tint},
                (Mat3f) Transform2D{.rotate = angle, .rotationCenter = SOF.position} * objectTransform
        );

        for (auto &node: path) {
            renderer.drawSegment(
                    node.connectingLine,
                    {.fillColor = GREEN, .outlineWidth = 5},
                    objectTransform
            );
            renderer.drawCross(node.info->position, session->getMap()->getCircleSize() / 2,
                               {.fillColor = ORANGE}, objectTransform);

            if (node.info->bonus) {
                renderer.drawRect(
                        {{SOF.radius, SOF.radius}, node.info->position},
                        {
                            .texture = &StandardResources::SliderHitPoint,
                            .fillColor = tint
                        }, objectTransform
                );
            }
        }
    }
}