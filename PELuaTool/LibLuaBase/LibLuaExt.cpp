#include "Stdafx.h"
#include <string>
#include "util/StringEx.h"
#include <sstream>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include "LuaExt/lfs.h"
#include "LuaExt/RegIni.h"
#include "LuaExt/OsCommon.h"
#include "LuaExt/Firewall.h"
#include "LibLuaExt.h"
//#include "../LuaCJson/LuaCJson.h"

void LuaOpenExtendLib(PVOID pLuaState)
{
	lua_State *LuaS = (lua_State *)pLuaState;

	luaopen_lfs(LuaS);
	luaopen_OsCommon(LuaS);
	luaopen_regini(LuaS);
	luaopen_Firewall(LuaS);
}
