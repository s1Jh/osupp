#include "Spinner.hpp"

#include "Math.hpp"

NS_BEGIN
void Spinner::onLogicUpdate(double delta)
{
    if (isActive()) {
        auto cursor = Normalize(session.getCursorPosition());
        auto alpha = (float) Cross(lastVector, cursor);

        RPM = float(double(Abs(alpha) / 2.0_pi) / delta * 60.0);
        rotation += (float) alpha;

        if (Abs(rotation) > 2.0_pi) {
            rotation = std::fmod(rotation, (float) 2.0_pi);
            rotationsCompleted++;
        }

        lastVector = cursor;
    }
}

void Spinner::onUpdate(double delta)
{
    // update texture animations
    spinner.update(delta);
    spinnerCenter.update(delta);
    spinnerMeter.update(delta);
}

void Spinner::onBegin()
{
    lastVector = Normalize(session.getCursorPosition());
}

HitResult Spinner::onFinish()
{
    if (rotationsCompleted == 0)
        return HitResult::Missed;

    return HitResult::Hit50;
}

Spinner::Spinner(std::shared_ptr<ObjectTemplateSpinner> t, BaseGameMode &g)
    : HitObject(std::move(t), g), RPM(0.0), rotationAccum(0.0f), rotationsCompleted(0), rotation(0.0f)
{
    auto skin = session.getActiveSkin();

    spinner = skin->createObjectSprite(SPINNER_SPRITE);
    spinnerCenter = skin->createObjectSprite(SPINNER_CENTER_SPRITE);
    spinnerMeter = skin->createObjectSprite(SPINNER_METER_SPRITE);

    SOF = {2.0f, {0.0f, 0.0f}};
}

void Spinner::onPress()
{
    lastVector = Normalize(session.getCursorPosition());
}

void Spinner::onDraw(Renderer &renderer)
{
    auto alpha = getAlpha();
    const auto &objectTransform = session.getObjectTransform();

    frect all = {{0.8f, 0.8f}, {0.f, 0.f}};

    NotOSUObjectDrawInfo spinnerInfo{all, alpha, objectTransform};
    NotOSUObjectDrawInfo spinnerCenterInfo{
        all, alpha, (Mat3f) Transform2D{.rotate = rotation} * objectTransform};

    renderer.draw(spinner, spinnerInfo);
    renderer.draw(spinnerCenter, spinnerCenterInfo);
}

bool Spinner::needsApproachCircle() const
{ return false; }

void Spinner::onReset()
{
    rotation = 0.0f;
    rotationAccum = 0.0f;
    rotationsCompleted = 0;
}

}
