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

#define BEGIN_TEMPLATED_RENDER_FUNCTOR_DECL(TypeT, Arg0, ...) \
namespace detail { \
template <typename T> \
struct RenderFunctor< TypeT <T>> { \
    typedef TypeT <T> RenderType; \
    const static bool enable = true; \
    void operator() (Renderer& renderer, const RenderType &object, Arg0 __VA_OPT__(,) __VA_ARGS__); \
private: \
struct PersistentRenderInfo { \
    explicit PersistentRenderInfo();

#define BEGIN_RENDER_FUNCTOR_DECL(TypeT, Arg0, ...) \
namespace detail { \
template <> \
struct RenderFunctor<TypeT> { \
    typedef TypeT RenderType; \
    const static bool enable = true; \
    void operator() (Renderer& renderer, const RenderType &object, Arg0 __VA_OPT__(,) __VA_ARGS__); \
private: \
struct PersistentRenderInfo { \
    explicit PersistentRenderInfo();

#define END_RENDER_FUNCTOR_DECL() }; static PersistentRenderInfo& getData() { \
    static PersistentRenderInfo singleton{}; return singleton; }};}

#define BEGIN_TEMPLATED_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(TypeT) \
    template <typename T> detail::RenderFunctor<TypeT<T>>::PersistentRenderInfo::PersistentRenderInfo()

#define BEGIN_RENDER_FUNCTOR_CONSTRUCTOR_DEFINITION(TypeT) \
    detail::RenderFunctor<TypeT>::PersistentRenderInfo::PersistentRenderInfo()

#define BEFRIEND_RENDER_FUNCTOR(TypeT) \
    friend struct detail::RenderFunctor<TypeT>;


NS_BEGIN

namespace detail
{

template<typename RenderT>
struct RenderFunctor
{
    const static bool enable = false;
};

}; // namespace detail

template<typename T>
concept IsDrawable = detail::RenderFunctor<T>::enable;

NS_END