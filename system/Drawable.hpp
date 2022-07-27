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