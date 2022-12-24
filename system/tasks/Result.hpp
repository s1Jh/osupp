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

#include "Message.hpp"
#include "Response.hpp"

#include <memory>
#include <type_traits>
#include <optional>

NS_BEGIN

namespace tasks {

template<typename TaskHolderT>
class Result {
	typedef std::remove_cvref_t<TaskHolderT> HolderType;
	typedef typename HolderType::ResultType ResultType;
public:
	Result() = default;
	explicit Result(std::shared_ptr<HolderType> originIn) : origin(originIn) {}

	[[nodiscard]] bool isComplete() const {
		if (!origin)
			return true;

		return origin->isComplete();
	}

	[[nodiscard]] std::optional<ResultType> getResult() const {
		if (!origin) {
			return {};
		}

		if (!isComplete()) {
			return {};
		}
		auto&& copy = *origin->result;
		return {copy};
	}

	[[nodiscard]] ResultType waitResult() const {
		if (!origin) {
			return {};
		}

		while (!isComplete()) {}

		return *origin->result;
	}

	[[nodiscard]] bool isValid() const {
		return bool(origin);
	}

	template<typename MessageT>
	auto message(MessageT &&msg) {
		using MessageHolderType = detail::MessageHolder<MessageT, HolderType>;
		using ReturnType = Response<MessageHolderType>;

		auto wrapped = std::make_shared<MessageHolderType>(msg, origin);
		ReturnType returnValue{wrapped};
		origin->receive(std::reinterpret_pointer_cast<detail::BaseMessageHolder>(wrapped));
		return returnValue;
	}

	template<typename MessageT>
	auto message(const MessageT &msg) {
		return message(std::forward(msg));
	}

private:
	std::shared_ptr<HolderType> origin{nullptr};
};

}

NS_END
