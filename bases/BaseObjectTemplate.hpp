/*******************************************************************************
 * Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

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