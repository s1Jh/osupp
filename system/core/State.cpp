#include "State.hpp"

NS_BEGIN

namespace detail
{

int BaseState::update(double)
{
    return 0;
}
int BaseState::draw()
{
    return 0;
}
int BaseState::exit()
{
    return 0;
}
int BaseState::init(GameState)
{
    return 0;
}

}

NS_END