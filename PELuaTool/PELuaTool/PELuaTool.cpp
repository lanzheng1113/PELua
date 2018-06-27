// PELuaTool.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PELuaTool.h"
#include "util/StringEx.h"
#include "util/File.h"
#include "util/Path.h"
#include "util/Logger.h"
#include <string>
#include <shellapi.h>
using namespace std;
#include "pe.h"


#ifndef WS2S
#define WS2S(w) String::fromStdWString(w)
#endif // WS2S

#ifndef S2WS
#define S2WS(a) String(a).toStdWString(a)
#endif

bool RunLuaScript(const std::string& strScript);

//
// 进程退出值：
// - 0 成功
// - 1 未指定需要执行的脚本
// - 2 未知错误
// - 3 打开输入文件失败
// - 4 执行脚本发生错误。
//
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (lpCmdLine)
	{
		LPWSTR *szArgList;
		int argCount;
		szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
		if (szArgList == NULL)
		{
			//parse arg failed.
			return 10;
		}
		if (argCount == 3 && 0 == _wcsicmp(szArgList[1],L"MAIN"))
		{
			string luaFile = WS2S(wstring(szArgList[2]));
			if (File::exists(luaFile))
			{
				PEMain(wstring(szArgList[2]));
			}
		}
		else if (argCount == 2)
		{
			string luaFile = WS2S(wstring(szArgList[1]));
			if (File::exists(luaFile))
			{
				FileReader f(luaFile);
				if (f.open())
				{
					string str = f.read();
					bool bRet = RunLuaScript(str);
					f.close();
					if (bRet)
					{
						return 0;
					}
					else
						return 4;
				}
				else
				{
					LOG_ERROR("打开输入文件%s失败", luaFile.c_str());
					return 3;
				}
			}
		}
		else
		{
			;
		}
		LocalFree(szArgList);
	}
	else
	{
		return 1;
	}

    return 2;
}

