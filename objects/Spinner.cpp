//
// Created by sijh on 30.05.22.
//

#include "Spinner.h"

#include <utility>

namespace GAME_TITLE {
    void Spinner::onUpdate(double delta) {

    }

    void Spinner::onBegin() {

    }

    HitResult Spinner::onFinish() {
        return HitResult::Hit50;
    }

    void Spinner::onRaise() {

    }

    Spinner::Spinner(std::shared_ptr<ObjectTemplateSpinner> t, BaseGameMode *g)
            : HitObject(std::move(t), g) {}

    void Spinner::onPress() {

    }

    void Spinner::onDraw(Renderer &renderer) {

    }
}
