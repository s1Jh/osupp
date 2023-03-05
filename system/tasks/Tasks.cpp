//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=

#include "Tasks.hpp"
#include "Math.hpp"
#include "Log.hpp"

#include <queue>
#include <thread>
#include <memory>
#include <algorithm>
#include <array>

namespace PROJECT_NAMESPACE::tasks
{

struct ThreadHolder
{
	std::thread theThread;
	std::atomic<bool> run{true};
	std::atomic<bool> executing{false};
	std::atomic<bool> paused{true};
	std::shared_ptr<detail::BaseTaskHolder> task{nullptr};

	ThreadHolder() = default;
	void stop();

	ThreadHolder(const ThreadHolder &other);
};

static std::vector<ThreadHolder> Threads;

static std::array<ThreadHolder, (size_t)TaskID::COUNT> NamedTasks;

static std::mutex TaskQueueMutex;

static std::queue<std::shared_ptr<detail::BaseTaskHolder>> TaskQueue;

void TaskRunner(std::reference_wrapper<ThreadHolder> wrappedSelf)
{
	auto &self = wrappedSelf.get();

	using Clock = std::chrono::high_resolution_clock;

	int sleepJumpValue = 100;
	const int maxSleepTime = 1000;

	std::chrono::time_point<Clock> lastExecTP = Clock::now();

	while (self.run) {
		if (self.paused) {
			auto timeSinceLastExec = Clock::now() - lastExecTP;
			auto us =
				(int) std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceLastExec).count();

			int sleepTime = us / sleepJumpValue;
			sleepTime *= sleepJumpValue;
			sleepTime = math::Min(sleepTime, maxSleepTime);

			if (sleepTime > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			}

			continue;
		}

		if (bool(self.task)) {
			self.executing = true;
			while (!self.task->isComplete() && self.run) {
				self.task->invoke();
			}
			self.task = nullptr;
		}
		self.executing = false;
		self.paused = true;
		lastExecTP = Clock::now();
	}
	log::Debug("Stopping");
}

ThreadHolder::ThreadHolder(const ThreadHolder &)
{
}

void ThreadHolder::stop()
{
	if (task) {
		task->stop();
	}
	run = false;
	if (theThread.joinable()) {
		theThread.join();
	}
}

void detail::Enqueue(const std::shared_ptr<BaseTaskHolder> &newTask)
{
	std::lock_guard<std::mutex> lock(TaskQueueMutex);
	TaskQueue.push(newTask);
}

void detail::PutNamed(unsigned int id, const std::shared_ptr<detail::BaseTaskHolder> &ptr)
{
	auto &thread = NamedTasks[id % NamedTasks.size()];

	// If the thread is already doing something, stop that task first.
	if (thread.executing) {
		thread.task->stop();
	}

	if (std::this_thread::get_id() == thread.theThread.get_id()) {
		return;
	}

	// Wait for the task to actually finish
	while (thread.executing) {}

	// The new task.
	if (bool(ptr)) {
		thread.task = ptr;
		thread.paused = false;
	}
}

std::shared_ptr<detail::BaseTaskHolder> detail::GetNamed(unsigned int id)
{
	return NamedTasks[id % NamedTasks.size()].task;
}

void Stop()
{
	std::lock_guard<std::mutex> lock(TaskQueueMutex);
	log::Info("Stopping all threads");
	while (!TaskQueue.empty()) {
		TaskQueue.pop();
	}

	for (auto &thread : Threads) {
		thread.stop();
	}
	for (auto &thread : NamedTasks) {thread.stop();	}
}

void Start()
{
	std::lock_guard<std::mutex> lock(TaskQueueMutex);
	Threads.resize(BASE_TASK_THREAD_COUNT);

    int taskThreads = 0, persistentThreads = 0;

	for (auto &thread : Threads) {
        thread.theThread = std::thread(TaskRunner, std::ref(thread));
        taskThreads++;
	}

	for (auto &thread : NamedTasks) {
		thread.theThread = std::thread(TaskRunner, std::ref(thread));
        persistentThreads++;
	}

	log::Info("Started ", taskThreads, " task threads and ", persistentThreads, " persistent threads.");
}

int Update()
{
	int newTasksQueued = 0;
	for (auto &info : Threads) {
		if (!info.executing && info.paused) {
			std::lock_guard<std::mutex> lock(TaskQueueMutex);
			if (!TaskQueue.empty()) {
				info.task = TaskQueue.front();
				info.paused = false;
				TaskQueue.pop();
				newTasksQueued++;
			}
		}
	}

	return newTasksQueued;
}

bool Running()
{
	return std::any_of(NamedTasks.begin(), NamedTasks.end(), [](auto &info) -> bool
	{
		return info.executing || bool(info.task);
	});
}

size_t GetTaskQueueLength()
{
    std::lock_guard<std::mutex> lock(TaskQueueMutex);
    return TaskQueue.size();
}
size_t GetTaskThreadCount()
{
    return Threads.size();
}

std::vector<ThreadInfo> GetThreadStates()
{
    std::vector<ThreadInfo> infos;

    for (const auto& thread : Threads) {
        ThreadInfo inf{
            .id = thread.theThread.get_id(),
            .taskPtr = thread.task.get()
        };

        if (!thread.run) {
            inf.state = ThreadState::Dead;
        } else {
            if (thread.executing) {
                inf.state = ThreadState::Running;
            } else {
                if (thread.paused) {
                    inf.state = ThreadState::Sleeping;
                } else {
                    inf.state = ThreadState::Inactive;
                }
            }
        }
        infos.push_back(inf);
    }
    return infos;
}

}