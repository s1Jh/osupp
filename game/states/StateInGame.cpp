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

#include "StateInGame.hpp"

#include "Random.hpp"
#include "Util.hpp"

#include "Context.hpp"
#include "HumanInput.hpp"
#include "AutoPilot.hpp"

NS_BEGIN

int State<GameState::InGame>::update(double delta)
{
	if (ctx.keyboard[Key::ESC].releasing || (endTimer.isDone())) {
		ctx.state.setState(GameState::MainMenu);
	}

	playField.update(delta);
	cursor.update(delta);

	ctx.game.update(delta);

	// Timers are too unreliable to control when music should start, so we use the actual game time to start it instead
	if ((ctx.game.getCurrentTime() >= (-ctx.game.getStartOffset())) && !musicStarted) {
		musicStarted = true;

		auto info = ctx.game.getMap();
		auto& channel = ctx.audio.getMusicChannel();
		auto musicTrack = ctx.resources.get<SoundStream>(info->songPath, info->getDirectory(), false);
		channel.setSound(musicTrack.ref(), true);
	}

	if (ctx.game.isFinished() && !endTimer.isRunning()) {
		endTimer.setTime(2.5);
		endTimer.setMode(TimerMode::SINGLE);
		endTimer.start();
	}

	cursorTrail[trailIndex] = ctx.game.getCursorPosition();
	trailIndex++;
	trailIndex %= cursorTrail.size();

	if (ctx.keyboard[Key::F1].releasing)
		ctx.game.setInputMapper(std::make_unique<HumanInput>());
	if (ctx.keyboard[Key::F2].releasing)
		ctx.game.setInputMapper(std::make_unique<AutoPilot>());

	if (ctx.keyboard[Key::SPACE].releasing)
		ctx.game.skipToFirst();

    return 0;
}

int State<GameState::InGame>::draw()
{
    ctx.game.draw(ctx.gfx);

	const auto& transform = ctx.game.getTransform();

	fvec2d last = cursorTrail[trailIndex];
	auto length = (int) cursorTrail.size();

	for (int i = 1; i < length; i++) {
		int realIndex = (trailIndex + i) % length;
		fvec2d current = cursorTrail[realIndex];

		auto fill = LAVENDER;
		fill.a = float(i) / float(length);

		ctx.gfx.draw(fline{last, current}, VisualAppearance{.fillColor = fill}, transform);

		last = current;
	}

	const float cursorSize = ctx.game.getCircleSize();
	ObjectDrawInfo cursorInfo{{{cursorSize, cursorSize}, ctx.game.getCursorPosition()}, 1.0f, transform};
	ctx.gfx.draw(cursor, cursorInfo);

	ObjectDrawInfo playFieldInfo{ctx.game.getPlayField(), 1.0f, MAT3_NO_TRANSFORM<float>};
	ctx.gfx.draw(playField, playFieldInfo);

    return 0;
}

int State<GameState::InGame>::exit()
{
	ctx.audio.getMusicChannel().stop();
	ctx.game.setMap(nullptr);
	return 0;
}

int State<GameState::InGame>::init(GameState)
{
    float ratio = 512.f / 384.f;

    float base = 0.9f;
    field = {{base * ratio, base}, {0.0f, 0.0f}};
    ctx.game.setPlayField(field);

	// give the player some time before the game starts
	const float startDelay = 5.0f;
	ctx.game.reset();
	ctx.game.scrobble(-startDelay);

	ctx.game.setInputMapper(std::make_unique<HumanInput>());
	ctx.audio.getMusicChannel().stop();

	const auto& skin = ctx.activeSkin;
	cursor = skin->createObjectSprite(CURSOR_SPRITE, HitObjectArguments{});
	playField = skin->createObjectSprite(PLAY_FIELD_SPRITE, HitObjectArguments{});

    return 0;
}

State<GameState::InGame>::State()
    : ctx(GetContext())
{
	cursorTrail.resize(64, {0, 0});
}

NS_END

