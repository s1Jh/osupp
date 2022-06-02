#pragma once

#include "define.hpp"
#include "HitObject.tpp"
#include "NoteTemplate.hpp"

namespace GAME_TITLE {

    class Note : public HitObject<ObjectTemplateNote> {
    public:
        explicit Note(std::shared_ptr<ObjectTemplateNote>, BaseGameMode *);


    protected:
        void onDraw(Renderer &renderer) override;
        void onUpdate(double delta) override;
        void onBegin() override;
        void onPress() override;
        HitResult onFinish() override;
        void onRaise() override;
    };

}