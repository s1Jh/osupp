#include "BaseGameMode.hpp"

#include "Util.hpp"
#include "HitObjects.h"
#include "Math.hpp"

#define MAKE_CASE(TYPE) \
    case HitObjectType::TYPE: {auto object = std::make_shared<TYPE>(std::static_pointer_cast<ObjectTemplate##TYPE>(objectTemplate), this);activeObjects.push_front(object);break;}

namespace GAME_TITLE {


    BaseGameMode::BaseGameMode() : info(nullptr), currentTime(0.0), playField({1.0f, 1.0f}, {0.0f, 0.0f}) {
        reset();
    }

    double BaseGameMode::getCurrentTime() const {
        return currentTime;
    }

    void BaseGameMode::setCurrentTime(double newTime) {
        currentTime = newTime;

        for (auto& obj : activeObjects) {
            obj->reset();
            obj->update(0.0);
        }

        last = std::find_if(
            activeObjects.begin(), activeObjects.end(),
            [&](const std::shared_ptr<BaseHitObject>& object) -> bool {
                return newTime <= object->getStartTime();
            }
        );
    }

    void BaseGameMode::update(double delta) {
        currentTime += delta;
        this->onUpdate(delta);
    }

    void BaseGameMode::draw(Renderer &gfx) {
        this->onDraw(gfx);
    }

    void BaseGameMode::setMap(MapInfo *newMap) {
        info = newMap;
        activeObjects.clear();

        if (info) {
            auto templates = info->getObjectTemplates();
            for (const auto &objectTemplate: templates) {
                switch (objectTemplate->getType()) {
                    MAKE_CASE(Spinner)
                    MAKE_CASE(Slider)
                    MAKE_CASE(Note)
                    default:
                        log::warning("Corrupted map template: ", objectTemplate);
                        break;
                }
            }
        }

        reset();
    }

    void BaseGameMode::reset() {
        if (info)
            currentTime = info->getStartOffset();
        else
            currentTime = 0.0;
        last = activeObjects.begin();
    }

    const frect &BaseGameMode::getPlayField() const {
        return playField;
    }

    void BaseGameMode::setPlayField(const frect &field) {
        playField = field;
        transform =
                MakeTranslationMatrix(field.position) *
                MakeScaleMatrix(fvec2d(1.f / field.size));
    }

    fvec2d BaseGameMode::getCursorPosition() const {
        auto pos = Mouse::position();
        pos -= playField.position;
        pos /= (fvec2d) playField.size;
        return pos;
    }

    MapInfo *BaseGameMode::getMap() const {
        return info;
    }
}