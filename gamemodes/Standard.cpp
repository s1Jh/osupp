#include "Standard.hpp"

#include "Math.hpp"
#include "Util.hpp"

namespace GAME_TITLE {

    void Standard::onUpdate(double delta) {
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
                        // Therefore, we will set the object after this as the next "last" object.
                        // The onUpdate loop will then start with that object, ignoring all objects before,
                        // who we've assumed to already be done.
                        auto nextIt = std::next(it);
                        if (last != nextIt)
                        {
                            last = nextIt;
                        }
                        continue;
                    } else {
                        // The object is ahead of our field of vision, since objects are sorted
                        // by order of appearance, all objects ahead of this one will also be invisible.
                        // We therefore stop checking and return.
                        return;
                    }
                case HitObjectState::Ready: {
                    // The object can now be interacted with, check for user inputs and then onUpdate the object accordingly.
                    auto SOF = obj->getSOF();
                    auto cursor = getCursorPosition();

                    if (Distance(SOF.position, cursor) <= SOF.radius) {
                        if (keyboard[Key::X].pressing || keyboard[Key::Z].pressing) {
                            // We are within the object's hit window and are pressing down on the right keys, start.
                            obj->begin();
                        }
                    }

                    break;
                }
                case HitObjectState::Active: {
                    // object has been activated and is currently being held, check if it's been released
                    auto SOF = obj->getSOF();
                    auto cursor = getCursorPosition();

                    if (Distance(SOF.position, cursor) > SOF.radius ||
                        keyboard[Key::X].releasing || keyboard[Key::Z].releasing) {
                        obj->raise();
                    }
                    break;
                }
                case HitObjectState::Inactive: {
                    // object has been activated but is not being held, check if it's being held again
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
    }

    void Standard::onDraw(NotOSU::Renderer &renderer) {
        // the "last" object should be the next visible object in line
        // we therefore onDraw every object until we hit one that is invisible

        renderer.drawRect(getPlayField(), {.fillColor = RED_VIOLET});

        for (auto it = last;; it++) {
            if (it == activeObjects.end())
                break;

            const auto &obj = *it;
            if (obj->getState() == HitObjectState::Invisible) {
                break;
            }

            obj->draw(renderer);
        }
    }

}