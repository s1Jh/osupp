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

#include "HitObject.hpp"

NS_BEGIN

enum class OsuHitObjectFlags : uint8_t
{
	NoApproachCircle = 0 << 1,
	DrawApproachCircle = 1 << 1,	// bit 1
	NoFadeoutAnimation = 0 << 2,
	ApplyFadeoutAnimation = 1 << 2,	// bit 2
};

ENABLE_BITMASK_OPERATORS(OsuHitObjectFlags)

constexpr OsuHitObjectFlags DEFAULT_OSU_HIT_OBJECT_FLAGS =
	OsuHitObjectFlags::DrawApproachCircle |
	OsuHitObjectFlags::ApplyFadeoutAnimation;

template <typename TemplateT, OsuHitObjectFlags Flags = DEFAULT_OSU_HIT_OBJECT_FLAGS>
requires IsTemplateV<TemplateT>
class OsuHitObject : public HitObject<TemplateT, OsuHitObjectFlags, Flags>
{
public:
	explicit OsuHitObject(std::shared_ptr<TemplateT> templateIn, const HitObjectArguments& argsIn) :
		HitObject<TemplateT, OsuHitObjectFlags, Flags>(std::move(templateIn)), args(argsIn)
	{
		approachCircle = this->ctx.activeSkin->createObjectSprite(APPROACH_CIRCLE_SPRITE, args);
	}

protected:
	void drawApproachCircle()
	{
		// HACK: Since drawApproachCircle only gets called once during the draw, we can
		//		 also update the sprite animation. This will break if the approach circle
		// 		 for whatever reason gets drawn twice during one frame. The animation will
		//		 be sped up as a result. Also, we lose const-ness on this function.
		approachCircle.update(this->ctx.timing.getDelta());
		const auto &transform = this->getObjectTransform();

		if (this->isApproachCircleDrawn()) {
			const auto scale = 4.0f;  // how big will the circle be at -ar
			const auto offset = 1.0f; // how big the circle will be at 0

			float slope = (scale - offset) / this->ctx.game.getApproachTime();

			float x = this->ctx.game.getCurrentTime() - this->getStartTime();
			float y = x * -slope + offset;

			float acSize = this->ctx.game.getCircleSize() * y;
			acSize = Max(acSize, 0.0);

			this->ctx.gfx.draw(approachCircle, ObjectDrawInfo{
				{{acSize, acSize}, this->SOF.position},
				this->getAlpha(), transform
			});
		}
	}

	[[nodiscard]] Mat3f calculateObjectTransform() const override
	{
		if (
			this->getState() == HitObjectState::Fading &&
			bool(Flags & OsuHitObjectFlags::ApplyFadeoutAnimation)
			)
		{
			// make objects float up after they've been struck
			// use the alpha value, as it's 1.0 when fading
			// starts and 0.0 when it finishes
			float fadingProgress = 1.0f - this->getAlpha();

			float floatAmount = 0.1f;
			float scale = Lerp(1.0f, 0.75f, SmoothStep(fadingProgress));

			return
				MakeTranslationMatrix<float>({0.0f, fadingProgress * floatAmount}) *
				MakeScaleMatrix<float>({scale, scale}, this->SOF.position);
		}
		return MAT3_NO_TRANSFORM<float>;
	}

private:
	HitObjectArguments args;
	ObjectSprite approachCircle;
};

NS_END