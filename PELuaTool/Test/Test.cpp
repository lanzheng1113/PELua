// Test.cpp : �������̨Ӧ�ó������ڵ㡣
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
	system("pause");
    return 0;
}

