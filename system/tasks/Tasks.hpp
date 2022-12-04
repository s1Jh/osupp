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

#include "NamedTask.hpp"
#include "Holder.hpp"
#include "Message.hpp"
#include "Result.hpp"
#include "SimpleTask.hpp"
#include "PersistentTask.hpp"
#include "CountedTask.hpp"

#define USER_PERSISTENT_TASKS_INCLUDES
#include "config.hpp"

NS_BEGIN

namespace tasks
{

namespace detail
{
void Enqueue(const std::shared_ptr<detail::BaseTaskHolder> &);

void PutNamed(unsigned int id, const std::shared_ptr<detail::BaseTaskHolder> &);

std::shared_ptr<detail::BaseTaskHolder> GetNamed(unsigned int id);
}

bool Running();

int Update();

void Start();

void Stop();

template<typename TaskT, typename ... TaskArgsT>
auto MakeSimple(TaskT &&task, TaskArgsT &&... args)
{
	using InnerHolderT = detail::SimpleTaskHolder<TaskT, TaskArgsT...>;
	using HolderT = detail::TaskHolder<std::invoke_result_t<TaskT, TaskArgsT...>, TaskT>;
	using ReturnT = Result<HolderT>;

	auto holder = std::make_shared<InnerHolderT>(std::forward<TaskT>(task), std::tuple(args...));
	detail::Enqueue(holder);
	return ReturnT{holder};
}

template<typename TaskT, typename ... TaskArgsT>
auto MakePersistent(TaskT &&task, TaskArgsT &&... args)
{
	using InnerHolderT = detail::PersistentTaskHolder<TaskT, TaskArgsT...>;
	using HolderT = detail::TaskHolder<std::invoke_result_t<TaskT, TaskArgsT...>, TaskT>;
	using ReturnT = Result<HolderT>;

	auto holder = std::make_shared<InnerHolderT>(std::forward<TaskT>(task), std::tuple(args...));
	detail::Enqueue(holder);
	return ReturnT{holder};
}

template<typename TaskT, typename ... TaskArgsT>
auto MakeCounted(int runTarget, TaskT &&task, TaskArgsT &&... args)
{
	using InnerHolderT = detail::CountedTaskHolder<TaskT, TaskArgsT...>;
	using HolderT = detail::TaskHolder<std::invoke_result_t<TaskT, TaskArgsT...>, TaskT>;
	using ReturnT = Result<HolderT>;

	auto holder = std::make_shared<InnerHolderT>(runTarget, std::forward<TaskT>(task), std::tuple(args...));
	detail::Enqueue(holder);
	return ReturnT{holder};
}

template<TaskID ID>
auto GetNamed()
{
	using Info = NamedTaskInfo<ID>;
	using Functor = typename Info::Functor;
	using ReturnType = typename Info::ReturnValue;
	using HolderT = detail::TaskHolder<ReturnType, Functor>;
	using ReturnT = Result<HolderT>;

	auto task = detail::GetNamed((unsigned int)ID);
	auto cast = std::reinterpret_pointer_cast<HolderT>(task);
	return ReturnT{cast};
}

template<TaskID ID, typename ... TaskArgsT>
requires NamedTaskInfo<ID>::enable
auto StartNamed(TaskArgsT &&... args)
{
	using Info = NamedTaskInfo<ID>;
	using Functor = typename Info::Functor;
	using HolderT = detail::PersistentTaskHolder<Functor, TaskArgsT...>;

	auto holder = std::make_shared<HolderT>(Functor(), std::tuple(args...));
	detail::PutNamed((unsigned int)ID, holder);
	return GetNamed<ID>();
}

template<TaskID ID>
void StopNamed()
{
	detail::PutNamed((unsigned int)ID, nullptr);
}

}

NS_END
