#include "BaseGameMode.hpp"

#include "HitObjects.hpp"
#include "Math.hpp"
#include "Util.hpp"

#define MAKE_CASE(TYPE)                                                        \
  case HitObjectType::TYPE: {                                                  \
    auto object = std::make_shared<TYPE>(                                      \
        std::static_pointer_cast<ObjectTemplate##TYPE>(objectTemplate), this); \
    activeObjects.push_front(object);                                          \
    break;                                                                     \
  }

NS_BEGIN

BaseGameMode::BaseGameMode(Game &instance)
    : info(nullptr), instance(instance), playField({1.0f, 1.0f}, {0.0f, 0.0f}),
      currentTime(0.0)
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

    /*last = std::find_if(
        activeObjects.begin(), activeObjects.end(),
        [&](const std::shared_ptr<BaseHitObject>& object) -> bool {
            return newTime <= object->getStartTime();
        }
    );*/
}

void BaseGameMode::update(double delta)
{
    currentTime += delta;
    this->onUpdate(delta);
}

void BaseGameMode::draw(Renderer &gfx)
{ this->onDraw(gfx); }

void BaseGameMode::setMap(MapInfo *newMap)
{
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

void BaseGameMode::reset()
{
    if (info)
        currentTime = info->getStartOffset();
    else
        currentTime = 0.0;

    for (auto &object: activeObjects) {
        object->reset();
    }

    last = activeObjects.begin();
}

const frect &BaseGameMode::getPlayField() const
{ return playField; }

void BaseGameMode::setPlayField(const frect &field)
{
    playField = field;
    transform = MakeScaleMatrix(fvec2d(field.size)) *
        MakeTranslationMatrix(field.position);
}

fvec2d BaseGameMode::getCursorPosition() const
{
    auto pos = Mouse::position();
    pos -= playField.position;
    pos /= (fvec2d) playField.size;
    return pos;
}

MapInfo *BaseGameMode::getMap() const
{ return info; }

const Mat3f &BaseGameMode::getObjectTransform() const
{ return transform; }

Game &BaseGameMode::getGame()
{ return instance; }

const df2 &
BaseGameMode::getHitObjectVisuals(const std::string &objectType) const
{
    return settings["objects"][objectType];
}

Resources &BaseGameMode::getResources()
{ return instance.getResourcePool(); }

float BaseGameMode::getCircleSize()
{
    return info->getCircleSize(); /* * multiplier */
}

SkinP BaseGameMode::getActiveSkin()
{ return instance.getActiveSkin(); }

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

NS_END