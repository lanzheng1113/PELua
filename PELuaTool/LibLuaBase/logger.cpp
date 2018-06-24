#include "stdafx.h"
#include "util/Logger.h"
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include "logger.h"

#define LOG_VERSION "1.0.0.1"
#define LOG_LIBNAME "Log"
/*
** Assumes the table is on top of the stack.
*/
static void set_info(lua_State *L)
{
	lua_pushliteral(L, "Copyright (C) 2015 - 2018 Lua everywhere");
	lua_setfield(L, -2, "_COPYRIGHT");
	lua_pushliteral(L, "Log is a Lua library developed to encapsure log function.");
	lua_setfield(L, -2, "_DESCRIPTION");
	lua_pushliteral(L, "OsExt " LOG_VERSION);
	lua_setfield(L, -2, "_VERSION");
}


/**
* \brief 打印日志到DEBUGVIEW
*/
int LFOutputDebugString(lua_State *l)
{
	const char* lpMsg = lua_tostring(l, 1);
	if (lpMsg)
	{
		OutputDebugStringA(lpMsg);
	}
	return 0;
}

int LFSetLogFile(lua_State *l)
{
	const char* pLogFileName = lua_tostring(l, 1);
	Logger::getInstance()->setLogFileName(pLogFileName);
	return 0;
}

int LFLogDebug(lua_State *l)
{
	const char* pMsg = lua_tostring(l, 1);
	LOG_DEBUG("%s", pMsg);
	return 0;
}

int LFLogInfo(lua_State *l)
{
	const char* pMsg = lua_tostring(l, 1);
	LOG_INFO("%s", pMsg);
	return 0;
}

int LFLogWarn(lua_State* l)
{
	const char* pMsg = lua_tostring(l, 1);
	LOG_WARN("%s", pMsg);
	return 0;
}

int LFLogError(lua_State* l)
{
	const char* pMsg = lua_tostring(l, 1);
	LOG_ERROR("%s", pMsg);
	return 0;
}

static const struct luaL_Reg LogLib[] = {
	{ "OutputDebugString",LFOutputDebugString },
	{ "SetLogFile",LFSetLogFile },
	{ "debug",LFLogDebug },
	{ "info",LFLogInfo },
	{ "warn",LFLogWarn },
	{ "error",LFLogError },
	{ NULL, NULL },
};

/**
* \}
*/

#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

int luaopen_Log(lua_State* l)
{
	luaL_newlib(l, LogLib);
	lua_pushvalue(l, -1);
	lua_setglobal(l, LOG_LIBNAME);
	set_info(l);
	return 1;
}
