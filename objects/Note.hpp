#pragma once

#include "HitObject.tpp"
#include "NotOSUObjectSprite.hpp"
#include "NoteTemplate.hpp"
#include "define.hpp"

NS_BEGIN

class Note: public HitObject<ObjectTemplateNote>
{
public:
    explicit Note(std::shared_ptr<ObjectTemplateNote>, BaseGameMode &);

    [[nodiscard]] fvec2d getStartPosition() const override;

    [[nodiscard]] fvec2d getEndPosition() const override;

    [[nodiscard]] double getStartTime() const override;

    [[nodiscard]] double getEndTime() const override;

protected:
    void onReset() override;

    void onUpdate(double delta) override;

    void onDraw(Renderer &renderer) override;

    void onBegin() override;

    HitResult onFinish() override;

private:
    bool wasHit = false;
    NotOSUObjectSprite noteBase;
    NotOSUObjectSprite noteOverlay;
    NotOSUObjectSprite noteUnderlay;
};

NS_END