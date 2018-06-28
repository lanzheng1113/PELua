// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

#define Text_Environment TEXT("Environment")

int main()
{
	DWORD_PTR dwResult = 0;
	//::SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)Text_Environment);

	if (0 == SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)Text_Environment, SMTO_ABORTIFHUNG, 5000, &dwResult))
	{
		DWORD lastErr = GetLastError();
		if (lastErr == ERROR_TIMEOUT)
		{
			printf("广播环境变量变化时发生了超时错误\n");
		}
		else
		{
			printf("广播环境变量变化时发生了错误%d\n", lastErr);
		}
	}
	else
	{
		printf("成功广播环境变量发生变化的消息。\n");
	}
	system("pause");
    return 0;
}

