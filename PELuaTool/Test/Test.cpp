// Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>

#define Text_Environment TEXT("Environment")


int EnableAppleTrackRightClick(BOOL bEnable)
{
	HANDLE hAppleTrackPad = CreateFile(_T("\\\\.\\AppleTrackpad"), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hAppleTrackPad)
	{
		printf("���豸\\\\.\\AppleTrackpadʧ���ˣ���ȷ���Ƿ�û�а�װ���������=%d\n", GetLastError());
		return 1;
	}
	if (!bEnable)
	{
		DWORD disableValue = 0x28;
		DWORD byteReturned = 0;
		BOOL bDisable = DeviceIoControl(hAppleTrackPad, 0xF2004, &disableValue, sizeof(disableValue), NULL, NULL, &byteReturned, NULL);
		if (bDisable)
		{
			printf("�ɹ����ͽ���AppleTrackPad�Ҽ�������\n");
			return 0;
		}
		else
		{
			printf("���ͽ���AppleTrackPad�Ҽ�������ʧ����(%d)\n",GetLastError());
		}
	}
	else
	{
		DWORD enableValue = 0x68;
		DWORD byteReturned = 0;
		if (DeviceIoControl(hAppleTrackPad, 0xF2004, &enableValue, sizeof(enableValue), NULL, NULL, &byteReturned, NULL))
		{
			printf("�ɹ���������AppleTrackPad�Ҽ�������\n");
			return 0;
		}
		else
		{
			printf("��������AppleTrackPad�Ҽ�������ʧ����(%d)\n", GetLastError());
		}
	}
	
	return 1;
}


int main()
{
	if (0 == EnableAppleTrackRightClick(TRUE))
	{
		printf("�Ѿ������Ҽ�����������һ��mac���Ҽ���\n");
		system("pause");
	}
	
	if (0 == EnableAppleTrackRightClick(FALSE))
	{
		printf("�Ѿ������Ҽ���������ȷ��һ��Mac���Ҽ��Ƿ��Ѿ����á�\n");
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
			printf("�㲥���������仯ʱ�����˳�ʱ����\n");
		}
		else
		{
			printf("�㲥���������仯ʱ�����˴���%d\n", lastErr);
		}
	}
	else
	{
		printf("�ɹ��㲥�������������仯����Ϣ��\n");
	}
	*/
	system("pause");
    return 0;
}

