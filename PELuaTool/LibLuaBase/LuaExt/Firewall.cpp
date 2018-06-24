/**
 * \file Firewall.cpp 
 * \brief ���ļ�����ӷ���ǽ������Ĳ�����װ��LUA��Firewall�С�
 *
 * Firewall��(LUA)�������½ӿڣ�
 * - AddAuthorizedApplication(������������·��) ���һ�����򵽷���ǽ������
 * - AddAuthorizedMe(������) ����Լ�������ǽ�����С�
 * - RemoveAuthorizedApplication(����·��)��һ������ӷ���ǽ�����б���ɾ����
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
 * \defgroup LUA_FIREWALL LUA�ű�:Firewallģ��
 * \{
 */
/**
 * \brief LUA�����ű�boolean AddAuthorizedApplication()��ʵ�֣� ���һ�����򵽷���ǽ������
 * \remark 
 * ʹ�÷�����AddAuthorizedApplication(������������·��)
 * <pre>
 * For example:
 * BooleanValue = AddAuthorizedApplication(������������·��)
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
 * \brief LUA�����ű�boolean AddAuthorizedMe()��ʵ�֣�����Լ�������ǽ�����С�
 * \remark 
 * ʹ�÷�����AddAuthorizedMe(������)
 * <pre>
 * For example:
 * BooleanValue = AddAuthorizedMe(������)
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
 * \brief LUA�����ű�boolean RemoveAuthorizedApplication()��ʵ�֣���һ������ӷ���ǽ�����б���ɾ����
 * \remark 
 * ʹ�÷�����RemoveAuthorizedApplication(����·��)
 * <pre>
 * For example:
 * BooleanValue = RemoveAuthorizedApplication(����·��)
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
 * \brief FirewallLib�ṹ�嶨��Firewallģ��LUA�ӿں��ڲ�ʵ�ֵ�ӳ���ϵ��
 * ÿ����Ա�ĵ�һ���ǵ����ӿ������ڶ������ڲ�ʵ�ֺ�������
 * �й�LUA�ӿڵ�ʹ�ü��ڲ�ʵ������˵����������ӿ��Բ鿴��
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
