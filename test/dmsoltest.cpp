

#include "sol.hpp"
#include "sol_forward.hpp"
#include "test/gtest/gtest.h"
TEST(lua_empty_call, lua_empty_call)
{
    sol::state lua;

    auto result1 = lua.safe_script("function f() end", sol::script_pass_on_error);

    for (int i=0; i < 1000000; ++i)
    {
        sol::function f = lua["f"];
        f();
    }
    //lua_empty_call.lua_empty_call (157 ms)
}
TEST(lua_tb_create, lua_tb_create)
{
    sol::state lua;

    auto result1 = lua.safe_script("function f()"
        "local ret = {}"
        "ret.number = \"13615632545\""
        "ret.email = \"13615632545@163.com\""
        "ret.age = 28"
        "ret.ptype = 2"
        "ret.desc = {}"
        "ret.desc[1] = \"first\""
        "ret.desc[2] = \"second\""
        "ret.desc[3] = \"three\""
        "end", sol::script_pass_on_error);

    for (int i = 0; i < 1000000; ++i)
    {
        sol::function f = lua["f"];
        f();
    }
    //lua_tb_create.lua_tb_create (1655 ms)
}
