#include <windows.h>
#include <cstdio>

static int EnableAppleTrackRightClick(BOOL bEnable)
{
	HANDLE hAppleTrackPad = CreateFile(L"\\\\.\\AppleTrackpad", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (INVALID_HANDLE_VALUE == hAppleTrackPad)
	{
		printf("打开设备\\\\.\\AppleTrackpad失败了，请确认是否没有安装。错误代码=%d", GetLastError());
		return 1;
	}
	if (!bEnable)
	{
		DWORD disableValue = 0x28;
		DWORD byteReturned = 0;
		BOOL bDisable = DeviceIoControl(hAppleTrackPad, 0xF2004, &disableValue, sizeof(disableValue), NULL, NULL, &byteReturned, NULL);
		if (bDisable)
		{
			printf("成功发送禁用AppleTrackPad右键的请求");
			return 0;
		}
		else
		{
			printf("发送禁用AppleTrackPad右键的请求失败了(%d)", GetLastError());
		}
	}
	else
	{
		DWORD enableValue = 0x68;
		DWORD byteReturned = 0;
		if (DeviceIoControl(hAppleTrackPad, 0xF2004, &enableValue, sizeof(enableValue), NULL, NULL, &byteReturned, NULL))
		{
			printf("成功发送启用AppleTrackPad右键的请求");
			return 0;
		}
		else
		{
			printf("发送启用AppleTrackPad右键的请求失败了(%d)", GetLastError());
		}
	}
	return 1;
}

int main(int argc, char**argv)
{
	return EnableAppleTrackRightClick(TRUE);
}