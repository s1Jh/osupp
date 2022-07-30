#pragma once

#include "define.hpp"

NS_BEGIN

class Renderer;

#define ALL_STATES \
    USER_STATE(None, = 0x100) \
    USER_STATES \
    USER_STATE(Exit, = 0xffffffff)

enum class GameState: unsigned int
{
#define USER_STATE(Name, ...) Name __VA_ARGS__,
    ALL_STATES
#undef USER_STATE
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