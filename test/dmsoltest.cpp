
#include "sol.hpp"
#include "sol_forward.hpp"
#include "test/gtest/gtest.h"
TEST(luapb, luapb)
{
    sol::state lua;

    auto result1 = lua.safe_script("function f() end", sol::script_pass_on_error);

    for (int i=0; i < 1000000; ++i)
    {
        sol::function f = lua["f"];
        f();
    }
}
