#include "Note.hpp"

#include <utility>

NS_BEGIN

    void Note::onBegin() {
        // notes are completed on the first click
        // when this gets called, the object is complete
        transferToPickup(session->getCurrentTime());
        wasHit = true;
    }

    HitResult Note::onFinish() {
        if (!wasHit) {
            // object wasn't hit, return miss
            return HitResult::Missed;
        }
        // negative if hit before the start time
        double hitDelta = getTimeFinished() - getStartTime();
        // should be in the interval < -hitWindow; +hitWindow >
        int rank = 3 - Abs(int(3 * hitDelta / session->getMap()->getHitWindow()));
        rank = Clamp(rank, 1, 3);
        return (HitResult) rank;
    }

    Note::Note(std::shared_ptr<ObjectTemplateNote> t, BaseGameMode *g)
            : HitObject(std::move(t), g) {
        SOF = {session->getMap()->getCircleSize(), objectTemplate->position};
    }

    void Note::onDraw(Renderer &renderer) {
        const auto& objectTransform = session->getObjectTransform();
        auto circleSize = SOF.radius;
        auto col = WHITE;
        col.a = getAlpha();
        renderer.drawRect(
                { { circleSize, circleSize }, SOF.position },
                {
                        .texture = &StandardResources::NoteBase,
                        .fillColor = col
                }, objectTransform);
        renderer.drawRect(
                { { circleSize, circleSize }, SOF.position },
                {
                        .texture = &StandardResources::NoteOverlay,
                        .fillColor = col
                }, objectTransform);
    }
}
