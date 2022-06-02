#pragma once

#include "define.hpp"
#include "HitObject.tpp"
#include "SpinnerTemplate.hpp"

namespace GAME_TITLE {
    class Spinner : public HitObject<ObjectTemplateSpinner> {
    public:
        explicit Spinner(std::shared_ptr<ObjectTemplateSpinner>, BaseGameMode *);

    protected:
        void onDraw(Renderer &renderer) override;
        void onUpdate(double delta) override;
        void onBegin() override;
        void onPress() override;
        void onRaise() override;
        HitResult onFinish() override;

        [[nodiscard]] bool needsApproachCircle() const override;

        float RPM;
        float rotation;
        fvec2d lastVector;
    };

}