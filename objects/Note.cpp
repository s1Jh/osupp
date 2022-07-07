#include "Note.hpp"

#include <utility>

NS_BEGIN

void Note::onBegin()
{
    // notes are completed on the first click
    // when this gets called, the object is complete
    transferToPickup();
    wasHit = true;
}

HitResult Note::onFinish()
{
    if (!wasHit) {
        // object wasn't hit, return miss
        return HitResult::Missed;
    }
    // negative if hit before the start time
    double hitDelta = getTimeFinished() - getStartTime();
    // should be in the interval < -hitWindow; +hitWindow >
    int rank = 3 - Abs(int(3 * hitDelta / session->getHitWindow()));
    rank = Clamp(rank, 1, 3);
    return (HitResult) rank;
}

Note::Note(std::shared_ptr<ObjectTemplateNote> t, BaseGameMode *g)
    : HitObject(std::move(t), g)
{
    SOF = {session->getCircleSize(), objectTemplate->position};

    auto skin = session->getActiveSkin();

    noteBase = skin->createObjectSprite(NOTE_BASE_SPRITE);
    noteOverlay = skin->createObjectSprite(NOTE_OVERLAY_SPRITE);
    noteUnderlay = skin->createObjectSprite(NOTE_UNDERLAY_SPRITE);
}

void Note::onDraw(Renderer &renderer)
{
    const auto &objectTransform = session->getObjectTransform();

    NotOSUObjectDrawInfo info{SOF, getAlpha(), objectTransform};

    renderer.draw(noteUnderlay, info);
    renderer.draw(noteBase, info);
    renderer.draw(noteOverlay, info);
}

void Note::onUpdate(double delta)
{
    noteBase.update(delta);
    noteOverlay.update(delta);
    noteUnderlay.update(delta);
}

fvec2d Note::getStartPosition() const
{ return objectTemplate->position; }

fvec2d Note::getEndPosition() const
{ return objectTemplate->position; }

double Note::getStartTime() const
{
    return (objectTemplate->startTime + objectTemplate->endTime) / 2.0f;
}

double Note::getEndTime() const
{
    return (objectTemplate->startTime + objectTemplate->endTime) / 2.0f;
}

void Note::onReset()
{ wasHit = false; }
}
