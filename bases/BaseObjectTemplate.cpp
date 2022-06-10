#include "BaseObjectTemplate.hpp"

#include "define.hpp"

NS_BEGIN

HitObjectType BaseObjectTemplate::getType() const {
    return this->type;
}

BaseObjectTemplate::BaseObjectTemplate(HitObjectType type) : type(type) {}

NS_END