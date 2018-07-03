#include "stdafx.h"
#include <objbase.h>
//lua support
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include "util/Logger.h"
#include "../LibLuaBase/LibLuaExt.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib,"../x64/Debug/LibLuaBase.lib")
#else
#pragma comment(lib,"../x64/Release/LibLuaBase.lib")
#endif // DEBUG

#else
#ifdef _DEBUG
#pragma comment(lib,"../Debug/LibLuaBase.lib")
#else
#pragma comment(lib,"../Release/LibLuaBase.lib")
#endif // DEBUG

#endif


int luaopen_PE(lua_State* l);
lua_State *LuaS; //全局唯一的LUA解释器指针。
/**
 * \brief 初始化LUA解释器、导入LUA标准库、导入铠甲拓展库和CJson库
 */
static void initLuaScript()
{
	LuaS = lua_open();		//初始化指针
	luaL_openlibs(LuaS);	//打开标准库
	LuaOpenExtendLib(LuaS); //打开我们的扩展库
	luaopen_PE(LuaS);
	//luaopen_cjson(LuaS);	//打开CJson库
	return;
}

#include "util/StringConverter.h"

/**
* \brief 执行一段lua脚本。
* \detail 如果没有初始化LUA脚本解释器则初始化并导入扩展模块.
* \param strScript脚本字符串。注意不是脚本名，不是文件名！
* \return 成功返回true否则返回false.
*/
HRESULT PE_Text(LPCTSTR pszText, COLORREF color, LPCTSTR font, int FontSize, long left, long top, long right, long bottom);
bool RunLuaScript(const std::string& strScript)
{
	bool bRet = false;
	//Firewall模块（LUA）使用了COM组件，需要初始化一下COM组件
	CoInitialize(NULL);
	//std::string astrPwd =Poco::Path::getApplicationDirPath();
	static bool isInited = false;
	if (!isInited)
	{
		isInited = true;
		initLuaScript();
	}

	//string utf8str = StringConverter::LocalStringToUtf8(strScript);
	int iRet = luaL_dostring(LuaS, strScript.c_str());
	if (iRet != 0)
	{
		int t = lua_type(LuaS, -1);
		const char* err = lua_tostring(LuaS, -1);
		LOG_ERROR("LUA Error: %s\n", err);
		WCHAR pMsg[4096] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, err, -1, pMsg, _countof(pMsg));
		PE_Text(pMsg, RGB(255, 0, 0), _T("Microsoft YaHei"), 40, 200, 100, 800, 600);
		MessageBoxA(NULL, err, err, MB_OK);
		lua_pop(LuaS, 1);
		//pvs Delete
		//bRet = false;
	}
	CoUninitialize();
	bRet = true;
	return bRet;
}