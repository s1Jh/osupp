#include "BaseGameMode.hpp"

#include "HitObjects.hpp"
#include "Math.hpp"
#include "Util.hpp"

#define MAKE_CASE(TYPE)                                                         \
  case HitObjectType::TYPE: {                                                   \
    auto object = std::make_shared<TYPE>(                                       \
        std::static_pointer_cast<ObjectTemplate##TYPE>(objectTemplate), *this); \
    activeObjects.push_front(object);                                           \
    break;                                                                      \
  }

NS_BEGIN

BaseGameMode::BaseGameMode()
    : info(nullptr), playField(UNIT_RECT<float>), currentTime(0.0)
{
    reset();
}

double BaseGameMode::getCurrentTime() const
{ return currentTime; }

void BaseGameMode::setCurrentTime(double newTime)
{
    currentTime = newTime;

    for (auto &obj: activeObjects) {
        obj->reset();
        obj->update(0.0);
    }

    last = activeObjects.begin();
}

void BaseGameMode::update(double delta)
{
    currentTime += delta;
    this->onUpdate(delta);
}

void BaseGameMode::draw(Renderer &gfx)
{ this->onDraw(gfx); }

void BaseGameMode::setMap(MapInfoP map)
{
    info = map;
    activeObjects.clear();

    int objectSeed = 0;
    int comboSeed = 0;
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

            if (bool(objectTemplate->parameters & HitObjectParams::ComboEnd))
                comboSeed++;
            objectSeed++;
        }
    }

    reset();
}

void BaseGameMode::reset()
{
    log::debug("Resetting");
    if (info)
        currentTime = info->getStartOffset();
    else
        currentTime = 0.0;

    for (auto &object: activeObjects) {
        object->reset();
    }

    last = activeObjects.begin();

    this->onReset();
}

const frect &BaseGameMode::getPlayField() const
{ return playField; }

void BaseGameMode::setPlayField(const frect &field)
{
    playField = field;
    auto smaller = Min(field.size.w, field.size.h);
    transform = MakeScaleMatrix(fvec2d(smaller, smaller)) *
        MakeTranslationMatrix(field.position);
}

fvec2d BaseGameMode::getCursorPosition() const
{
    auto pos = Mouse::position();
    pos -= playField.position;
    auto smaller = Min(playField.size.w, playField.size.h);
    pos /= fvec2d{smaller, smaller};
    return pos;
}

MapInfoP BaseGameMode::getMap() const
{ return info; }

const Mat3f &BaseGameMode::getObjectTransform() const
{ return transform; }

float BaseGameMode::getCircleSize()
{
    return info->getCircleSize(); /* * multiplier */
}

float BaseGameMode::getApproachTime()
{
    return info->getApproachTime(); /* * multiplier */
}

float BaseGameMode::getFadeTime()
{
    return info->getFadeTime(); /* * multiplier */
}

float BaseGameMode::getHitWindow()
{
    return info->getHitWindow(); /* * multiplier */
}

void BaseGameMode::onReset()
{}

NS_END