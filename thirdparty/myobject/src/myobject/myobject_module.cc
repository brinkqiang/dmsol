
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "myobject_module.h"

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

namespace my_object {

	sol::table open_my_object(sol::this_state L) {
		sol::state_view lua(L);
		sol::table module = lua.create_table();
		module.new_usertype<test>("test",
			sol::constructors<test(), test(int)>(),
			"value", &test::value
			);

		return module;
	}

} // namespace my_object

LUAMOD_API int luaopen_myobject(lua_State* L)
{
	return sol::stack::call_lua(L, 1, my_object::open_my_object );
}

LUAMOD_API int require_myobject(lua_State* L)
{
    luaL_requiref(L, "myobject", luaopen_myobject, 0);
    printf("lua module: require myobject\n");
    return 1;
}