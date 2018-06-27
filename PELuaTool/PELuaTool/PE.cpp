#include "stdafx.h"
#include <string>
#include <Shlwapi.h>
#include <TlHelp32.h>
#include <shellapi.h>
#include "Resource.h"
#include "util/Path.h"
#include "util/StringEx.h"
#include "util/Logger.h"
#include "util/File.h"
using std::wstring;
using std::string;
#include "pe.h"

#ifndef WS2S
#define WS2S(w) String::fromStdWString(w)
#endif // WS2S

#ifndef S2WS
#define S2WS(a) String(a).toStdWString()
#endif

// 定义
#define MAX_NAME		80
#define PESHELL			TEXT("Shell")
#define PELOGON			TEXT("SOFTWARE\\PELOGON")
#define FindShellMsg	(WM_USER + 01)
#define CM_nTaskIcoMsg	(WM_USER + 02)
#define MAX_STR			1024

// 全局变量
HINSTANCE		g_hInst = NULL;
HWND			MainWindowHwnd = NULL;
TCHAR			MyShell[MAX_PATH];
BOOL			TrayIcon = FALSE;
HHOOK			g_hHook = NULL;
HANDLE			CurrentProcess = NULL;
TCHAR			MenuTipText[MAX_NAME];
HMENU			hSubMenu;

NOTIFYICONDATA	FpNotify;
UINT ToalMenu = 0;
DWORD			MenuFirstID = 0;
TCHAR			HotkCmd[MAX_PATH];
TCHAR			MenuCommand[128][MAX_PATH] = { 0 };
TCHAR			HotKeyCommand[8][MAX_PATH] = { 0 };

void GetKeyModAndKeyCode(PTSTR ptzHotk, UINT &Key_Mod, UINT &Hotkey)
{
	if (StrStrI(ptzHotk, TEXT("ALT")))
	{
		Key_Mod = Key_Mod | MOD_ALT;
	}
	if (StrStrI(ptzHotk, TEXT("CTRL")))
	{
		Key_Mod = Key_Mod | MOD_CONTROL;
	}
	if (StrStrI(ptzHotk, TEXT("SHIFT")))
	{
		Key_Mod = Key_Mod | MOD_SHIFT;
	}
	if (StrStrI(ptzHotk, TEXT("WIN")))
	{
		Key_Mod = Key_Mod | MOD_WIN;
	}
	PTSTR pKeyCode = StrChrW(ptzHotk, '#');
	if (pKeyCode)
	{
		INT i = 0;
		StrToIntExW(++pKeyCode, STIF_SUPPORT_HEX, &i);
		Hotkey = i;
	}
}

void RegMenuHotk(void)
{
	static BOOL bReg = FALSE;
	if (!bReg)
	{
		RegisterHotKey(MainWindowHwnd, 0, MOD_ALT | MOD_CONTROL, 'M');
		UINT Key_Mod = 0;
		UINT Hotkey = 0;
		TCHAR HotkCode[MAX_NAME] = L"Ctrl + Alt + #0x41, PECMD.EXE DISP W800H600B16F75";
		PTSTR ptzHotk = UStrTrim(HotkCode);
		PTSTR ptzCmd = StrChrW(ptzHotk, ',');
		if (ptzCmd)
		{
			*ptzCmd++ = 0;
		}
		if (ptzCmd && ptzHotk)
		{
			wcscpy(HotkCmd, ptzCmd);//
			GetKeyModAndKeyCode(ptzHotk, Key_Mod, Hotkey);
			RegisterHotKey(MainWindowHwnd, 1, Key_Mod, Hotkey);
		}
		bReg = TRUE;
	}
}


void SetMenuText(HMENU hsMenu)
{
	TCHAR MenuText[MAX_STR];
	//static HBITMAP hBitMap = NULL;
	HMENU sMenu = NULL;
	MENUITEMINFO mii;

	//if (!hBitMap)
	//{
	//	hBitMap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
	//}

	int MenuItemCount = GetMenuItemCount(hsMenu);
	int MenuItemID;
	for (int i = 0; i<MenuItemCount; i++)
	{
		MenuItemID = GetMenuItemID(hsMenu, i);
		/* 指定条目的菜单ID。
		如条目属于一个弹出式菜单，就返回-1；
		如指定的条目属于一个分隔符（比如一条分隔线）则返回0 */
		switch (MenuItemID)
		{
		case -1:
			sMenu = GetSubMenu(hsMenu, i);
			if (sMenu)
			{
				SetMenuText(sMenu);
			}
			break;
		case 0:
			break;
		default:
			if (!MenuFirstID)
			{
				MenuFirstID = MenuItemID;
			}
			if ((MenuItemID>0) && (MenuItemID != ID_HIDE_ICON) && (ToalMenu <128))
			{
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask = MIIM_TYPE;
				mii.fType = MFT_STRING;
				mii.cch = MAX_STR;
				mii.dwTypeData = MenuText;
				GetMenuItemInfo(hsMenu, MenuItemID, FALSE, &mii);
				PTSTR ptzCmd = StrChrW(MenuText, ',');
				if (ptzCmd)
				{
					*ptzCmd++ = 0;
					ptzCmd = UStrTrim(ptzCmd);
					TCHAR tzCmd[MAX_PATH];
					ExpandEnvironmentStrings(ptzCmd, tzCmd, MAX_PATH);
					wcscpy(MenuCommand[ToalMenu], tzCmd);//
					ToalMenu++;
				}
				PTSTR pMenuText = UStrTrim(MenuText);
				ModifyMenu(hsMenu, MenuItemID, MF_STRING, MenuItemID, pMenuText);
			}
		}
	}
}


HMENU MyPopupMenu(BOOL Created)
{
	static HMENU hMenu;
	if (Created)
	{
		if (!hMenu)
		{
			hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		}

		if ((!hSubMenu) && (hMenu))
		{
			RegMenuHotk();
			ToalMenu = 0;
			hSubMenu = GetSubMenu(hMenu, 0);
			if (hSubMenu)
			{
				SetMenuText(hSubMenu);
			}
		}
	}
	else
	{
		if (hSubMenu)
		{
			if (DestroyMenu(hSubMenu))
			{
				hSubMenu = NULL;
				if (hMenu)
				{
					if (DestroyMenu(hMenu))
					{
						hMenu = NULL;
					}
				}
			}
		}
	}
	return hSubMenu;
}

VOID InsTrayIcon(BOOL Install)
{
	if (Install)
	{
		for (UINT i = 1; i < 10; i++)
		{
			//if (FindProc(EXPLORER))
			{
				SleepEx(100, 1);
				if (!hSubMenu)
				{
					hSubMenu = MyPopupMenu(TRUE);
					if (MainWindowHwnd && hSubMenu)
					{
						if (!FpNotify.hWnd)
						{
							FpNotify.cbSize = sizeof(NOTIFYICONDATA);
							FpNotify.hWnd = MainWindowHwnd;
							FpNotify.uID = 0;
							FpNotify.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
							//HICON hIcon = LoadIcon(g_hInst,(PTSTR)IDI_ICON1);
							HICON hIcon = LoadIcon(g_hInst, (LPCWSTR)IDI_ICON1);
							if (!hIcon)
							{
								//hIcon = LoadIcon(g_hInst,(PTSTR)IDI_Main);
								hIcon = LoadIcon(g_hInst, (LPCWSTR)IDI_Main);
							}
							FpNotify.hIcon = hIcon;
							FpNotify.uCallbackMessage = CM_nTaskIcoMsg;
							StrCpyNW(FpNotify.szTip, MenuTipText, 64);
						}
					}
				}

				if (FpNotify.hWnd)
				{
					if (Shell_NotifyIcon(NIM_ADD, &FpNotify))
					{
						break;
					}
				}
			}
			SleepEx(200, 1);
		}
	}
	else
	{
		if (hSubMenu)
		{
			Shell_NotifyIcon(NIM_DELETE, &FpNotify);
			hSubMenu = MyPopupMenu(FALSE);
		}
	}
	SetProcessWorkingSetSize(CurrentProcess, -1, -1);
}

//////////////////////////////////////////////////////////////////////////////////////////
void RegHotKey(void)
{
	UINT KeyMod[8] = { 0 };
	UINT HotKey[8] = { 0 };
	PTSTR ptzKeyMod;
	PTSTR ptzHotKey;
	DWORD dwType;
	DWORD dwSize;
	TCHAR ptzKeyName[MAX_NAME];

	for (UINT i = 0; i < 8; i++)
	{
		dwType = REG_SZ;
		dwSize = MAX_STR;

		wsprintfW(ptzKeyName, TEXT("HotKey%d"), i + 1);
		SHGetValue(HKEY_LOCAL_MACHINE, PELOGON, ptzKeyName, &dwType, &HotKeyCommand[i][0], &dwSize);
		if (HotKeyCommand[i][0])
		{
			ptzKeyMod = StrChrW(HotKeyCommand[i], ',');
			if (ptzKeyMod)
			{
				*ptzKeyMod++ = 0;
				INT ti = 0;
				StrToIntExW(ptzKeyMod, STIF_SUPPORT_HEX, &ti);
				KeyMod[i] = ti;
				ptzHotKey = StrChrW(ptzKeyMod, ',');
				if (ptzHotKey)
				{
					*ptzHotKey++ = 0;
					INT ti = 0;
					StrToIntExW(ptzHotKey, STIF_SUPPORT_HEX, &ti);
					HotKey[i] = ti;
				}
			}

			if (HotKey[i] > 0)
			{
				BOOL rbReg = RegisterHotKey(MainWindowHwnd, i + 2, KeyMod[i], HotKey[i]);

				wsprintfW(ptzKeyName, TEXT("RegHotKey%d"), i + 1);
				TCHAR KeyName[MAX_STR];
				wsprintfW(KeyName,
					TEXT("Result=%d; KeyMod=%d, HotKey=%d; CMD=%s"),
					rbReg,
					KeyMod[i],
					HotKey[i],
					HotKeyCommand[i]);

				SHSetValue(HKEY_LOCAL_MACHINE,
					PELOGON,
					ptzKeyName,
					REG_SZ,
					KeyName,
					_tcslen(KeyName) * sizeof(TCHAR));

			}

		}
	}

	SHSetValue(HKEY_LOCAL_MACHINE,
		PELOGON,
		TEXT("MainWindowHwnd"),
		REG_DWORD,
		&MainWindowHwnd,
		sizeof(MainWindowHwnd));

}

void PopupLogonMenu(HWND hWND)
{
	POINT p;
	GetCursorPos(&p);
	SetForegroundWindow(MainWindowHwnd);
	TrackPopupMenu(hSubMenu, TPM_LEFTBUTTON, p.x, p.y, NULL, hWND, NULL);
	PostMessage(hWND, WM_NULL, 0, 0);
	SetProcessWorkingSetSize(CurrentProcess, -1, -1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY://0x0002
		for (UINT i = 0; i < 10; i++)
		{
			UnregisterHotKey(MainWindowHwnd, i);
		}
		PostQuitMessage(0);
		break;

	case WM_COMMAND://0x0111
		if (wParam == ID_HIDE_ICON)
		{
			InsTrayIcon(FALSE);
		}
		else
		{
			Exec(MenuCommand[wParam - MenuFirstID]);
		}
		break;

	case WM_HOTKEY://0x0312
		switch LOWORD(wParam)
		{
		case 0:
			InsTrayIcon(!hSubMenu);
			break;
		case 1:
			Exec(HotkCmd);
			break;
		default:
			Exec(HotKeyCommand[wParam - 2]);
		}
		break;

	case FindShellMsg://0x0401
		switch (wParam)
		{
		case 0:
			RegHotKey();
			GetShell(true);
			break;
		case 1:
			InstallHookKeyBoard(TRUE);
			break;
		case 2:
			LOG_INFO("收到需要安装托盘图标的消息。");
			InsTrayIcon(TRUE);
			break;
		}
		break;

	case CM_nTaskIcoMsg: //0x0402
		switch (lParam)
		{
		case WM_LBUTTONUP:
			PopupLogonMenu(hWnd);
			break;
		case WM_RBUTTONUP:
			PopupLogonMenu(hWnd);
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define szWindowName TEXT("PELogon(Created By Lxl1638)")
#define szWindowClass TEXT("PELogon CallBack Window")
//////////////////////////////////////////////////////////////////////////////////////////
HWND CreateMainWindow(HINSTANCE hInstance/*, int nCmdShow*/)
{
	HWND hResult = NULL;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	if (1)
	{
		if (RegisterClassEx(&wcex))
		{
			hResult = CreateWindow(szWindowClass,
				szWindowName,
				WS_DISABLED | WS_MINIMIZE,
				CW_USEDEFAULT, 0,
				CW_USEDEFAULT, 0,
				NULL, NULL, hInstance, NULL);
		}
		//TCHAR ChineseMenuTip[MAX_NAME];
		//UINT16ToUStr(ChineseMenuTip, AppTrayText, _NumOf(AppTrayText));
		//UStrCat(MenuTipText, (PTSTR)((DWORD)ChineseMenuTip*(!NotChineseLangID)));
	}
	return hResult;
}


//
// 取(或者删除)HKEY_LOCAL_MACHINE\SOFTWARE\PELOGON下Shell、TrayIcon和InitProc的值
//
void GetShell(bool DeleteSettingKey)
{
	if (DeleteSettingKey)
	{
		SHSetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("SysShell"), REG_SZ, MyShell, _tcslen(MyShell) * sizeof(TCHAR));
		SHDeleteValue(HKEY_LOCAL_MACHINE, PELOGON, PESHELL);
	}
	else
	{
		BOOL InitProc = FALSE;
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		SHGetValue(HKEY_LOCAL_MACHINE, PELOGON, PESHELL, &dwType, &MyShell[0], &dwSize);
		dwType = REG_DWORD;
		SHGetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("TrayIcon"), &dwType, &TrayIcon, &dwSize);
		SHGetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("InitProc"), &dwType, &InitProc, &dwSize);
		if ((!g_hHook) && (InitProc) && (MainWindowHwnd))
		{
			LOG_INFO("已获取Shell:[%s]", String::fromStdWString(MyShell).c_str());
			SendMessage(MainWindowHwnd, FindShellMsg, 1, 0);
		}
	}
}

//从文件名中分离出文件路径
PTSTR UPathSplitName(PTSTR ptzPath)
{
	PTSTR p = StrRChrW(ptzPath, NULL, L'\\');
	if (p)
	{
		p++;
		return p;
	}
	else
	{
		return ptzPath;
	}
}

//
// Find process
// 指定进程名，参数为空时返回父进程ID
//
HRESULT FindProc(PCTSTR ptzCmd)
{
	HRESULT hResult = NULL;
	DWORD CurrentProcessId = GetCurrentProcessId();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		for (BOOL b = Process32First(hSnap, &pe); b; b = Process32Next(hSnap, &pe))
		{
			if (ptzCmd) //取进程ID
			{
				if (_wcsicmp(pe.szExeFile, ptzCmd) == 0)
				{
					hResult = pe.th32ProcessID;
					break;
				}
			}
			else //取父进程ID
			{
				if (pe.th32ProcessID == CurrentProcessId)
				{
					hResult = pe.th32ParentProcessID;
					break;
				}
			}
		}
		CloseHandle(hSnap);
	}
	return hResult;
}


//
//删字符串左空函数
//
PTSTR UStrLeftTrim(PTSTR ptstr)
{
	if (ptstr)
	{
		for (; (*ptstr == ' ') || (*ptstr == '\t'); ptstr++);
	}
	return ptstr;
}

//
//删字符串右空函数
//
PTSTR UStrRightTrim(PTSTR ptstr)
{
	PTSTR tmp = ptstr;
	if (ptstr)
	{
		size_t i = wcslen(ptstr) - 1;
		ptstr = ptstr + i;
		while ((*ptstr == ' ') || (*ptstr == '\t'))
		{
			*ptstr = 0;
			ptstr--;
		}
	}
	return tmp;
}

//
//删字符串前后空函数
//
PTSTR UStrTrim(PTSTR ptstr)
{
	return UStrRightTrim(UStrLeftTrim(ptstr));
}


DWORD WINAPI MainThread(_In_ LPVOID lpParameter)
{
	LOG_INFO("PELuaTool进入工作线程");
	// 如果没有设置外壳程序，则一直等待。
	while (!MyShell[0])
	{
		GetShell(false);
		Sleep(50);
	}
	LOG_INFO("检测到外壳程序");
	//
	// 如果设置了外壳程序，那么尝试调用外壳程序
	//
	PTSTR ShellName = UPathSplitName(MyShell);
	if (MainWindowHwnd && !FindProc(ShellName))
	{
		TCHAR EMenuTip[MAX_NAME] = TEXT("Shell process not run!");
		wcscat(MenuTipText, EMenuTip);
		UStrTrim(MenuTipText);
		SendMessage(MainWindowHwnd, FindShellMsg, 0, 0);
	}

	LOG_INFO("尝试调用外壳程序。");
	while (MyShell[0] && (!FindProc(ShellName)))
	{
		Exec(MyShell);
		Sleep(200);
	}
	// 启动SHELL之后，把托盘启动起来。
	LOG_INFO("外壳程序已启动");
	SendMessage(MainWindowHwnd, FindShellMsg, 2, 0);
	return S_OK;
}

// Execute command
HRESULT Exec(wstring ptzCmd)
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = TEXT("WinSta0\\Default");
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_NORMAL;
	WCHAR pcmd[1024] = { 0 };
	wcscpy(pcmd, ptzCmd.c_str());
	BOOL bResult = CreateProcess(NULL, pcmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (bResult)
	{
		SetProcessWorkingSetSize(CurrentProcess, -1, -1);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return bResult ? S_OK : S_FALSE;
}

//
// Ctrl+Alt+Del 调用任务管理器
//
TCHAR TASKMANAGER[] = TEXT("TaskMgr.exe");
LRESULT CALLBACK InitProc(INT iCode, WPARAM wParam, LPARAM lParam)
{
	if ((iCode == HC_ACTION) && (wParam == WM_KEYDOWN) && (((LPKBDLLHOOKSTRUCT)lParam)->vkCode == VK_DELETE))
	{
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000))
		{
			Exec(TASKMANAGER);
			return TRUE;
		}
	}
	return CallNextHookEx(g_hHook, iCode, wParam, lParam);
}

//
// 安装处理Ctrl+Alt+Del的键盘钩子
//
VOID InstallHookKeyBoard(BOOL Install)
{
	if (Install)
	{
		if (!g_hHook)
		{
			g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, InitProc, g_hInst, 0);
		}
	}
	else
	{
		if (g_hHook)
		{
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}
}


int PEMain(const wstring& szLuaFile)
{
	// Switch desktop
	Logger::getInstance()->setLogFileName(Path::getApplicationDirPath() + "PELuaTool.log");
	HDESK hDesk = OpenDesktop(TEXT("Default"), 0, TRUE, GENERIC_ALL);
	if (hDesk)
	{
		LOG_INFO("成功打开Default桌面");
		if (SetThreadDesktop(hDesk))
		{
			LOG_INFO("成功设置当前线程的桌面");
			SwitchDesktop(hDesk);
		}
		CloseDesktop(hDesk);
	}
	//
	// 创建一个子进程执行LUA脚本
	//
	if (!szLuaFile.empty())
	{
		TCHAR szAppPath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szAppPath, MAX_PATH);
		wstring wstrCmd = szAppPath;
		wstrCmd += L" ";
		wstrCmd += szLuaFile;
		//TCHAR szMsg[1024] = { 0 };
		//wsprintfW(szMsg, L"MAIN执行[%s]", wstrCmd.c_str());
		//MessageBox(NULL, szMsg, NULL, MB_OK);
		Exec(wstrCmd);
	}
	//
	// 创建主窗口，这个进程不退出。
	// TODO：创建托盘菜单等。
	//
	MainWindowHwnd = CreateMainWindow(g_hInst);
	DWORD dwThread;
	HANDLE hThread = CreateThread(NULL, 0, MainThread, NULL, 0, &dwThread);
	CloseHandle(hThread);
	SetProcessWorkingSetSize(CurrentProcess, -1, -1);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// Exit
	InstallHookKeyBoard(FALSE);
	return (HRESULT)msg.wParam;
}


static std::wstring FormatErrorCode(DWORD errCode)
{
	wstring strRet;
	LPVOID lpMsgBuf = NULL;
	DWORD dw = errCode;
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0, NULL);
	if (lpMsgBuf)
	{
		strRet = (LPWSTR)lpMsgBuf;
	}
	LocalFree(lpMsgBuf);
	return strRet;
}


void PE_INIT()
{
	BOOL Icon = TRUE;
	LSTATUS ErrorCode = SHSetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("TrayIcon"), REG_DWORD, &Icon, sizeof(Icon));
	if (ERROR_SUCCESS == ErrorCode)
		LOG_INFO("PE INIT 成功");
	else
	{
		LOG_INFO("PE INIT 失败[%s]", WS2S(FormatErrorCode(ErrorCode)).c_str());
	}
}

void PE_SHELL(LPCTSTR lpShell)
{
	SHSetValue(HKEY_LOCAL_MACHINE, PELOGON, PESHELL, REG_SZ, lpShell, wcslen(lpShell) * sizeof(TCHAR));
}

#define Text_Environment TEXT("Environment")
#define Session_Manager TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager")
BOOL PE_SetEnvironment(wstring ptzEnvName, wstring ptzEnvValue, BOOL bSystem)
{
	if (ptzEnvName.empty() || ptzEnvValue.empty())
	{
		DWORD_PTR dwResult = 0;
		SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)Text_Environment, SMTO_ABORTIFHUNG, 5000, &dwResult);
		return TRUE;
	}

	BOOL bResult = SetEnvironmentVariable(ptzEnvName.c_str(), ptzEnvValue.c_str()) ? S_OK : S_FALSE;
	
	if (bSystem)
	{
		wstring strRegPath(Session_Manager);
		strRegPath += TEXT("\\");
		strRegPath += Text_Environment;
		BOOL isContentMacro = (ptzEnvValue.find(L'%') != wstring::npos);
		SHSetValue(HKEY_LOCAL_MACHINE, strRegPath.c_str(), ptzEnvName.c_str(), isContentMacro ? REG_EXPAND_SZ : REG_SZ, ptzEnvValue.c_str(), ptzEnvValue.length()*sizeof(TCHAR));
		SHSetValue(HKEY_CURRENT_USER, Text_Environment, ptzEnvName.c_str(), isContentMacro ? REG_EXPAND_SZ : REG_SZ, ptzEnvValue.c_str(), ptzEnvValue.length()*sizeof(TCHAR));
	}

	if (bSystem)
	{
		DWORD_PTR dwResult = 0;
		SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)Text_Environment, SMTO_ABORTIFHUNG, 5000, &dwResult);
	}
	return bResult;
}

INT UWStrToInt(PCWSTR pwzStr)
{
	INT i = 0;
	StrToIntExW(pwzStr, STIF_SUPPORT_HEX, &i);
	return i;
}

//
// 调整（获取）权限
//
HRESULT AdjustPrivilege(PCTSTR ptzName)
{
	HANDLE hToken;
	if (!OpenProcessToken(CurrentProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}
	TOKEN_PRIVILEGES tPriv;
	LookupPrivilegeValue(NULL, ptzName, &tPriv.Privileges[0].Luid);
	tPriv.PrivilegeCount = 1;
	tPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	return AdjustTokenPrivileges(hToken, FALSE, &tPriv, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
}

/**
 * \brief 关机/重启
 */
HRESULT Boot(BOOL bReboot)
{
	DWORD_PTR dwResult;
	SendMessageTimeout(HWND_BROADCAST, WM_QUERYENDSESSION, 0, 0, 0, 2000, &dwResult);
	SendMessageTimeout(HWND_BROADCAST, WM_ENDSESSION, 0, 0, 0, 2000, &dwResult);

	//SendMessageTimeout(HWND_BROADCAST, WM_CLOSE, 0, 0, 0, 2000, &dwResult);

	SendMessageTimeout(HWND_BROADCAST, WM_DESTROY, 0, 0, 0, 2000, &dwResult);

	// Get function address
	typedef DWORD(NTAPI *PNtShutdownSystem)(DWORD dwAction);
	PNtShutdownSystem NtShutdownSystem = (PNtShutdownSystem)GetProcAddress(GetModuleHandle(TEXT("NTDLL")), "NtShutdownSystem");
	if (!NtShutdownSystem)
	{
		return E_FAIL;
	}
	// Shutdown
	AdjustPrivilege(SE_SHUTDOWN_NAME);
	return NtShutdownSystem(bReboot ? 1 : 2);
}


//=====================================================
//================绘制LOGO、TEXT、桌面=================
//=====================================================

struct GPSTART { UINT uVersion; PROC pCallback; BOOL bSuppressThread; BOOL bSuppressExternal; };
typedef HRESULT(WINAPI* PGdiplusStartup)(HANDLE* phToken, const GPSTART* gpIn, PVOID pvOut);
typedef HRESULT(WINAPI* PGdiplusShutdown)(HANDLE hToken);
typedef HRESULT(WINAPI* PGdipLoadImageFromFile)(const PWSTR pwzPath, HANDLE* phImage);
typedef HRESULT(WINAPI* PGdipDisposeImage)(HANDLE hImage);
typedef HRESULT(WINAPI* PGdipCreateFromHDC)(HDC hDC, HANDLE* phGraph);
typedef HRESULT(WINAPI* PGdipDrawImageRectI)(HANDLE hGraph, HANDLE hImage, INT iLeft, INT iTop, INT iWidth, INT iHeight);
PGdiplusStartup pGdiplusStartup = NULL;
PGdiplusShutdown pGdiplusShutdown = NULL;
PGdipDisposeImage pGdipDisposeImage = NULL;
PGdipCreateFromHDC pGdipCreateFromHDC = NULL;
PGdipDrawImageRectI pGdipDrawImageRectI = NULL;
PGdipLoadImageFromFile pGdipLoadImageFromFile = NULL;

const struct
{
	DLGTEMPLATE dtDlg;
	WCHAR  wzMenu[1];
	WCHAR wzClass[1];
	WCHAR wzCaption[6];
	WORD wEnd[6];
} c_dtLogo =
{
	{ WS_POPUP | WS_VISIBLE | DS_CENTER, WS_EX_LAYERED | WS_EX_TOOLWINDOW, 0, 0, 0, 400, 300 },
	0,
	0,
	L"PECMD"
};

COLORREF BackgroundColor = 0;
bool ExistLogoImage = false;
bool tzColorSet = false;
HWND s_hLogo = NULL;
HANDLE s_hImage = NULL;
RECT *tzRECT = NULL;
RECT s_rtText = { 4, 4, 640, 64 };
RECT s_rtScreen = { 0,0,0,0 };
COLORREF s_crText = 0xFFFFFF;
HFONT hfont = NULL;					// TEXT指令设置
TCHAR s_tzText[MAX_NAME] = { 0 }; // TEXT指令设置
BOOL tzDrawTextDone = TRUE;
HWND PassButton;
BOOL PassWordEditEnter;
UINT fontsize = 14;


HRESULT OutPutText(HDC hDC)
{
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, s_crText);
	(hfont ? SelectObject(hDC, hfont) : SelectObject(hDC, GetStockObject(DEFAULT_GUI_FONT)));
	HRESULT hResult = DrawText(hDC, s_tzText, -1, &s_rtText, DT_LEFT | DT_WORDBREAK);
	if (!s_hLogo)
	{
		tzDrawTextDone = TRUE;
	}
	return hResult ? S_OK : S_FALSE;
}

//
// Logo 窗口
//
INT_PTR CALLBACK LogoProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DWORD timestart = 0;
	switch (uMsg)
	{
	case WM_DESTROY: //0x0002
		LOG_INFO("LOGO窗口销毁");
		PostQuitMessage(0);
		break;

	case WM_PAINT:	 //0x000F
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		if (!ExistLogoImage)
		{
			// 背景图片不存在时，用画刷填充背景色。如果背景颜色没有设置（0）将是黑色。
			if (tzColorSet)
			{
				tzRECT = &s_rtText;
			}
			else
			{
				tzRECT = &s_rtScreen;
				tzColorSet = true;
			}
			FillRect(ps.hdc, tzRECT, CreateSolidBrush(BackgroundColor));
		}
		OutPutText(ps.hdc);
		EndPaint(hWnd, &ps);
		tzDrawTextDone = TRUE;
		break;

	case WM_ERASEBKGND: //0x0014
		HANDLE hGraph;
		if (s_hImage && (pGdipCreateFromHDC((HDC)wParam, &hGraph) == S_OK))
		{
			pGdipDrawImageRectI(hGraph, s_hImage, 0, 0, s_rtScreen.right, s_rtScreen.bottom);
			return (INT_PTR)GetStockObject(NULL_BRUSH);
		}
		break;

	case WM_INITDIALOG: //0x0110
		// 调整整个窗口的大小，从初始值400*300调整为全屏
		s_hLogo = hWnd;
		s_rtScreen.right = GetSystemMetrics(SM_CXSCREEN);
		s_rtScreen.bottom = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, s_rtScreen.right, s_rtScreen.bottom, 0);

#ifndef LWA_COLORKEY
		SetLayeredWindowAttributes = (PSLW)GetProcAddress(GetModuleHandle(TEXT("USER32")), "SetLayeredWindowAttributes");
		if (SetLayeredWindowAttributes)
#endif
		{
			SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
		}
		break;

	case WM_COMMAND: //0x0111
		if (wParam == IDCANCEL)
		{
			LOG_INFO("IDCANCEL");
#ifndef LWA_COLORKEY
			if (SetLayeredWindowAttributes)
#endif
			{
				// Close window
				for (UINT i = 0; i <= 255; i += 5, Sleep(20))
				{
					// Fake out
					SetLayeredWindowAttributes(hWnd, 0, 255 - i, LWA_ALPHA);
				}
			}
			s_hLogo = NULL;
			return EndDialog(hWnd, IDCLOSE);
		}
		break;
	}
	return FALSE;
}


#define UserKey TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")
/**
 * \brief 从注册表SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon中读取名为Background的值作为LOGO的背景颜色。
 * R/G/B以空格分割。
 * \return 背景RGB颜色。
 */
DWORD GetBackgroundColor(void)
{
	DWORD dwType = REG_SZ;
	DWORD dwSize = MAX_NAME;
	TCHAR tzStr[MAX_NAME];
	TCHAR tSubKey[MAX_NAME];
	wsprintfW(tSubKey, TEXT("%s\\Winlogon"), UserKey);

	SHGetValue(HKEY_LOCAL_MACHINE,
		tSubKey,
		TEXT("Background"),
		&dwType,
		&tzStr[0],
		&dwSize);

	CHAR R, G, B;

	R = UWStrToInt(tzStr);
	PTSTR p = StrChrW(tzStr, L' ');
	if (p)
	{
		*p++ = 0;
		G = UWStrToInt(p);
		p = StrChrW(p, L' ');
		if (p)
		{
			*p++ = 0;
			B = UWStrToInt(p);
		}
	}
	return RGB(R, G, B);
}

BOOL SetLogoImp(LPCTSTR lpImgPath, COLORREF bgColor/*=0*/)
{
	BackgroundColor = bgColor;
	if (!BackgroundColor)
	{
		BackgroundColor = GetBackgroundColor();
	}

	HMODULE hLib = NULL;
	HANDLE hToken = NULL;

	if (PathFileExists(lpImgPath))
	{
		// Copy parameter
		WCHAR wzPath[MAX_PATH];
		wcscpy(wzPath, lpImgPath);

		ExistLogoImage = true;
		if (s_hLogo)
		{
			// 如果LOGO窗口已经存在，那么调用GdipLoadImageFromFile重新载入图片。
			if (s_hImage)
			{
				pGdipDisposeImage(s_hImage);
				s_hImage = NULL;
			}
			if (pGdipLoadImageFromFile)
			{
				pGdipLoadImageFromFile(wzPath, &s_hImage);
			}
			return InvalidateRect(s_hLogo, NULL, TRUE);
		}

		// 第一次运行，先加载gdi+的DLL
		hLib = LoadLibrary(TEXT("GdiPlus.dll"));
		if (hLib == NULL)
		{
			// Search GDI+
			// 加载X:\Windows\WinSxs\*\dll
			HANDLE hFind;
			WIN32_FIND_DATA fd;
			TCHAR tzDir[MAX_PATH];
			ExpandEnvironmentStrings(TEXT("%SystemRoot%\\WinSxS\\*"), tzDir, MAX_PATH);
			if ((hFind = FindFirstFile(tzDir, &fd)) != INVALID_HANDLE_VALUE)
			{
				do
				{
					if ((fd.cFileName[0] != '.') && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						TCHAR tzPath[MAX_PATH];
						wsprintfW(tzPath, TEXT("%s\\%s\\GdiPlus.dll"), tzDir, fd.cFileName);
						hLib = LoadLibrary(tzPath);
					}
				} while (!hLib && FindNextFile(hFind, &fd));
				FindClose(hFind);
			}
		}

		// Get GDI+ function
		pGdiplusStartup = (PGdiplusStartup)GetProcAddress(hLib, "GdiplusStartup");
		pGdiplusShutdown = (PGdiplusShutdown)GetProcAddress(hLib, "GdiplusShutdown");
		pGdipLoadImageFromFile = (PGdipLoadImageFromFile)GetProcAddress(hLib, "GdipLoadImageFromFile");
		pGdipDisposeImage = (PGdipDisposeImage)GetProcAddress(hLib, "GdipDisposeImage");
		pGdipCreateFromHDC = (PGdipCreateFromHDC)GetProcAddress(hLib, "GdipCreateFromHDC");
		pGdipDrawImageRectI = (PGdipDrawImageRectI)GetProcAddress(hLib, "GdipDrawImageRectI");

		// Load Image
		if (pGdipLoadImageFromFile)
		{
			GPSTART s = { 1 };
			pGdiplusStartup(&hToken, &s, NULL);
			pGdipLoadImageFromFile(wzPath, &s_hImage);
		}
	}

	// 首次运行，或者指定的图片没有都会运行到这里。
	// 创建一个400 * 300的窗口
	DialogBoxIndirect(g_hInst, (LPCDLGTEMPLATE)&c_dtLogo, NULL, (DLGPROC)LogoProc);
	LOG_INFO("LOGO窗口关闭。");
	if (hLib)
	{
		// Free GDI+
		if (s_hImage)
		{
			pGdipDisposeImage(s_hImage);
			s_hImage = NULL;
		}
		if (pGdiplusShutdown)
		{
			pGdiplusShutdown(hToken);
		}
		FreeLibrary(hLib);
	}
	return TRUE;
}

typedef struct tagLogoThreadParam
{
	TCHAR szImagePath[MAX_PATH];
	COLORREF bgColor;
}LogoThreadParam, *PLogoThreadParam;

DWORD WINAPI thread_logo(LPVOID lpThreadParam)
{
	PLogoThreadParam pParam = (PLogoThreadParam)lpThreadParam;
	SetLogoImp(pParam->szImagePath, pParam->bgColor/*=0*/);
	delete pParam;
	return 0;
}

#define RETRY 20
void WaitLogo(BOOL ReDoLoad)
{
	if (ReDoLoad)
	{
		PE_SetLogo(NULL,0);
	}

	INT i = -180;
	while ((!s_hLogo) && (i < RETRY))
	{
		Sleep(50);
		i++;
	}

	if (s_hLogo)
	{
		SetProcessWorkingSetSize(CurrentProcess, -1, -1);
	}
	else
	{
		Boot(MessageBoxA(s_hLogo, "可能因内存不足，加载 LOGO 出现错误，是否重新启动？\r\n选择[是]则重启，[否]则关机。", "发生错误！", MB_ICONQUESTION) == IDYES);
	}
}

BOOL PE_SetLogo(LPCTSTR lpImgPath, COLORREF bgColor/*=0*/)
{
	if (!lpImgPath || !lpImgPath[0])
	{
		LOG_INFO("关闭LOGO窗口");
		PostMessage(s_hLogo, WM_COMMAND, IDCANCEL, 0);
		DeleteObject(hfont);
	}
	else
	{

		tzDrawTextDone = FALSE;
		ExistLogoImage = FALSE;
		//
		//创建线程，在线程中创建和显示LOGO窗体
		//
		LogoThreadParam* px = new LogoThreadParam;
		wcscpy(px->szImagePath, lpImgPath);
		px->bgColor = bgColor;
		CloseHandle(CreateThread(NULL, 0, thread_logo, px, 0, NULL));
		//等候 Logo 窗口启动
		WaitLogo(FALSE);
		LOG_INFO("### Logo 窗口启动完毕 ###");
	}

	return TRUE;
}

BOOL firstText = TRUE;
DWORD WINAPI SetFirstTextDone(LPVOID lpThreadParam)
{
	Sleep(1000);
	firstText = FALSE;
	return 0;
}

void WaitDrawText(void)
{
	DWORD timestart = GetTickCount();
	MSG msg;
	while (((GetTickCount() - timestart < 500) && (!tzDrawTextDone)) || (!firstText))
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		DispatchMessage(&msg);
	}
}

#define deffontname TEXT("Tahoma")
HRESULT PE_Text(LPCTSTR pszText, COLORREF color, LPCTSTR font, int FontSize, long left, long top, long right, long bottom)
{
	if (!pszText)
	{
		return FALSE;
	}

	LPCTSTR strFontType = deffontname;
	if (font && font[0])
	{
		strFontType = font;
	}

	UINT static old_fontsize = 0;
	s_crText = color;

	if (left == -1 &&
		top == -1 &&
		right == -1 &&
		bottom == -1)
	{
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		s_rtText.left = width / 2 - 80;
		s_rtText.right = width;
		s_rtText.top = 40;
		s_rtText.bottom = 80;
	}
	else
	{
		s_rtText.left = left;
		s_rtText.top = top;
		s_rtText.right = right;
		s_rtText.bottom = bottom;
	}

	fontsize = FontSize;

	BOOL rClaer = (pszText[0] == NULL);

	_tcscpy(s_tzText, pszText);

	HRESULT hResult = S_OK;

	if ((fontsize) && (old_fontsize != fontsize))
	{
		if (hfont)
		{
			DeleteObject(hfont);
		}
		hfont = CreateFont(fontsize, 0, 0, 0, 400, 0, 0, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, strFontType);
		if (NULL == hfont)
		{
			LOG_INFO("CreateFont失败[%d]", GetLastError());
		}
		old_fontsize = fontsize;
	}

	if (rClaer)
	{
		LOG_INFO("Clear all text.");
		hResult = InvalidateRect(s_hLogo, &s_rtText, TRUE) ? S_OK : S_FALSE;
	}

	if ((!s_hLogo) || (!rClaer))
	{
		LOG_INFO("DrawText [%s] in area (%d,%d)(%d,%d)", String::fromStdWString(s_tzText).c_str(), s_rtText.left, s_rtText.top, s_rtText.right, s_rtText.bottom);
		hResult = OutPutText(GetDC(s_hLogo));
		InvalidateRect(s_hLogo, &s_rtText, TRUE);
	}

	if (!firstText)
	{
		CloseHandle(CreateThread(NULL,0,SetFirstTextDone, NULL,0,NULL));
	}

	if (!tzDrawTextDone)
	{
		WaitDrawText();
	}
	return hResult;
}


BOOL PE_Wall(LPCTSTR WallPaperPath)
{
	if (PathFileExists(WallPaperPath))
	{
		LOG_INFO("设置墙纸:[%s]", String::fromStdWString(WallPaperPath).c_str());
		SHSetValue(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), TEXT("Wallpaper"), REG_SZ, (PTSTR)WallPaperPath, _tcslen(WallPaperPath) * sizeof(TCHAR));
		if (!SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)WallPaperPath, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE))
		{
			LOG_ERROR("设置墙纸发生了错误[%d]", GetLastError());
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

