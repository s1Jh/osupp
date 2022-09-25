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

#include "Traits.hpp"
#include "Job.hpp"
#include "Message.hpp"

#include <optional>

NS_BEGIN

template<class Holder> requires IsInstance<Holder, detail::JobHolder>
class JobResult {
public:
    using JobType = typename Holder::JobType;
    using HeldValue = typename Holder::ResultType;

    explicit JobResult(std::weak_ptr<Holder> ptr) : origin(std::move(ptr)) {}

    [[nodiscard]] bool complete() const {
        if (auto ptr = origin.lock()) {
            return ptr->done;
        }
        return true;
    }

    [[nodiscard]] bool valid() const {
        if (auto ptr = origin.lock()) {
            return ptr->valid;
        }
        return false;
    }

    // Will wait for the result before returning
    HeldValue waitResult() {
        if (!valid())
            return HeldValue{};

        while (!complete()) {}
        auto res = getResult();

        if (res) {
            return *res;
        } return HeldValue{};
    }
    // Returns a empty optional if the result hasn't been calculated
    std::optional<HeldValue> getResult() {
        if (auto ptr = origin.lock()) {
            if (ptr->done && ptr->valid) {
                return *ptr->result;
            } else {
                return false;
            }
        }
        return false;
    }

    // Send a message to the job
    template <class MessageT>
    bool message(MessageT& msg) {
        return message(std::forward(msg));
    }

    template <class MessageT>
    bool message(MessageT&& msg) {
        if (auto ptr = origin.lock()) {
            if (!ptr->valid)
                return false;

            auto wrapped = std::make_shared<detail::MessageHolder<MessageT, Holder>>(msg);
            return ptr->insertMessage(std::static_pointer_cast<detail::BaseMessageHolder>(wrapped));
        }
        return false;
    }

private:
    std::weak_ptr<Holder> origin;
};

NS_END
