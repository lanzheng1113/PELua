// PELuaTool.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PELuaTool.h"
#include "util/StringEx.h"
#include "util/File.h"
#include "util/Path.h"
#include "util/Logger.h"
#include <string>
using namespace std;

#ifndef WS2S
#define WS2S(w) String::fromStdWString(w)
#endif // WS2S

#ifndef S2WS
#define S2WS(a) String(a).toStdWString(a)
#endif

bool RunLuaScript(const std::string& strScript);

//
// �����˳�ֵ��
// - 0 �ɹ�
// - 1 δָ����Ҫִ�еĽű�
// - 2 δ֪����
// - 3 �������ļ�ʧ��
// - 4 ִ�нű���������
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
		wstring wstrCmdLine = lpCmdLine;
		string luaFile = WS2S(wstrCmdLine);
		if (File::exists(luaFile))
		{
			string luaFileName = string("LUAExec_") + File(luaFile).getFileName() + ".log";
			Logger::getInstance()->setLogFileName(luaFileName);
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
				LOG_ERROR("�������ļ�%sʧ��", luaFile.c_str());
				return 3;
			}
		}
	}
	else
	{
		return 1;
	}

    return 2;
}

