#include "Note.h"

#include <utility>

namespace GAME_TITLE {

    void Note::onUpdate(double delta) {

    }

    void Note::onBegin() {

    }

    HitResult Note::onFinish() {
        return HitResult::Hit50;
    }

    void Note::onRaise() {

    }

    Note::Note(std::shared_ptr<ObjectTemplateNote> t, BaseGameMode *g)
            : HitObject(std::move(t), g) {
        SOF = {session->getMap()->getCircleSize(), objectTemplate->position};
    }

    void Note::onPress() {

    }

    void Note::onDraw(Renderer &renderer) {
    }
}
