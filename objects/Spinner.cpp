//
// Created by sijh on 30.05.22.
//

#include "Spinner.hpp"

#include <utility>

NS_BEGIN
void Spinner::onLogicUpdate(double delta)
{
    if (isActive()) {
        auto cursor = Normalize(session->getCursorPosition());
        auto alpha =
            std::atan2(cursor.y, cursor.x) - std::atan2(lastVector.y, lastVector.x);
        rotation += alpha;

        lastVector = cursor;
    }
}

void Spinner::onUpdate(double delta)
{
    // update texture animations
    // spinner->update(delta);
    // spinnerCenter->update(delta);
    // spinnerMeter->update(delta);
}

void Spinner::onBegin()
{
    lastVector = Normalize(session->getCursorPosition());
}

HitResult Spinner::onFinish()
{ return HitResult::Hit50; }

void Spinner::onRaise()
{}

Spinner::Spinner(std::shared_ptr<ObjectTemplateSpinner> t, BaseGameMode *g)
    : HitObject(std::move(t), g), RPM(0.0), rotation(0.0f)
{

    auto skin = g->getActiveSkin();

    spinner = skin->createObjectSprite(SPINNER_SPRITE);
    spinnerCenter = skin->createObjectSprite(SPINNER_CENTER_SPRITE);
    spinnerMeter = skin->createObjectSprite(SPINNER_METER_SPRITE);

    SOF = {2.0f, {0.0f, 0.0f}};
}

void Spinner::onPress()
{
    lastVector = Normalize(session->getCursorPosition());
}

void Spinner::onDraw(Renderer &renderer)
{
    auto alpha = getAlpha();
    const auto &objectTransform = session->getObjectTransform();

    frect all = {{0.8f, 0.8f}, {0.f, 0.f}};

    NotOSUObjectDrawInfo spinnerInfo{all, alpha, objectTransform};
    NotOSUObjectDrawInfo spinnerCenterInfo{
        all, alpha, (Mat3f) Transform2D{.rotate = rotation} * objectTransform};

    renderer.draw(spinner, spinnerInfo);
    renderer.draw(spinnerCenter, spinnerCenterInfo);
}

bool Spinner::needsApproachCircle() const
{ return false; }
}
