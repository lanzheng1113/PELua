#pragma once

PTSTR UStrTrim(PTSTR ptstr);
DWORD FindProcess(PCTSTR ptzCmd);
HRESULT Exec(wstring ptzCmd);
void GetShell(bool DeleteSettingKey);
VOID InstallHookKeyBoard(BOOL Install);
int PEMain(const wstring& szLuaFile);

//LUA��������
HRESULT PE_Text(LPCTSTR pszText, COLORREF color, LPCTSTR font, int FontSize, long left, long top, long right, long bottom);
void PE_INIT(BOOL noTray, BOOL noKeyboardHook);
void PE_SHELL(LPCTSTR lpShell);
BOOL PE_SetEnvironment(wstring ptzEnvName, wstring ptzEnvValue, BOOL bSystem);
BOOL PE_SetLogo(LPCTSTR lpImgPath, COLORREF bgColor);
BOOL PE_Wall(LPCTSTR ptzCmd);
VOID InitEnvironmentVariable();