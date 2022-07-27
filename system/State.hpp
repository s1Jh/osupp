#pragma once

#include "define.hpp"

NS_BEGIN

class Renderer;

enum class GameState: unsigned int
{
    None = 0x100,

#define USER_STATE(Name) Name,
    USER_STATES
#undef USER_STATE

    Exit = 0xffffffff
};

namespace detail
{

class BaseState
{
public:
    virtual int update(double);
    virtual int draw();
    virtual int exit();
    virtual int init(GameState);

    virtual ~BaseState() = default;
};
}

template<GameState Name>
class State
{
};

#define DECLARE_STATE(StateName) \
    template<> class State<StateName> : public detail::BaseState

NS_END