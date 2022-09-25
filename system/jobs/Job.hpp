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

#include "BaseJob.hpp"
#include "BaseMessage.hpp"

#include <type_traits>
#include <tuple>
#include <queue>

NS_BEGIN

namespace detail {

template<class JobT, typename JobResultT, typename ... JobArguments>
requires std::is_invocable_v<JobT, JobArguments...> &&
    std::is_same_v<JobResultT, std::invoke_result_t<JobT, JobArguments...>>
struct JobHolder : public BaseJobHolder {
    using ResultType =std::remove_reference_t<JobResultT> ;
    using JobType = std::remove_reference_t<JobT>;

    explicit JobHolder(JobType&& jobIn, std::tuple<JobArguments...> argsIn, int runTargetIn = 1) :
        BaseJobHolder(runTargetIn), task(std::move(jobIn)), arguments(std::move(argsIn)),
        outerMessageQueue(&messageQueue[0]), innerMessageQueue(&messageQueue[1])
    {
        result = std::make_shared<ResultType>();
    }

    bool invokeOnce() override {
        *result = std::apply(task, arguments);

        {
            std::lock_guard<std::mutex> guard(messageQueueMutex);
            if (outerMessageQueue == &messageQueue[0]) {
                innerMessageQueue = &messageQueue[0];
                outerMessageQueue = &messageQueue[1];
            } else {
                outerMessageQueue = &messageQueue[0];
                innerMessageQueue = &messageQueue[1];
            }
        }

        auto& queue = *innerMessageQueue;
        while (!queue.empty()) {
            auto &msg = queue.front();
            msg->receiveSelf(this);
            queue.pop();
        }

        done = true;
        return true;
    }

    bool insertMessage(std::shared_ptr<BaseMessageHolder> ptr) override {
        std::lock_guard<std::mutex> guard(messageQueueMutex);
        outerMessageQueue->push(ptr);
        return true;
    }

    JobType task;
    std::tuple<JobArguments...> arguments;
    std::shared_ptr<ResultType> result;
    std::queue<std::shared_ptr<BaseMessageHolder>> messageQueue[2];

    std::mutex messageQueueMutex;
    std::queue<std::shared_ptr<BaseMessageHolder>> *innerMessageQueue;
    std::queue<std::shared_ptr<BaseMessageHolder>> *outerMessageQueue;
};

}

NS_END
