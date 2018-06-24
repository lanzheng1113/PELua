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

#ifdef _DEBUG
#pragma comment(lib,"../Debug/LibLuaBase.lib")
#else
#pragma comment(lib,"../Release/LibLuaBase.lib")
#endif // DEBUG



lua_State *LuaS; //ȫ��Ψһ��LUA������ָ�롣
/**
 * \brief ��ʼ��LUA������������LUA��׼�⡢����������չ���CJson��
 */
static void initLuaScript()
{
	LuaS = lua_open();		//��ʼ��ָ��
	luaL_openlibs(LuaS);	//�򿪱�׼��
	LuaOpenExtendLib(LuaS); //�����ǵ���չ��
	//luaopen_cjson(LuaS);	//��CJson��
	return;
}

#include "util/StringConverter.h"

/**
* \brief ִ��һ��lua�ű���
* \detail ���û�г�ʼ��LUA�ű����������ʼ����������չģ��.
* \param strScript�ű��ַ�����ע�ⲻ�ǽű����������ļ�����
* \return �ɹ�����true���򷵻�false.
*/
bool RunLuaScript(const std::string& strScript)
{
	bool bRet = false;
	//Firewallģ�飨LUA��ʹ����COM�������Ҫ��ʼ��һ��COM���
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
		LOG_ERROR("LUA", "Error: %s\n", err);
		MessageBoxA(NULL, err, NULL, MB_OK);
		lua_pop(LuaS, 1);
		//pvs Delete
		//bRet = false;
	}
	CoUninitialize();
	bRet = true;
	return bRet;
}