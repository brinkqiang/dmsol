

#include "sol.hpp"
#include "sol_forward.hpp"
#include "test/gtest/gtest.h"

#define TEST_MAX_COUNT 1000000
TEST(lua_empty_call, lua_empty_call)
{
    sol::state lua;
    lua.open_libraries();
    auto result1 = lua.safe_script("function f() end", sol::script_pass_on_error);

    for (int i=0; i < TEST_MAX_COUNT; ++i)
    {
        sol::function f = lua["f"];
        f();
    }
    //lua_empty_call.lua_empty_call (157 ms) -> call
    //lua_empty_call.lua_empty_call (1599 ms) -> pcall
}
TEST(lua_tb_create, lua_tb_create)
{
    sol::state lua;
    lua.open_libraries();
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


    for (int i = 0; i < TEST_MAX_COUNT; ++i)
    {
        sol::function f = lua["f"];
        f();
    }
    //lua_tb_create.lua_tb_create (1655 ms)
    //lua_tb_create.lua_tb_create (8294 ms) -> pcall
}
//
//TEST(lua_pbcodec_single, lua_pbcodec_single)
//{
//    sol::state lua;
//    lua.open_libraries();
//
//    auto result0 = lua.load_file("pbcodec.lua");
//    if (!result0.valid())
//    {
//        ASSERT_TRUE(0);
//        return;
//    }
//    auto result1 = lua.safe_script(
//        "local pbload = require \"pbcodec\""
//        "pbcodec:load_file(\"net.proto\")"
//
//        "function pbtest()"
//        "local msg = {}"
//        "msg.number = \"13615632545\""
//        "msg.email = \"13615632545@163.com\""
//        "msg.age = 28"
//        "msg.ptype = 2"
//        "msg.desc = {}"
//        "msg.desc[1] = \"first\""
//        "msg.desc[2] = \"second\""
//        "msg.desc[3] = \"three\""
//        "local msg_data = { proto = \"net.tb_Person\", data = pbload:encode(\"net.tb_Person\", msg) }"
//        "local msg2 = pbload:decode(\"net.tb_Person\", msg_data.data)"
//        "msg.desc = {}"
//        "end", sol::script_pass_on_error);
//    if (!result1.valid())
//    {
//        ASSERT_TRUE(0);
//        return;
//    }
//
//    sol::function f = lua["pbtest"];
//    f();
//}

TEST(lua_pbcodec, lua_pbcodec)
{
    sol::state lua;
    lua.open_libraries();

    auto result0 = lua.load_file("pbcodec.lua");
    if (!result0.valid())
    {
        ASSERT_TRUE(0);
        return;
    }
    auto result1 = lua.safe_script(
        "local pbload = require \"pbcodec\""
        "pbcodec:load_file(\"net.proto\")"

        "function pbtest()"
        "local msg = {}"
        "msg.number = \"13615632545\""
        "msg.email = \"13615632545@163.com\""
        "msg.age = 28"
        "msg.ptype = 2"
        "msg.desc = {}"
        "msg.desc[1] = \"first\""
        "msg.desc[2] = \"second\""
        "msg.desc[3] = \"three\""
        "local msg_data = { proto = \"net.tb_Person\", data = pbload:encode(\"net.tb_Person\", msg) }"
        "local msg2 = pbload:decode(\"net.tb_Person\", msg_data.data)"
        "end", sol::script_pass_on_error);
    if (!result1.valid())
    {
        ASSERT_TRUE(0);
        return;
    }
    
    sol::function f = lua["pbtest"];
    for (int i = 0; i < TEST_MAX_COUNT; ++i)
    {
        f();
    }

    //lua_tb_create.lua_tb_create (1655 ms)
    //lua_tb_create.lua_tb_create (8294 ms) -> pcall
}
