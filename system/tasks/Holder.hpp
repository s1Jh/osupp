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
#pragma once

#include "define.hpp"

#include <memory>
#include <queue>
#include <mutex>
#include <thread>

#include "Message.hpp"

NS_BEGIN

namespace tasks
{

constexpr unsigned int MAX_TASK_MESSAGE_QUEUE_LENGTH = 128;

namespace detail
{

struct BaseMessageHolder;

struct BaseTaskHolder
{
	inline virtual bool invoke()
	{ return false; };

	inline virtual void receive(const std::shared_ptr<BaseMessageHolder> &)
	{};

	inline virtual void stop()
	{};

	inline virtual bool isComplete() const
	{ return true; };
};

template<typename ResultT, typename TaskT> requires std::is_default_constructible_v<ResultT>
struct TaskHolder: public BaseTaskHolder
{
	typedef std::remove_cvref_t<ResultT> ResultType;
	typedef std::remove_cvref_t<TaskT> TaskType;

	explicit TaskHolder(TaskT &&taskIn)
		: task(std::move(taskIn))
	{}

	std::unique_ptr<ResultType> result;
	TaskType task;
};

template<typename TaskT, typename ... TaskArgsT> requires std::is_default_constructible_v<
	std::invoke_result_t<TaskT, TaskArgsT...>>
struct WrappedTaskHolder: public TaskHolder<std::invoke_result_t<TaskT, TaskArgsT...>, TaskT>
{
	using Parent = TaskHolder<std::invoke_result_t<TaskT, TaskArgsT...>, TaskT>;
	typedef std::remove_cvref_t<TaskT> TaskType;
	typedef std::tuple<TaskArgsT...> TaskArgumentsType;
	typedef std::invoke_result_t<TaskT, TaskArgsT...> TaskResultType;

	WrappedTaskHolder(TaskType &&taskIn, TaskArgumentsType &&argsIn)
		: Parent(std::move(taskIn)), outerMessageQueue(&messageQueues[0]),
		  innerMessageQueue(&messageQueues[1]), args(argsIn)
	{}

	bool invoke() override
	{
		executionId = std::this_thread::get_id();
		if (!this->result) {
			this->result = std::make_unique<TaskResultType>();
		}

		if (!this->isComplete()) {
			*(this->result) = std::apply(this->task, args);
		}

		this->onIteration();

		{
			std::lock_guard<std::mutex> guard(messageQueueMutex);
			if (outerMessageQueue == &messageQueues[0]) {
				innerMessageQueue = &messageQueues[0];
				outerMessageQueue = &messageQueues[1];
			} else {
				outerMessageQueue = &messageQueues[0];
				innerMessageQueue = &messageQueues[1];
			}
		}

		auto &queue = *innerMessageQueue;
		while (!queue.empty()) {
			auto &msg = queue.front();
			msg->receiveSelf(this);
			queue.pop();
		}

		return this->isComplete();
	}

	void receive(const std::shared_ptr<BaseMessageHolder> &msg) override
	{
		std::lock_guard<std::mutex> guard(messageQueueMutex);
		if (std::this_thread::get_id() == executionId) {
			innerMessageQueue->push(msg);
		} else if (outerMessageQueue->size() <= MAX_TASK_MESSAGE_QUEUE_LENGTH) {
			outerMessageQueue->push(msg);
		}
	}

	virtual void onIteration()
	{}

	std::queue<std::shared_ptr<BaseMessageHolder>> messageQueues[2];
	std::mutex messageQueueMutex;
	std::queue<std::shared_ptr<BaseMessageHolder>> *outerMessageQueue;
	std::queue<std::shared_ptr<BaseMessageHolder>> *innerMessageQueue;
	std::thread::id executionId;

	TaskArgumentsType args;
};

}

}

NS_END
