//
// Created by sijh on 30.05.22.
//

#include "Spinner.hpp"

#include <utility>

namespace GAME_TITLE {
    void Spinner::onUpdate(double delta) {
        //

        if (isActive()) {
            auto cursor = Normalize(session->getCursorPosition());
            auto alpha = std::atan2(cursor.y, cursor.x) - std::atan2(lastVector.y, lastVector.x);
            rotation += alpha;

            lastVector = cursor;
        }
    }

    void Spinner::onBegin() {
        lastVector = Normalize(session->getCursorPosition());
    }

    HitResult Spinner::onFinish() {
        return HitResult::Hit50;
    }

    void Spinner::onRaise() {

    }

    Spinner::Spinner(std::shared_ptr<ObjectTemplateSpinner> t, BaseGameMode *g)
            : HitObject(std::move(t), g), rotation(0.0f), RPM(0.0) {
        SOF = { 2.0f, {0.0f, 0.0f}};
    }

    void Spinner::onPress() {
        lastVector = Normalize(session->getCursorPosition());
    }

    void Spinner::onDraw(Renderer &renderer) {
        auto tint = WHITE;
        tint.a = getAlpha();
        const auto& objectTransform = session->getObjectTransform();
        auto centerTransform = (Mat3f) Transform2D {
                .rotate = rotation
        } * objectTransform;

        frect all = {{0.8f, 0.8f}, {0.f, 0.f}};
        renderer.drawRect(
            all, {.texture = &StandardResources::Spinner, .fillColor = tint},
            objectTransform
        );
        renderer.drawRect(
            all, {.texture = &StandardResources::SpinnerCenter, .fillColor = tint},
            centerTransform
        );
    }
    bool Spinner::needsApproachCircle() const {
        return false;
    }
}
