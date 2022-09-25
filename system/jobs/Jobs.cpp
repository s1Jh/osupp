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
#include "Jobs.hpp"

#include "BaseJob.hpp"

#include <queue>
#include <thread>
#include <memory>

NS_BEGIN

static std::vector<
    std::pair<
        std::thread,
        std::shared_ptr<
            detail::BaseJobHolder
        >
    >
> Threads;

static std::array<
    std::pair<
        std::thread,
        std::shared_ptr<
            detail::BaseJobHolder
        >
    >,
    (size_t)PersistentJob::COUNT
> PersistentThreads;

static std::mutex JobQueueMutex;
static std::queue<std::shared_ptr<detail::BaseJobHolder>> JobQueue;

bool detail::EnqueueJob(const std::shared_ptr<BaseJobHolder>& newJob)
{
    std::lock_guard<std::mutex> lock(JobQueueMutex);
    JobQueue.push(newJob);
    return true;
}

int StopJobs()
{
    while (!JobQueue.empty()) {
        JobQueue.pop();
    }

    for (auto& pair : Threads) {
        pair.second->done = true;
        if (pair.first.joinable())
            pair.first.join();
    }
    for (auto& pair : PersistentThreads) {
        pair.second->done = true;
        if (pair.first.joinable())
            pair.first.join();
    }
    return 0;
}

int StartJobs()
{
    while (Threads.size() < BASE_JOB_THREAD_COUNT) {
        Threads.emplace_back();
        Threads.back().second = std::make_shared<detail::BaseJobHolder>(-1);
        Threads.back().second->done = true;
    }

    for (auto& pair : PersistentThreads) {
        pair.second = std::make_shared<detail::BaseJobHolder>(-1);
        pair.second->done = false;
    }
    return 0;
}

int UpdateJobs()
{
    int newJobsQueued = 0;
    for (auto& pair : Threads) {
        auto& thread = pair.first;
        auto& job = pair.second;

        if (job->done) {
            if (thread.joinable())
                thread.join();
            std::lock_guard<std::mutex> lock(JobQueueMutex);
            if (!JobQueue.empty()) {
                job = JobQueue.front();
                JobQueue.pop();
                thread = std::thread(&detail::BaseJobHolder::invokeOnce, job.get());
                newJobsQueued += thread.joinable();
            }
        }
    }
    return newJobsQueued;
}

bool JobsRunning()
{
    return std::any_of(Threads.begin(), Threads.end(), [](const auto& pair)
        { return !(pair.second->done && pair.second->valid); }
    ) || std::any_of(PersistentThreads.begin(), PersistentThreads.end(), [](const auto& pair)
        { return !(pair.second->done && pair.second->valid); }
    ) || !JobQueue.empty() ;
}

bool StartPersistentJob(PersistentJob id)
{
    auto& job = PersistentThreads[(size_t)id % PersistentThreads.size()];

    auto& thread = job.first;
    auto& holder = job.second;

    switch (id) {
#define JOB(ID, Name, Functor,...) \
        case PersistentJob::ID: holder = \
        std::shared_ptr<detail::JobHolder<Functor, int>>(new detail::JobHolder<Functor, int>{Functor(), {}}); break;
    PERSISTENT_JOBS
#undef JOB
    case PersistentJob::COUNT:
    default:
        return false;
    }

    holder->done = false;

    if (thread.joinable()) {
        return true;
    }

    thread = std::thread(&detail::BaseJobHolder::invokeOnce, holder.get());

    return thread.joinable();
}

bool StopPersistentJob(PersistentJob id)
{
    auto& job = PersistentThreads[(size_t)id % PersistentThreads.size()];

    auto& thread = job.first;
    auto& holder = job.second;
    holder->done = true;

    if (thread.joinable()) {
        thread.join();
    }
    return true;
}


NS_END
