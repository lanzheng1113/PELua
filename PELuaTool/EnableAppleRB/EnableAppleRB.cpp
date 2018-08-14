#include <windows.h>
#include <cstdio>

static int EnableAppleTrackRightClick(BOOL bEnable)
{
	HANDLE hAppleTrackPad = CreateFile(L"\\\\.\\AppleTrackpad", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hAppleTrackPad)
	{
		printf("���豸\\\\.\\AppleTrackpadʧ���ˣ���ȷ���Ƿ�û�а�װ���������=%d", GetLastError());
		return 1;
	}
	if (!bEnable)
	{
		DWORD disableValue = 0x28;
		DWORD byteReturned = 0;
		BOOL bDisable = DeviceIoControl(hAppleTrackPad, 0xF2004, &disableValue, sizeof(disableValue), NULL, NULL, &byteReturned, NULL);
		if (bDisable)
		{
			printf("�ɹ����ͽ���AppleTrackPad�Ҽ�������");
			return 0;
		}
		else
		{
			printf("���ͽ���AppleTrackPad�Ҽ�������ʧ����(%d)", GetLastError());
		}
	}
	else
	{
		DWORD enableValue = 0x68;
		DWORD byteReturned = 0;
		if (DeviceIoControl(hAppleTrackPad, 0xF2004, &enableValue, sizeof(enableValue), NULL, NULL, &byteReturned, NULL))
		{
			printf("�ɹ���������AppleTrackPad�Ҽ�������");
			return 0;
		}
		else
		{
			printf("��������AppleTrackPad�Ҽ�������ʧ����(%d)", GetLastError());
		}
	}
	return 1;
}

int main(int argc, char**argv)
{
	return EnableAppleTrackRightClick(TRUE);
}