#include "Standard.hpp"

#include "Math.hpp"
#include "Util.hpp"

NS_BEGIN

void Standard::onUpdate(double delta)
{
    // objects are sorted by order of appearance
    for (auto it = last;; it++) {
        if (it == activeObjects.end())
            return;

        auto &obj = *it;

        obj->update(delta);
        switch (obj->getState()) {
            case HitObjectState::Invisible:
                if (obj->isFinished()) {
                    // This object has already been passed and is invisible.
                    // We can assume it will not need to be checked anymore.
                    // Therefore, we will set the object after this as the next "last"
                    // object. The onUpdate loop will then start with that object,
                    // ignoring all objects before, who we've assumed to already be done.
                    auto nextIt = std::next(it);
                    if (last != nextIt) {
                        if (nextIt == activeObjects.end()) {
                            // first check if nextIt is the end, if it is so
                            // just set it without checking for time as that would
                            // cause access violation
                            last = activeObjects.end();
                        }
                        else if ((*last)->getEndTime() < (*nextIt)->getEndTime()) {
                            last = nextIt;
                        }
                    }
                    continue;
                }
                else {
                    // The object is ahead of our field of vision, since objects are sorted
                    // by order of appearance, all objects ahead of this one will also be
                    // invisible. We therefore stop checking and return.
                    return;
                }
            case HitObjectState::Ready: {
                // The object can now be interacted with, check for user inputs and then
                // onLogicUpdate the object accordingly.
                auto SOF = obj->getSOF();
                auto cursor = getCursorPosition();

                if (Distance(SOF.position, cursor) <= SOF.radius) {
                    if (keyboard[Key::X].pressing || keyboard[Key::Z].pressing) {
                        // We are within the object's hit window and are pressing down on the
                        // right keys, start.
                        obj->begin();
                    }
                }

                break;
            }
            case HitObjectState::Active: {
                // object has been activated and is currently being held, check if it's
                // been released
                auto SOF = obj->getSOF();
                auto cursor = getCursorPosition();

                if (Distance(SOF.position, cursor) > SOF.radius ||
                    keyboard[Key::X].releasing || keyboard[Key::Z].releasing) {
                    obj->raise();
                }
                break;
            }
            case HitObjectState::Inactive: {
                // object has been activated but is not being held, check if it's being
                // held again
                auto SOF = obj->getSOF();
                auto cursor = getCursorPosition();

                if (Distance(SOF.position, cursor) <= SOF.radius) {
                    if (keyboard[Key::X].pressed || keyboard[Key::Z].pressed) {
                        obj->press();
                    }
                }
                break;
            }
                // The object cannot be interacted with yet
            case HitObjectState::Pickup: {
                auto score = obj->finish();
                log::info("SCORE: ", (int) score);
            }
            case HitObjectState::Fading:
            case HitObjectState::Approaching:
            default:
                break;
        }
    }

    // update the visuals
    playField.update(delta);
}

void Standard::onDraw(NotOSU::Renderer &renderer)
{
    const auto &transform = getObjectTransform();
    // the "last" object should be the next visible object in line
    // we therefore onDraw every object until we hit one that is invisible
    renderer.draw(playField, NotOSUObjectDrawInfo{.destination = getPlayField(), .transform = transform});

    if (last == activeObjects.end())
        return;

    // we need to reverse the order objects are drawn in, meaning that objects which will come next will be drawn last
    // this is done so that the most relevant objects are always on top

    // first find the last object that would be drawn
    auto end = std::find_if(last, activeObjects.end(), [](const std::shared_ptr<BaseHitObject> &obj)
    {
        return (obj->getState() == HitObjectState::Invisible) && !obj->isFinished();
    });

    // render everything from the object before last to the current object
    for (auto it = std::prev(end); it != std::prev(last); it--) {
        auto ptr = *it;
        ptr->draw(renderer);

        renderer.drawCross(ptr->getEndPosition(), 0.1f, {.fillColor = RED, .zIndex = 1.0f}, transform);
        renderer.drawCross(ptr->getStartPosition(), 0.1f, {.fillColor = GREEN, .zIndex = 1.0f}, transform);
        if (it != last && it != std::prev(end)) {
            fline connector{
                (*it)->getEndPosition(),
                (*std::next(it))->getStartPosition()
            };
            color tint = ORANGE;
            tint.a = 0.5f;
            renderer.drawSegment(connector, {.fillColor = tint}, transform);
        }
    }

}

Standard::Standard(Game &instance)
    : BaseGameMode(instance)
{
    playField = getActiveSkin()->createObjectSprite(PLAY_FIELD_SPRITE);
}

NS_END