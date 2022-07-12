#pragma once

#include "define.hpp"

#include "Enum.hpp"

#define BEGIN_OBJECT_TEMPLATE(TYPE)                                            \
  struct ObjectTemplate##TYPE : public BaseObjectTemplate {                    \
    inline explicit ObjectTemplate##TYPE()                                     \
        : BaseObjectTemplate(HitObjectType::TYPE) {}

#define END_OBJECT_TEMPLATE()                                                  \
  }                                                                            \
  ;

NS_BEGIN

struct BaseObjectTemplate
{
    explicit BaseObjectTemplate(HitObjectType type);

    [[nodiscard]] HitObjectType getType() const;

    double startTime = 0.0;
    double endTime = 0.0;
    HitObjectParams parameters = OBJECT_NO_PARAMS;

private:
    const HitObjectType type;
};

template<typename T>
struct IsTemplate
{
    static const bool value = std::is_base_of_v<BaseObjectTemplate, T>;
};

template<class T> inline constexpr bool IsTemplateV = IsTemplate<T>::value;

NS_END