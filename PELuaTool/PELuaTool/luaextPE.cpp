//lua support
#include "stdafx.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include <string>
#include "util/StringEx.h"
#include "util/Logger.h"
using std::string;
using std::wstring;

#include "pe.h"

#ifndef WS2S
#define WS2S(w) String::fromStdWString(w)
#endif // WS2S

#ifndef S2WS
#define S2WS(a) String(a).toStdWString()
#endif

#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

int LFINIT(lua_State *l)
{
	PE_INIT();
	return 0;
}

int LFSHELL(lua_State* l)
{
	const char* shell = lua_tostring(l, 1);
	PE_SHELL(S2WS(shell).c_str());
	return 0;
}

int LFSetDesktopWallPaper(lua_State* l)
{
	const char* pBackGroundImagePath = lua_tostring(l, 1);
	BOOL bRet = FALSE;
	if (pBackGroundImagePath)
	{
		bRet = S_OK == PE_Wall(String(pBackGroundImagePath).toStdWString().c_str());
	}
	lua_pushboolean(l,bRet);
	return 1;
}

int LFGetDesktopSize(lua_State* l)
{
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	lua_pushinteger(l, width);
	lua_pushinteger(l, height);
	return 2;
}

int LFSetEnvironment(lua_State* l)
{
	const char* pEnvName = lua_tostring(l, 1);
	const char* pEnvValue = lua_tostring(l, 2);
	BOOL isSystem = lua_toboolean(l, 3);
	BOOL bRet = PE_SetEnvironment(S2WS(pEnvName), S2WS(pEnvValue), isSystem);
	lua_pushboolean(l, bRet);
	return 1;
}

int LFSetDesktopText(lua_State* l)
{
	const char* pText = lua_tostring(l, 1);
	lua_Integer rgbColor = lua_tointeger(l, 2);
	const char* pFontName = lua_tostring(l, 3);
	lua_Integer FontSize = lua_tointeger(l, 4);
	// 如果left,top == -1,-1
	// 那么居中显示
	lua_Integer xleft = lua_tointeger(l, 5);
	lua_Integer ytop = lua_tointeger(l, 6);
	lua_Integer xright = lua_tointeger(l, 7);
	lua_Integer ybotton = lua_tointeger(l, 8);
	PE_Text(pText ? String(pText).toStdWString().c_str() : NULL,
			rgbColor, 
			pFontName ? String(pFontName).toStdWString().c_str() : NULL, 
			FontSize, xleft, ytop, xright, ybotton);
	return 0;
}

int LFShowUSBDisk(lua_State* l)
{
	LOG_INFO("未实现ShowUSBDisk");
	return 0;
}

int LFSetLogo(lua_State* l)
{
	const char* pImagePath = lua_tostring(l, 1);
	COLORREF dwColor =  lua_tointeger(l,2);
	BOOL bRet = FALSE;
	if (pImagePath == NULL)
	{
		LOG_INFO("清除LOGO窗口");
		bRet = PE_SetLogo(NULL, dwColor);
	}
	else
	{
		LOG_INFO("创建LOGO,背景=[%s],颜色=0x%08x", pImagePath, dwColor);
		bRet = PE_SetLogo(String(pImagePath).toStdWString().c_str(), dwColor);
	}
	lua_pushboolean(l, bRet);
	return 1;
}

int LFInitEnvironmentVariable(lua_State* l)
{
	InitEnvironmentVariable();
	return 0;
}

static const struct luaL_Reg PEExtLib[] = {
	{ "INIT", LFINIT },
	{ "SHELL", LFSHELL },
	{ "SetLogo", LFSetLogo },
	{ "SetDesktopWallPaper", LFSetDesktopWallPaper },
	{ "GetDesktopSize", LFGetDesktopSize },
	{ "SetEnvironment", LFSetEnvironment },
	{ "SetDesktopText", LFSetDesktopText },
	{ "ShowUSBDisk", LFShowUSBDisk },
	{ "InitEnvironmentVariable", LFInitEnvironmentVariable },
	{ NULL, NULL },
};


#define PEEXT_VERSION "1.0.0.1"
#define PEEXT_LIBNAME "PEExt"

/*
** Assumes the table is on top of the stack.
*/
static void set_info(lua_State *L)
{
	lua_pushliteral(L, "Copyright (C) 2015 - 2016 Lua everywhere");
	lua_setfield(L, -2, "_COPYRIGHT");
	lua_pushliteral(L, "PEExt is a Lua library developed to encapsure PECMD function.");
	lua_setfield(L, -2, "_DESCRIPTION");
	lua_pushliteral(L, "PEExt " PEEXT_VERSION);
	lua_setfield(L, -2, "_VERSION");
}

int luaopen_PE(lua_State* l)
{
	luaL_newlib(l, PEExtLib);
	lua_pushvalue(l, -1);
	lua_setglobal(l, PEEXT_LIBNAME);
	set_info(l);
	return 1;
}

