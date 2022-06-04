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
        void onBegin() override;
        HitResult onFinish() override;

        bool wasHit = false;
    };

}