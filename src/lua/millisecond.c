#ifdef __cplusplus
	#include "lua.hpp"
#else
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
#endif


#if defined(_WIN32)
#include <windows.h>

const int CLOCK_BOOTTIME = 0;

struct timespec { long tv_sec; long tv_nsec; };   
int clock_gettime(int clk_id, struct timespec *spec) 
{  
	long long wintime; 
	GetSystemTimeAsFileTime((FILETIME*)&wintime);
	wintime      -= 116444736000000000ULL;  //1jan1601 to 1jan1970
	spec->tv_sec  = (long)(wintime / 10000000ULL);           //seconds
	spec->tv_nsec = (long)(wintime % 10000000ULL *100);      //nano-seconds
	return 0;
}
#else
#include <stdint.h>
#include <time.h>
#endif

static int l_start(lua_State *L)
{
	struct timespec* start = (struct timespec*)lua_newuserdata(L, sizeof(struct timespec));
	clock_gettime(CLOCK_BOOTTIME, start);
	return 1;
}

static int l_stop(lua_State *L)
{
	struct timespec *start = (struct timespec *)lua_touserdata(L, 1);
	struct timespec EndTime;
	clock_gettime(CLOCK_BOOTTIME, &EndTime);
 	long diff = (long)(((long long)(EndTime.tv_sec * 1000000000ULL + EndTime.tv_nsec) - (long long)(start->tv_sec * 1000000000ULL + start->tv_nsec)) / 1000000);
    lua_pushinteger(L, diff);
    return 1;
}

LUAMOD_API int luaopen_millisecond(lua_State* L)
{
    luaL_checkversion(L);
    luaL_Reg l[] = {
        { "start", l_start },
        { "stop", l_stop },
        { NULL, NULL },
    };
    luaL_newlib(L, l);
	return 1;
}

//-------------------- -------------------- //
// 这个提供c/c++代码中注册，不用编译为dll/so, 需要增加一个头文件

//static int lua_namespace (lua_State *L) {
//  luaL_newlib(L, funs);
//  return 1;
//}
//
//int require_millisecond(lua_State* L)
//{
//	luaL_requiref(L,"millisecond",lua_namespace,0);
//	return 1;
//}

/*
lua example:

	local ms = require ("millisecond")
	local begin = ms.start()
	os.execute("ping localhost > NUL") 
	print(ms.stop(begin), "ms")



*/

/* CMakeFiles.txt
if (UNIX AND NOT APPLE)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()
*/