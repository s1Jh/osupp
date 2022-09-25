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

#include "define.hpp"

#include "Jobs.hpp"

#include <chrono>

#ifdef WINDOWS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

using namespace PROJECT_NAMESPACE;

#if defined(WINDOWS) && defined(RELEASE)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR pCmdLine, int nCmdShow)
#else
int main()
#endif
{
    StartJobs();
    StartPersistentJob(PersistentJob::Game);

    while (JobsRunning()) {
        UpdateJobs();
    }
    StopJobs();
    return 0;
/*
    while (ctx.state.isRunning()) {
        double delta = ctx.timing.getDelta();

		ctx.keyboard.update();
		ctx.mouse.update();
		ctx.audio.process();

        if (ctx.keyboard[Key::CTRL + Key::Q].releasing)
            ctx.state.setState(GameState::EXIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ctx.gfx.begin();

//		ctx.state.update(delta);
        ctx.state.draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Render();
        auto data = ImGui::GetDrawData();
        if (data)
            ImGui_ImplOpenGL3_RenderDrawData(data);

        ctx.gfx.end();

        if (!ctx.gfx.runTasks())
            ctx.state.exit();

        ctx.state.process();

        ctx.timing.await();
    }

    log::custom("GREETING", "Goodbye, world!");
    ctx.settings.write();
	StopTimerThread();
    std::exit(0);
    */
 }