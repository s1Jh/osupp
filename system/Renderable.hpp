#pragma once

#include "define.hpp"

NS_BEGIN

class Renderer;

namespace detail
{
template<typename T>
class Renderable
{
public:
    using DrawInfoClass = T;
    virtual void draw(Renderer &, DrawInfoClass &) = 0;
};
}; // namespace detail

NS_END