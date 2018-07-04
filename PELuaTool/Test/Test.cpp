// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

#define Text_Environment TEXT("Environment")


int EnableAppleTrackRightClick(BOOL bEnable)
{
	HANDLE hAppleTrackPad = CreateFile(_T("\\\\.\\AppleTrackpad"), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hAppleTrackPad)
	{
		printf("打开设备\\\\.\\AppleTrackpad失败了，请确认是否没有安装。错误代码=%d\n", GetLastError());
		return 1;
	}
	if (!bEnable)
	{
		DWORD disableValue = 0x28;
		DWORD byteReturned = 0;
		BOOL bDisable = DeviceIoControl(hAppleTrackPad, 0xF2004, &disableValue, sizeof(disableValue), NULL, NULL, &byteReturned, NULL);
		if (bDisable)
		{
			printf("成功发送禁用AppleTrackPad右键的请求\n");
			return 0;
		}
		else
		{
			printf("发送禁用AppleTrackPad右键的请求失败了(%d)\n",GetLastError());
		}
	}
	else
	{
		DWORD enableValue = 0x68;
		DWORD byteReturned = 0;
		if (DeviceIoControl(hAppleTrackPad, 0xF2004, &enableValue, sizeof(enableValue), NULL, NULL, &byteReturned, NULL))
		{
			printf("成功发送启用AppleTrackPad右键的请求\n");
			return 0;
		}
		else
		{
			printf("发送启用AppleTrackPad右键的请求失败了(%d)\n", GetLastError());
		}
	}
	
	return 1;
}


int main()
{
	if (0 == EnableAppleTrackRightClick(TRUE))
	{
		printf("已经启用右键。现在请试一下mac的右键。\n");
		system("pause");
	}
	
	if (0 == EnableAppleTrackRightClick(FALSE))
	{
		printf("已经禁用右键。现在请确认一下Mac的右键是否已经禁用。\n");
		system("pause");
	}
	/*
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
	*/
	system("pause");
    return 0;
}

