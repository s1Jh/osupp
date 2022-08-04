#include "Context.hpp"

NS_BEGIN

Context &GetContext()
{
    static Context ctx;

    return ctx;
}

NS_END