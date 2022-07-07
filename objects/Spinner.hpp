#pragma once

#include "HitObject.tpp"
#include "SpinnerTemplate.hpp"
#include "define.hpp"

NS_BEGIN
class Spinner: public HitObject<ObjectTemplateSpinner>
{
public:
    explicit Spinner(std::shared_ptr<ObjectTemplateSpinner>, BaseGameMode *);

protected:
    void onDraw(Renderer &renderer) override;

    void onLogicUpdate(double delta) override;

    void onUpdate(double delta) override;

    void onBegin() override;

    void onPress() override;

    void onRaise() override;

    HitResult onFinish() override;

    [[nodiscard]] bool needsApproachCircle() const override;

private:
    NotOSUObjectSprite spinner;
    NotOSUObjectSprite spinnerCenter;
    NotOSUObjectSprite spinnerMeter;
    float RPM;
    float rotation;
    fvec2d lastVector;
};
}