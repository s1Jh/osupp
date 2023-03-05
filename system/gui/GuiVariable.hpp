// Copyright (c) 2023 sijh (s1Jh.199[at]gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//
// Created by sijh on 04.02.23.
//
#pragma once

#include "define.hpp"

#include "GuiPreDecls.hpp"

#include <type_traits>
#include <memory>
#include <mutex>
#include <optional>

namespace PROJECT_NAMESPACE {

namespace gui
{
    namespace detail
    {
        class IVariable
        {
        };
    }

    template <typename T>
        requires std::is_default_constructible_v<T>
    class Variable : public detail::IVariable
    {
    public:
        void set(const T &newValue)
        {
            std::lock_guard<std::mutex> lock(data->mut);
            data->value = newValue;
        }

        const T &get() const
        {
            std::lock_guard<std::mutex> lock(data->mut);
            return data->value;
        }

    private:
        struct HeldData
        {
            bool changed{true};
            T value{};
            std::mutex mut;
        };
        std::shared_ptr<HeldData> data;
    };
}

}
