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
#include <optional>
#include <atomic>

NS_BEGIN

namespace video
{

class RenderingHandler;

template <typename GLRepr> requires std::is_default_constructible_v<GLRepr>
class GLResource
{
public:
	GLResource(const GLResource& other)
	{
		bool otherState = other.wasUploaded;
		wasUploaded = otherState;
		data = std::make_unique<GLRepr>(*other.data);
	}

	GLResource(GLResource&& other) noexcept
	{
		bool otherState = other.wasUploaded;
		wasUploaded = otherState;
		data = std::make_unique<GLRepr>(*other.data);
	}

	inline const GLRepr &getGLData() const {
		return *data;
	}
	inline bool uploaded() const { return wasUploaded; }

	inline bool upload() {
		auto newData = createData();
		if (bool(newData)) {
			invalidate();
			*data = newData.value();
			wasUploaded = true;
			return true;
		}
		return false;
	};

protected:
	GLResource() : wasUploaded(false) { data = std::make_shared<GLRepr>(); }

	virtual std::optional<GLRepr> createData() = 0;
	virtual void deleteData(const GLRepr&) = 0;

	inline void invalidate() {
		wasUploaded = false;
		deleteData(*data);
	}

private:
	std::shared_ptr<GLRepr> data;
	std::atomic<bool> wasUploaded;
};

}

NS_END