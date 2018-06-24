/**
 * \file Firewall.cpp 
 * \brief 本文件将添加防火墙例外项的操作封装到LUA库Firewall中。
 *
 * Firewall库(LUA)中有以下接口：
 * - AddAuthorizedApplication(程序名，程序路径) 添加一个程序到防火墙例外中
 * - AddAuthorizedMe(程序名) 添加自己到防火墙例外中。
 * - RemoveAuthorizedApplication(程序路径)将一个程序从防火墙例外列表中删除。
 * \author duocore
 * \date 2016/10/13
 */

#include "stdafx.h"
#include "../nsisFirewall.h"
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include "Firewall.h"
#include "util/StringEx.h"
#include <string>

#define Firewall_VERSION "1.0.0.1"
#define Firewall_LIBNAME "Firewall"


/**
 * Assumes the table is on top of the stack.
 */
static void set_info (lua_State *L) 
{
	lua_pushliteral(L, "Copyright (C) 2015 - 2016 Lua everywhere");
	lua_setfield(L, -2, "_COPYRIGHT");
	lua_pushliteral(L, "Firewall is a Lua library developed to encapsure common os function.");
	lua_setfield(L, -2, "_DESCRIPTION");
	lua_pushliteral(L, "RegIni " Firewall_VERSION);
	lua_setfield(L, -2, "_VERSION");
}

/**
 * \defgroup LUA_FIREWALL LUA脚本:Firewall模块
 * \{
 */
/**
 * \brief LUA导出脚本boolean AddAuthorizedApplication()的实现， 添加一个程序到防火墙例外中
 * \remark 
 * 使用方法：AddAuthorizedApplication(程序名，程序路径)
 * <pre>
 * For example:
 * BooleanValue = AddAuthorizedApplication(程序名，程序路径)
 * </pre>
 */
int LFAddAuthorizedApplication(lua_State* l)
{
	const char* pExceptionName = lua_tostring(l,1);
	const char* pProcessPath = lua_tostring(l,2);
	std::wstring wstrExceptionName = String(pExceptionName).toStdWString();
	std::wstring wstrProcessPath = String(pProcessPath).toStdWString();
	HRESULT hResult = AddAuthorizedApplication(wstrExceptionName.c_str(), wstrProcessPath.c_str());
	lua_pushboolean(l,SUCCEEDED(hResult));
	return 1;
}

/**
 * \brief LUA导出脚本boolean AddAuthorizedMe()的实现，添加自己到防火墙例外中。
 * \remark 
 * 使用方法：AddAuthorizedMe(程序名)
 * <pre>
 * For example:
 * BooleanValue = AddAuthorizedMe(程序名)
 * </pre>
 */
int LFAddAuthorizedMe(lua_State* l)
{
	const char* pExceptionName = lua_tostring(l,1);
	std::wstring wstrExceptionName = String(pExceptionName).toStdWString();
	HRESULT hResult = AddAuthorizedApplication(wstrExceptionName.c_str());
	lua_pushboolean(l,SUCCEEDED(hResult));
	return 1;
}

/**
 * \brief LUA导出脚本boolean RemoveAuthorizedApplication()的实现，将一个程序从防火墙例外列表中删除。
 * \remark 
 * 使用方法：RemoveAuthorizedApplication(程序路径)
 * <pre>
 * For example:
 * BooleanValue = RemoveAuthorizedApplication(程序路径)
 * </pre>
 */
int LFRemoveAuthorizedApplication(lua_State* l)
{
	const char* pProcessPath = lua_tostring(l,1);
	std::wstring wstr = String(pProcessPath).toStdWString();
	HRESULT hResult = RemoveAuthorizedApplication(wstr.c_str());
	lua_pushboolean(l,SUCCEEDED(hResult));
	return 1;
}

/**
 * \brief FirewallLib结构体定义Firewall模块LUA接口和内部实现的映射关系。
 * 每个成员的第一个是导出接口名，第二个是内部实现函数名。
 * 有关LUA接口的使用见内部实现明的说明。点击链接可以查看。
 */
static const struct luaL_Reg FirewallLib[] = 
{
	{"AddAuthorizedApplication", LFAddAuthorizedApplication},
	{"AddAuthorizedMe", LFAddAuthorizedMe},
	{"RemoveAuthorizedApplication", LFRemoveAuthorizedApplication},
	{NULL, NULL},
};

/**
 * \} 
 */

#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

int luaopen_Firewall( lua_State* l )
{
	luaL_newlib (l, FirewallLib);
	lua_pushvalue(l, -1);
	lua_setglobal(l, Firewall_LIBNAME);
	set_info (l);
	return 1;
}
