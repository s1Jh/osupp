#include "BaseGameMode.hpp"

#include "Util.hpp"
#include "HitObjects.hpp"
#include "Math.hpp"

#define MAKE_CASE(TYPE) \
case HitObjectType::TYPE: {auto object = std::make_shared<TYPE>(std::static_pointer_cast<ObjectTemplate##TYPE>(objectTemplate), this);activeObjects.push_front(object);break;}

NS_BEGIN


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

    last = activeObjects.begin();

    /*last = std::find_if(
        activeObjects.begin(), activeObjects.end(),
        [&](const std::shared_ptr<BaseHitObject>& object) -> bool {
            return newTime <= object->getStartTime();
        }
    );*/
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

    for (auto& object : activeObjects) {
        object->reset();
    }

    last = activeObjects.begin();
}

const frect &BaseGameMode::getPlayField() const {
    return playField;
}

void BaseGameMode::setPlayField(const frect &field) {
    playField = field;
    transform =
            MakeScaleMatrix( fvec2d(field.size) ) *
            MakeTranslationMatrix(field.position);
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

const Mat3f &BaseGameMode::getObjectTransform() const {
    return transform;
}

NS_END