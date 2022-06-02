#include "BaseObjectTemplate.hpp"

#include "define.hpp"

namespace GAME_TITLE {

    HitObjectType BaseObjectTemplate::getType() const {
        return this->type;
    }

    BaseObjectTemplate::BaseObjectTemplate(HitObjectType type) : type(type) {}
}