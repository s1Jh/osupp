#pragma once

#include "HitObject.tpp"
#include "NotOSUObjectSprite.hpp"
#include "NoteTemplate.hpp"
#include "define.hpp"

NS_BEGIN

class Note: public HitObject<ObjectTemplateNote>
{
public:
    explicit Note(std::shared_ptr<ObjectTemplateNote>, BaseGameMode *);

    fvec2d getStartPosition() const override;

    fvec2d getEndPosition() const override;

    double getStartTime() const override;

    double getEndTime() const override;

protected:
    void onReset() override;

protected:
    void onUpdate(double delta) override;

    void onDraw(Renderer &renderer) override;

    void onBegin() override;

    HitResult onFinish() override;

    bool wasHit = false;

private:
    NotOSUObjectSprite noteBase;
    NotOSUObjectSprite noteOverlay;
    NotOSUObjectSprite noteUnderlay;
};

NS_END