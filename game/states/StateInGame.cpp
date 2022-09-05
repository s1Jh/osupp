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
#include "TestInput.hpp"
#include "AutoPilot.hpp"

NS_BEGIN

int State<GameState::InGame>::update(double delta)
{
	if (timer.isDone()) {
		log::debug("STARTING");
		auto info = ctx.game.getMap();
		auto musicTrack = ctx.resources.get<SoundStream>(info->getSongPath(), info->getDirectory(), false);
		auto& channel = ctx.audio.getMusicChannel();
		channel.setSound(musicTrack, true);
		if (!ctx.game.start()) {
			log::error("Failed to start the game simulation");
			ctx.state.setState(GameState::MainMenu);
			return 0;
		}
	}

	if (ctx.game.isFinished() && !endTimer.isRunning()) {
		log::debug("ENDING");
		endTimer.setTime(2.5);
		endTimer.setMode(TimerMode::Single);
		endTimer.start();
	}

	ctx.game.update(delta);

    if (ctx.keyboard[Key::Esc].releasing || endTimer.isDone()) {
		ctx.state.setState(GameState::MainMenu);
	}

    return 0;
}

int State<GameState::InGame>::draw()
{
    ctx.game.draw(ctx.gfx);
    return 0;
}

int State<GameState::InGame>::exit()
{
	ctx.audio.getMusicChannel().stop();
	ctx.game.setMap(nullptr);
	ctx.game.stop();
	return 0;
}

int State<GameState::InGame>::init(GameState)
{
    float ratio = 512.f / 384.f;

    float base = 0.8f;
    field = {{base * ratio, base}, {0.0f, 0.0f}};
    ctx.game.setPlayField(field);

	timer.setTime(0.0);
	timer.setMode(TimerMode::Single);
	timer.start();

	ctx.game.setInputMapper(std::make_unique<AutoPilot>());
	ctx.game.reset();
	ctx.audio.getMusicChannel().stop();

    return 0;
}

State<GameState::InGame>::State()
    : ctx(GetContext())
{}

NS_END

