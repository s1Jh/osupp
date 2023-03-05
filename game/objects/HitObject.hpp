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

#include "GameManager.hpp"
#include "BaseHitObject.hpp"
#include "BaseObjectTemplate.hpp"
#include "Enum.hpp"
#include "Math.hpp"
#include "Util.hpp"
#include "Vector.hpp"
#include "Context.hpp"

#include <memory>
#include <string>

namespace PROJECT_NAMESPACE {

template<typename TemplateT, typename FlagT, FlagT FlagsIn>
requires IsTemplateV<TemplateT>
class HitObject: public BaseHitObject
{
public:
    explicit HitObject(std::shared_ptr<TemplateT> templateIn) :
		objectTemplate(std::move(templateIn))
	{}

    [[nodiscard]] double getStartTime() const final
	{
		return objectTemplate->startTime;
	}


	[[nodiscard]] double getEndTime() const final
	{
		return objectTemplate->endTime;
	}

protected:
    std::shared_ptr<TemplateT> objectTemplate;
	const static FlagT Flags = FlagsIn;
};

}