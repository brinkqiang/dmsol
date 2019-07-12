#define SOL_PRINT_ERRORS  1
#define SOL_NO_EXCEPTIONS 1

#include "sol.hpp"
#include "sol_forward.hpp"
#include "gtest/gtest.h"

#define TEST_MAX_COUNT 10000

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
TEST(lua_pbcodec_single, lua_pbcodec_single)
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
        "local pb = require \"luapb\"\n"
        "pb:import(\"net.proto\")\n"
        "function pbtest()\n"
        "local msg = {}\n"
        "msg.number = \"13615632545\"\n"
        "msg.email = \"13615632545@163.com\"\n"
        "msg.age = 28\n"
        "msg.ptype = 2\n"
        "msg.desc = {}\n"
        "msg.desc[1] = \"first\"\n"
        "msg.desc[2] = \"second\"\n"
        "msg.desc[3] = \"three\"\n"

        "local buffer = pb:encode(\"net.tb_Person\", msg)\n"
        "local msg2 = pb:decode(\"net.tb_Person\", buffer)\n"
        "end\n", sol::script_throw_on_error);
    if (!result1.valid())
    {
        ASSERT_TRUE(0);
        return;
    }

    sol::function f = lua["pbtest"];
    f();
}

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>
#include "myobject_module.h"

#include <iostream>

TEST(lua_require, lua_require)
{
	std::cout << "=== require from DLL ===" << std::endl;

	sol::state lua;
	lua.open_libraries(sol::lib::package, sol::lib::base);

	const auto& code = R"(
    mo = require("myobject")

    obj = mo.test.new(24)
    print(obj.value))";
	auto script_result = lua.safe_script(code, &sol::script_pass_on_error);
	if (script_result.valid()) {
		std::cout << "The DLL was require'd from successfully!" << std::endl;
	}
	else {
		sol::error err = script_result;
		std::cout << "Something bad happened: " << err.what() << std::endl;
	}
	assert(script_result.valid());
	my_object::test& obj = lua["obj"];
	assert(obj.value == 24);
}