

//////////////////////////////////////////////////////////////////////////////////////////////////
// Header
#pragma comment(linker, "/SECTION:PECMD,")
#pragma comment(linker, "/MERGE:.text=PECMD")
#pragma comment(linker, "/MERGE:.rdata=PECMD")
#pragma comment(lib, "ShLwAPI.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "MSVCRT.LIB")

#define _WIN32_WINNT		0x501
#define RETRY				20
#define MD5Str_Len         	16
#define FindShellMsg		WM_USER + 01
#define CM_nTaskIcoMsg		WM_USER + 02
#define PELOGON				TEXT("SOFTWARE\\PELOGON")
#define LEN_Command			4

#include "Define.h"
#include "UniBase.h"
#include "Resource.h"
#include "HIDEPROCESS.H"
#include "MD5.H"

HWND			PassButton;
BOOL			PassWordEditEnter;
HHOOK			g_hHook = NULL;
HMENU			hSubMenu;
HWND			HelphWND;
HWND			s_hLogo = NULL;
HINSTANCE		g_hInst = NULL;
UINT			HotkCount = 0;
INT				g_iShowCmd = SW_NORMAL;
BOOL			TrayIcon = FALSE;
BOOL			CheckMe = FALSE;
TCHAR			MyShell[MAX_PATH];
COLORREF		EditBackColor = 0xFFFFE0;
COLORREF		EditTextColor = 0x000AC0;
TCHAR			HotKeyCommand[8][MAX_PATH] = {0};
HWND			MainWindowHwnd = 0;
HANDLE			g_hXLog = NULL;
HANDLE			CurrentProcess = NULL;
TCHAR			LogFileName[MAX_PATH];
TCHAR			*LogFileData;
PTSTR			pFileStart = NULL;
TCHAR			PECMDCurrentDir[MAX_PATH];
TCHAR          	MD5_Code_Buf[MD5Str_Len*2];
NOTIFYICONDATA	FpNotify;
TCHAR			MenuCommand[128][MAX_PATH] = {0};
TCHAR			HotkCmd[MAX_PATH];
DWORD			MenuFirstID = 0;
TCHAR			LogoDescText[MAX_NAME] = {0};
BOOL			NotChineseLangID;
TCHAR			MenuTipText[MAX_NAME];
BOOL			g_bInit = FALSE;
DWORD			LastError = 0;

const WORD UDescStr[] = //LOGO信息字符串，以#结束
{
	0x4F,0x43,0x40,0x49,0x3F,0x1A,0x26,0x18,
	0x4E,0x5F,0x63,0x44,0x38,0x12,0x34,0x5F,
	0x5C,0x5B,0x4E,0x5A,0x4F,0x4F,0x5B,0x08,
	0x0F,0x33,0x54,0x48,0x4C,0x48,0x4A,0x45,
	0x43,0xFFFE,0x1F,0x55,0xFFFB,0x26,0x51,0x44,
	0x08,0x0C,0x08,0x0C,0xFFFC,0xFFF2,0xFFFD,0xFFF0,
	0x25,0x00,0xFFFB,0x04,0xFFF9,0x02,0xFFF9,0xFFFA,
	0xFFF5,0xFFF6,0xFFF6,0xFFFA,0xFFF5,0xFFE5,
};

const WORD Help_Text_MD5Str[] = //帮助说明文件的MD5
{
	0x65,0x5F,0x2F,0x5F,0x2F,0x2E,0x2C,0x31,
	0x58,0x5B,0x57,0x26,0x25,0x24,0x56,0x26,
	0x27,0x4F,0x53,0x20,0x20,0x4C,0x20,0x1A,
	0x17,0x1B,0x18,0x15,0x47,0x14,0x43,0x11,
};

const WORD Ver_MD5Str[] = //EXE文件版本信息的MD5
{	
	0x2F,0x31,0x33,0x61,0x5E,0x32,0x5B,0x5E,
	0x58,0x5C,0x26,0x26,0x58,0x28,0x24,0x28,
	0x53,0x53,0x20,0x1C,0x21,0x1D,0x22,0x19,
	0x1A,0x1E,0x46,0x1B,0x1C,0x46,0x16,0x12,
};

const WORD AppTrayText[] = //托盘图标提示字符串
{
	0x8000,0x4e5b,0x54,0x65,0x69,0x4a,0x3e,0x8f7d,
	0x52a0,0x5ddb,0x516c,
};


const WORD DescTextMD5[] = //LOGO字符串的MD5(不包括"#")
{	
	0x34,0x60,0x35,0x34,0x5D,0x2A,0x2C,0x2B,
	0x5D,0x2B,0x2A,0x27,0x24,0x57,0x54,0x56,
	0x1F,0x26,0x4E,0x52,0x50,0x1F,0x1E,0x4C,
	0x4C,0x19,0x4A,0x15,0x48,0x15,0x14,0x10,
};

const WORD PECMDHelp[] = // PECMD使用说明,
{
	0x24,0x51,0x71,0x5d,0x6d,0x6e,0x46,0x5d,
	0x65,0x6b,0x1a,0x50,0x43,0x37,0x34,0x3d,
	0x33,0x4f6d,0x7515,0x8be0,0x65f9,0x16,
};

const TCHAR c_tzCmd[][5] = //命令列表
{
	//TEXT("TEST"),
	TEXT("MAIN"), TEXT("LOAD"), TEXT("EXEC"), TEXT("CALL"),
	TEXT("REGI"), TEXT("ENVI"), TEXT("FILE"), TEXT("LINK"),
	TEXT("SEND"), TEXT("WAIT"), TEXT("KILL"), TEXT("SHUT"),
	TEXT("DEVI"), TEXT("SERV"), TEXT("PAGE"), TEXT("DISP"), 

	TEXT("LOGO"), TEXT("TEXT"), TEXT("LOGS"), TEXT("IFEX"),	
	TEXT("FBWF"), TEXT("WALL"), TEXT("RAMD"), TEXT("RUNS"),
	TEXT("NUMK"), TEXT("USER"), TEXT("SHEL"), TEXT("FONT"),
	TEXT("TEAM"), TEXT("FIND"), TEXT("HOTK"), TEXT("INIT"),
	TEXT("PATH"), TEXT("MESS"), TEXT("BROW"), TEXT("HELP"), 
	TEXT("EJEC"), TEXT("TEMP"), TEXT("MD5C"), TEXT("MOUN"),
	/*TEXT("CALC"),*/ /*TEXT("TINT"),/* TEXT("TSTR"),*/ NULL,	
	//Command函数不解释 _END 和 _SUB 两个关键词
};

enum  //命令ID
{
	//CMD_TEST,
	CMD_MAIN, CMD_LOAD, CMD_EXEC, CMD_CALL,
	CMD_REGI, CMD_ENVI, CMD_FILE, CMD_LINK,
	CMD_SEND, CMD_WAIT, CMD_KILL, CMD_SHUT,
	CMD_DEVI, CMD_SERV, CMD_PAGE, CMD_DISP,

	CMD_LOGO, CMD_TEXT, CMD_LOGS, CMD_IFEX,
	CMD_FBWF, CMD_WALL, CMD_RAMD, CMD_RUNS,
	CMD_NUMK, CMD_USER, CMD_SHEL, CMD_FONT,
	CMD_TEAM, CMD_FIND, CMD_HOTK, CMD_INIT,
	CMD_PATH, CMD_MESS, CMD_BROW, CMD_HELP,
	CMD_EJEC, CMD_TEMP, CMD_MD5C, CMD_MOUN,	
	//CMD_CALC,//CMD_TINT,//CMD_TSTR,
	// _END 和 _SUB 两个关键词由 LoadFile 函数解释
};
//////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
/*
void DebugMsg(PTSTR tzStr,HRESULT tzVal)
{
	TCHAR tzValStr[MAX_PATH];
	UStrPrint(tzValStr,TEXT("%d"),tzVal);	
	MessageBox(s_hLogo, tzStr,tzValStr, MB_OK );
}
*/
///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//删字符串左空函数
PTSTR UStrLeftTrim(PTSTR ptstr)
{
	if (ptstr)
	{
		for (; (*ptstr == ' ') || (*ptstr == '\t'); ptstr++);
	}
	return ptstr;
}
///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//删字符串右空函数
PTSTR UStrRightTrim(PTSTR ptstr)
{
	PTSTR tmp=ptstr;
	if (ptstr)
	{
		DWORD i=UStrLen(ptstr)-1;
		ptstr=ptstr + i;
		while ((*ptstr == ' ') || (*ptstr == '\t'))
		{
			*ptstr=0;
			ptstr--;
		}
	}
	return tmp;
}
///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//删字符串前后空函数
PTSTR UStrTrim(PTSTR ptstr)
{
	return UStrRightTrim(UStrLeftTrim(ptstr));
}
///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//从文件名中分离出文件路径
PTSTR UPathSplitName(PTSTR ptzPath)
{
	PTSTR p = UStrRChr(ptzPath, '\\');
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
///////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
PTSTR USplitStr(PTSTR ptzStr)
{
	PTSTR pzOld = ptzStr;
	while (*ptzStr)
	{
		if (*ptzStr == '\\')
		{
			if (ptzStr[1] == 'n')
			{
				*ptzStr++ = '\r';
				*ptzStr++ = '\n';
			}
		}
		ptzStr++;
	}
	return pzOld;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
DWORD GetMemo(BOOL FreeMemo)
{	
	MEMORYSTATUSEX statex;
	DWORD hResult = 0;
	ZeroMemory(&statex, sizeof(statex));
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	return (FreeMemo ? ((DWORD) statex.ullAvailPhys >> 20 ): ((DWORD) statex.ullTotalPhys >> 20));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD GetFreeMemo(VOID)
{	
	return GetMemo(true) + 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD GetTotalMemo(VOID)
{	
	return GetMemo(false) + 1;
}
//////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
HRESULT GetDiskSize(PTSTR pszDrive ,BOOL FreeSize,BOOL DiskExis)
{
	__int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;	
	BOOL hResult = false;

	hResult = GetDiskFreeSpaceEx(pszDrive,
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);

	if (DiskExis) 
	{
		return hResult;
	}
	else
	{
		return (FreeSize ? (DWORD) (i64FreeBytes >> 20): (DWORD) (i64TotalBytes >> 20));
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD GetDiskFree(PTSTR pszDrive)
{
	return GetDiskSize(pszDrive,true,false);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD GetDiskTotal(PTSTR pszDrive)
{
	return GetDiskSize(pszDrive,false,false);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DiskIsExist(PTSTR pszDrive)
{
	return GetDiskSize(pszDrive,false,true);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
/*
PTSTR UStrToUINT16(PTSTR ptzCmd,PTSTR OutStr)
{
TCHAR UINTStr[MAX_STR] = {0};
TCHAR temp[MAX_NAME];
UINT i = 0;
while (*ptzCmd)
{		
UStrPrint(temp, TEXT("0x%02x,"),(WORD)(*ptzCmd - i -1));
UStrCat(UINTStr,temp);
ptzCmd++;
i++;
}
UStrCopy(OutStr,UINTStr);
return OutStr;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
PTSTR UINT16ToUStr(PTSTR ptzBuf,const WORD ByteArray[],WORD pzBufLen)
{
	WORD i = 0;
	for (i = 0; (i <pzBufLen); i++)
	{
		ptzBuf[i] = ByteArray[i] + i + 1;
	}
	ptzBuf[i] = 0;
	return ptzBuf;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
PTSTR ByteToMD5(const BYTE pByteBuf[],UINT pBufLen)
{
	BYTE rData[MAX_NAME] = {0};
	MD5((PBYTE)&pByteBuf[0],pBufLen,&rData[0]);
	TCHAR tzByte[3];
	UMemSet(MD5_Code_Buf,0,UStrLen(MD5_Code_Buf));
	UINT i = 0;
	while (i < MD5Str_Len)
	{
		UStrPrint(tzByte, TEXT("%02x"),rData[i]);
		UStrCat(MD5_Code_Buf,tzByte);
		i++;
	}
	return  MD5_Code_Buf;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////

PTSTR GetModuleVerInfoMD5(PTSTR ModuleName,PTSTR pVer)
{
	DWORD dw = 0;	
	DWORD InfoSize = GetFileVersionInfoSize(ModuleName,&dw);
	if (InfoSize)
	{
		PBYTE VerBuf = NULL;;
		VerBuf = (PBYTE)UMemRealloc(VerBuf,sizeof(BYTE)* InfoSize);
		UMemSet(VerBuf,0,InfoSize);
		if (GetFileVersionInfo(ModuleName,NULL,InfoSize,&VerBuf[0]))
		{	
			PTSTR p = ByteToMD5(VerBuf,InfoSize);
			UStrCopy(pVer,p);
		}
		UMemFree(VerBuf);
	}	
	return pVer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT SetClipboardText(PTSTR ptzText)
{	
	CHAR tzTempBuf[MAX_STR];
	UStrToAStr(tzTempBuf,ptzText,MAX_STR);
	UINT nSize = UAStrLen(tzTempBuf);
	BOOL bResult = OpenClipboard(NULL);
	if (bResult)
	{
		if (EmptyClipboard())
		{
			HGLOBAL hGlobal = GlobalAlloc(GMEM_ZEROINIT, nSize+1);
			if (hGlobal)
			{
				LPSTR lpszData = (LPSTR)GlobalLock(hGlobal);
				if (lpszData)
				{
					UMemCopy(&lpszData[0],&tzTempBuf[0],nSize+1);
					GlobalUnlock(hGlobal);
					bResult = (BOOL)SetClipboardData(CF_TEXT, hGlobal);
				}
				GlobalFree(hGlobal);
			}
		}
		CloseClipboard();
	}
	return bResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CheckVerDM5(void)
{	
	BOOL bResult = FALSE;
	TCHAR ModuleName[MAX_PATH];
	GetModuleFileName(NULL, ModuleName, MAX_PATH);
	PTSTR pVerBuf = GetModuleVerInfoMD5((PTSTR)ModuleName,(PTSTR)MD5_Code_Buf);
	CHAR pz_MD5Str[MAX_NAME] = {0};
	PTSTR pVer = UINT16ToUStr((PTSTR)pz_MD5Str,Ver_MD5Str,_NumOf(Ver_MD5Str));
	//SetClipboardText(pVerBuf);//		
	bResult = (!UStrCmpNI(pVer,pVerBuf,UStrLen(pVerBuf)));
	//DebugMsg(pVerBuf,bResult);//
	return bResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckHelpDM5(void)
{		
	BOOL bResult = FALSE;
	HRSRC hRsrc = FindResource(g_hInst, _MakeIntRes(IDR_Help), RT_RCDATA);
	if (hRsrc)
	{
		PBYTE Help_Text = (PBYTE)LoadResource(g_hInst,hRsrc);
		if (Help_Text)
		{
			DWORD tzStrLen = SizeofResource(g_hInst,hRsrc);
			TCHAR pzTempBuf[MAX_NAME] = {0};
			PTSTR pHelpMd5 = ByteToMD5((PBYTE)Help_Text,tzStrLen);	
			TCHAR pz_MD5Str[MAX_NAME] = {0};
			PTSTR pHelp = UINT16ToUStr((PTSTR)pz_MD5Str,Help_Text_MD5Str,_NumOf(Help_Text_MD5Str));	
			//SetClipboardText(pHelpMd5);//
			bResult = (!UStrCmpNI(pHelp,pHelpMd5,UStrLen(pHelp)));
			//DebugMsg(pHelpMd5,bResult);//		
			FreeResource(Help_Text);
			Help_Text = NULL;
		}
	}
	return bResult;	
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckMERes(void)
{
	return (NotChineseLangID || (CheckHelpDM5() && CheckVerDM5()));
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
TCHAR szVolumeName[8] = TEXT("\\\\.\\C:");
UINT MyGetDriveType(PTSTR ptzDriver)
{
	if (!ptzDriver)
	{
		return NULL;
	}

	UINT uDriveType = GetDriveType(ptzDriver);
	if ((uDriveType == DRIVE_FIXED) || (uDriveType == DRIVE_REMOVABLE))
	{	
		DWORD dwAccessFlags = GENERIC_READ | GENERIC_WRITE;
		szVolumeName[4] = *ptzDriver;
		HANDLE hDevice = CreateFile(szVolumeName,
			dwAccessFlags,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		if (hDevice != INVALID_HANDLE_VALUE)
		{
			STORAGE_HOTPLUG_INFO stu_HOTPLUG;
			DWORD lngRet;
			if(DeviceIoControl(hDevice, 
				IOCTL_STORAGE_GET_HOTPLUG_INFO,
				NULL,
				0,
				&stu_HOTPLUG,
				sizeof(STORAGE_HOTPLUG_INFO),
				&lngRet,
				NULL))
			{
				if (stu_HOTPLUG.DeviceHotplug)
				{
					uDriveType = DRIVE_REMOVABLE;
				}
			}
			CloseHandle(hDevice);
		}
	}
	return uDriveType;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/*
UINT GetOSVersion(void)
{
g_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
GetVersionEx (&g_osvi);
return (g_osvi.dwMajorVersion*10 + g_osvi.dwMinorVersion);
}
*/
/////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Log result
VOID XLog(UINT uFmtRes, ...)
{
	if (g_hXLog)
	{
		va_list va;
		TCHAR tzLog[MAX_STR];
		TCHAR tzStr[MAX_PATH];
		va_start(va, uFmtRes);
		UINT i = UStrPrintV(tzLog, _GetStr(uFmtRes), va);
		va_end(va);
		UStrCat(LogFileData,tzLog);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
#define UserKey TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")
DWORD GetBackgroundColor(void)
{
	DWORD dwType=REG_SZ;
	DWORD dwSize=MAX_NAME;
	TCHAR tzStr[MAX_NAME];
	TCHAR tSubKey[MAX_NAME];
	UStrPrint(tSubKey,TEXT("%s\\Winlogon"),UserKey);	

	SHGetValue(HKEY_LOCAL_MACHINE,
		tSubKey,
		TEXT("Background"),
		&dwType, 
		&tzStr[0], 
		&dwSize);

	CHAR R,G,B;

	R = UStrToInt(tzStr);
	PTSTR p = UStrChr(tzStr, ' ');
	if (p)
	{
		*p++ = 0;
		G = UStrToInt(p);
		p = UStrChr(p, ' ');
		if (p)
		{
			*p++ = 0;
			B = UStrToInt(p);
		}

	}
	return RGB(R,G,B);
}
/////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
DWORD  EditDefWndProc;
HRESULT CALLBACK EditNewProc (HWND hEditHwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	BOOL tzOpDisabled;
	switch (uMsg)
	{
	case WM_KEYDOWN: //键盘消息
	case WM_KEYUP:
	case WM_CHAR:
		tzOpDisabled = ((wParam == 0x08)||(wParam == 0x2E)||(wParam == 0x6E));			
		if (!tzOpDisabled) //非删除键(Back、Delete、Del)
		{
			BOOL Ctrl_C = ((GetAsyncKeyState(0x43) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000));					
			if (!Ctrl_C) // 非(Ctrl+C)
			{
				tzOpDisabled = ((wParam<0x21) || (wParam>0x28)); //非移动光标
			}
		}
		break;

		//鼠标消息
	case WM_CUT: //剪切
	case WM_COPY: //复制
	case WM_PASTE: //粘帖
	case WM_CLEAR: //删除
		tzOpDisabled = (uMsg != WM_COPY);
		break;

	default:
		tzOpDisabled = false;
	}

	return tzOpDisabled? TRUE:CallWindowProc((WNDPROC)EditDefWndProc, hEditHwnd, uMsg, wParam,lParam);
}
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK HelpProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static LOGBRUSH	logbrush;
	static HBRUSH	hbrush;
	static BOOL bNoSel = FALSE;	
	static HWND hEdit;

	if (!hEdit)
	{
		hEdit = GetDlgItem(hWnd, IDC_Help);
	}
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:		
		DeleteObject(hbrush);
		return FALSE;

	case WM_INITDIALOG:		
		TCHAR tzStr[MAX_NAME];
		TCHAR tzText[MAX_PATH];

		SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(g_hInst,(LPCWSTR)IDI_Main)); 
		//SendMessage(hWnd,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(g_hInst,(PTSTR)IDI_Main)); 

		GetDlgItemText(hWnd, IDC_Version, tzStr, MAX_PATH);
		UStrPrint(tzText, tzStr, STR_VersionStamp);
		SetDlgItemText(hWnd, IDC_Version, tzText);

		UStrPrint(tzText,TEXT("%s - %s"),STR_AppName,STR_AppDesc);
		SetDlgItemText(hWnd, IDC_AppDesc,tzText);

		GetDlgItemText(hWnd, IDC_Build, tzStr, MAX_PATH);
		UStrPrint(tzText, tzStr, STR_BuildStamp);
		SetDlgItemText(hWnd, IDC_Build, tzText);
		SetDlgItemTextA(hWnd, IDC_Help, (PCSTR) LoadResource(g_hInst, FindResource(g_hInst, _MakeIntRes(IDR_Help), RT_RCDATA)));

		if (hEdit)
		{
			HelphWND = hWnd;
			SendMessage(hEdit,EM_SETREADONLY,FALSE,NULL);
			EditDefWndProc = GetWindowLongPtr(hEdit, GWLP_WNDPROC);
			SetWindowLongPtr(hEdit, GWLP_WNDPROC,(LONG)EditNewProc);
		}
		return TRUE;

	case WM_CTLCOLOREDIT:		
		SetTextColor((HDC)wParam,EditTextColor);
		SetBkColor((HDC)wParam,EditBackColor);
		logbrush.lbColor = EditBackColor;
		hbrush = CreateBrushIndirect(&logbrush);
		if ((!bNoSel) && (hEdit))
		{
			bNoSel = TRUE;
			SendMessage(hEdit,EM_SETSEL,-1,NULL);			
			SetProcessWorkingSetSize(CurrentProcess,-1,-1);
		}		
		return (long)hbrush;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, IDCANCEL);
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//  Set Windows Registry
#define hKCU TEXT("HKCU")
HRESULT Regi(PTSTR ptzCmd)
{
	PTSTR ptzSubKey = UStrChr(ptzCmd, '\\');
	if (!ptzSubKey)
	{
		return E_INVALIDARG;
	}
	else
	{
		*ptzSubKey++ = 0;
	}

	HKEY hKey;
	if (UStrCmpI(ptzCmd, TEXT("HKLM")) == 0)
	{
		hKey = HKEY_LOCAL_MACHINE;
	}
	else if (UStrCmpI(ptzCmd, hKCU) == 0)
	{
		hKey = HKEY_CURRENT_USER;
	}
	else if (UStrCmpI(ptzCmd, TEXT("HKCR")) == 0)
	{
		hKey = HKEY_CLASSES_ROOT;
	}
	else if (UStrCmpI(ptzCmd, TEXT("HKU")) == 0)
	{
		hKey = HKEY_USERS;
	}
	else if (UStrCmpI(ptzCmd, TEXT("HKCC")) == 0)
	{
		hKey = HKEY_CURRENT_CONFIG;
	}
	else
	{
		return E_INVALIDARG;
	}

	PTSTR ptzData = UStrChr(ptzSubKey, '=');
	if (ptzData)
	{
		*ptzData++ = 0;
		ptzData = UStrTrim(ptzData);
	}

	PTSTR ptzValue = UStrRChr(ptzSubKey, '\\');
	if (!ptzValue)
	{
		return E_INVALIDARG;
	}
	else
	{
		*ptzValue++ = 0;
	}

	if (ptzData && (*ptzData))
	{
		if (ptzData[0] == '#')
		{
			DWORD dwData = UStrToInt(ptzData + 1);
			return SHSetValue(hKey, ptzSubKey, ptzValue, REG_DWORD, &dwData, sizeof(DWORD));
		}
		else if (ptzData[0] == '@')
		{
			UINT i = 0;
			BYTE bData[MAX_PATH];
			for (ptzData++; i < sizeof(bData);)
			{
				bData[i++] = UStrToInt(ptzData);
				ptzData = UStrChr(ptzData, ' ');
				if (ptzData)
				{
					ptzData++;
				}
				else
				{
					break;
				}
			}
			return SHSetValue(hKey, ptzSubKey, ptzValue, REG_BINARY, bData, i);
		}
		else
		{
			return SHSetValue(hKey, ptzSubKey, ptzValue, REG_SZ, ptzData, UStrLen(ptzData) * sizeof(TCHAR));
		}
	}
	else
	{
		if ((ptzValue[0] == '!') && (ptzValue[1] == 0))
		{
			return SHDeleteKey(hKey, ptzSubKey);
		}
		else
		{
			return SHDeleteValue(hKey, ptzSubKey, ptzValue);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//  Folder macro
#include <ShlObj.h>
const struct {INT iFolder; PCTSTR ptzMacro;} c_sMacro[] =
{
	{CSIDL_FAVORITES,			TEXT("Favorites")},
	{CSIDL_DESKTOPDIRECTORY,	TEXT("Desktop")},
	{CSIDL_STARTMENU,			TEXT("StartMenu")},
	{CSIDL_STARTUP,				TEXT("Startup")},
	{CSIDL_PROGRAMS,			TEXT("Programs")},
	{CSIDL_SENDTO,				TEXT("SendTo")},
	{CSIDL_PERSONAL,			TEXT("Personal")},
	{CSIDL_APPDATA,				TEXT("QuickLaunch")},
};
//////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
#define Text_Environment TEXT("Environment")
#define REG_MemMgr TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management")
#define Session_Manager TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager")
HRESULT SetEnvironment(PTSTR ptzEnv, BOOL bSystem = FALSE,BOOL bInit = FALSE)
{	
	TCHAR tzStr[MAX_STR];
	HRESULT hResult = S_OK;

	if (!ptzEnv[0])
	{
		if (bInit)
		{
			{
				for (UINT i = 0; i < _NumOf(c_sMacro); i++)
				{
					if (ptzEnv[0])
					{
						SetEnvironment((PTSTR)c_sMacro[i].ptzMacro, bSystem,FALSE);
						continue;
					}

					PTSTR p = tzStr + UStrPrint(tzStr, TEXT("%s="), c_sMacro[i].ptzMacro);
					if (SHGetSpecialFolderPath(NULL, p, c_sMacro[i].iFolder, TRUE))
					{
						if (c_sMacro[i].iFolder == CSIDL_APPDATA)
						{
							// Trick
							UStrCat(p, TEXT("\\Microsoft\\Internet Explorer\\Quick Launch"));							
							UDirCreate(p);
							CreateDirectory(p, NULL);
						}
						SetEnvironment(tzStr, bSystem,FALSE);
					}
				}
			}
		}
		else
		{
			DWORD_PTR dwResult = 0;
			SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)Text_Environment, SMTO_ABORTIFHUNG, 5000, &dwResult);
		}
	}


	if ((bSystem) && ptzEnv[0])
	{
		UStrPrint(tzStr, TEXT("HKLM\\%s\\%s\\%s"),Session_Manager,Text_Environment,ptzEnv);
		Regi(tzStr); //设置系统级环境变量		
		UStrPrint(tzStr, TEXT("%s\\%s\\%s"),hKCU,Text_Environment,ptzEnv);
		Regi(tzStr); //设置用户级环境变量
	}

	PTSTR ptzValue = UStrChr(ptzEnv, '=');
	if (ptzValue)
	{
		// Trick : If no '=', ptzEnv can be a constant string.
		*ptzValue++ = 0;
	}

	if (ptzValue)
	{
		hResult = SetEnvironmentVariable(ptzEnv, ptzValue) ? S_OK : S_FALSE;
	}

	if (bSystem)
	{
		if (!ptzValue)
		{
			ptzValue = ptzEnv + UStrLen(ptzEnv) + 1;
		}
		DWORD_PTR dwResult = 0;
		SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)Text_Environment, SMTO_ABORTIFHUNG, 5000, &dwResult);
	}
	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////
//SetEnvironment
HRESULT Envi(PTSTR ptzCmd)
{
	BOOL bSys = (*ptzCmd == '$');
	ptzCmd = ptzCmd + bSys;
	return SetEnvironment(ptzCmd,bSys,((!ptzCmd) && bSys));
}
///////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
// Logo dialog
#ifndef LWA_COLORKEY
#define LWA_ALPHA 0x00000002
//#define LWA_COLORKEY 0x00000001
#define WS_EX_LAYERED 0x00080000
typedef BOOL (WINAPI *PSLW)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
PSLW SetLayeredWindowAttributes = NULL;
#endif

struct GPSTART {UINT uVersion; PROC pCallback; BOOL bSuppressThread; BOOL bSuppressExternal;};
typedef HRESULT (WINAPI* PGdiplusStartup)(HANDLE* phToken, const GPSTART* gpIn, PVOID pvOut);
typedef HRESULT (WINAPI* PGdiplusShutdown)(HANDLE hToken);
typedef HRESULT (WINAPI* PGdipLoadImageFromFile)(const PWSTR pwzPath, HANDLE* phImage);
typedef HRESULT (WINAPI* PGdipDisposeImage)(HANDLE hImage);
typedef HRESULT (WINAPI* PGdipCreateFromHDC)(HDC hDC, HANDLE* phGraph);
typedef HRESULT (WINAPI* PGdipDrawImageRectI)(HANDLE hGraph, HANDLE hImage, INT iLeft, INT iTop, INT iWidth, INT iHeight);

HANDLE s_hImage = NULL;

RECT s_rtText = {4, 4, 640, 64};
RECT s_rtScreen = {0,0,0,0};
RECT *tzRECT = NULL;
UINT UType = 0;

TCHAR s_tzText[MAX_NAME] = {0};
COLORREF s_crText = 0xFFFFFF;
COLORREF BackgroundColor = 0;

#define deffontname TEXT("Tahoma")
UINT fontsize = 14;
HFONT hfont = NULL;
bool tzColorSet = false;
bool ExistLogoImage = false;

PGdiplusStartup pGdiplusStartup = NULL;
PGdiplusShutdown pGdiplusShutdown = NULL;
PGdipDisposeImage pGdipDisposeImage = NULL;
PGdipCreateFromHDC pGdipCreateFromHDC = NULL;
PGdipDrawImageRectI pGdipDrawImageRectI = NULL;
PGdipLoadImageFromFile pGdipLoadImageFromFile = NULL;
BOOL tzDrawTextDone = TRUE;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT OutPutText(HDC hDC)
{
	SetBkMode(hDC, TRANSPARENT);				
	SetTextColor(hDC, s_crText);
	(hfont ? SelectObject(hDC, hfont): SelectObject(hDC, GetStockObject(DEFAULT_GUI_FONT)));		
	HRESULT hResult = DrawText(hDC, s_tzText, -1, &s_rtText, DT_LEFT | DT_WORDBREAK);
	if (!s_hLogo)
	{
		tzDrawTextDone = TRUE;
	}
	return hResult? S_OK:S_FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT TimeDelay(DWORD WaitTime)
{
	DWORD timestart = GetTickCount();	
	MSG msg;
	while ((GetTickCount() - timestart)< WaitTime)
	{
		PeekMessage(&msg,NULL,0,0,PM_REMOVE);   
		DispatchMessage(&msg);
	}	
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL firstText = TRUE;
PTSTR UStrToMD5(PTSTR);
void SetFirstTextDone(PTSTR ptzText)
{
	PTSTR ptzLogoText = UStrToMD5(ptzText); //ptzText LOGO字符串 ; ptzLogoText LOGO字符串MD5
	TCHAR LogoTextMD5[MAX_NAME] = {0};
	PTSTR pLogoTextMD5 = UINT16ToUStr((PTSTR)LogoTextMD5,DescTextMD5,_NumOf(DescTextMD5));
	BOOL bp = (!UStrCmpNI(pLogoTextMD5,ptzLogoText,UStrLen(ptzLogoText)));	
	Sleep(1000);
	firstText = bp;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
void WaitDrawText(void)
{
	DWORD timestart = GetTickCount();
	MSG msg;	
	while (((GetTickCount() - timestart< 500) && (!tzDrawTextDone)) ||(!CheckMe) || (!firstText))
	{
		PeekMessage(&msg,NULL,0,0,PM_REMOVE);   
		DispatchMessage(&msg);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK LogoProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DWORD timestart = 0;
	switch (uMsg)
	{
	case WM_DESTROY: //0x0002
		PostQuitMessage(0);
		break;

	case WM_PAINT:	 //0x000F
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		if (!ExistLogoImage)
		{
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
			return (INT_PTR) GetStockObject(NULL_BRUSH);
		}
		break;

	case WM_INITDIALOG: //0x0110
		// Adjust window size
		s_hLogo = hWnd;
		s_rtScreen.right=GetSystemMetrics(SM_CXSCREEN);
		s_rtScreen.bottom=GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, s_rtScreen.right, s_rtScreen.bottom, 0);


#ifndef LWA_COLORKEY
		SetLayeredWindowAttributes = (PSLW) GetProcAddress(GetModuleHandle(TEXT("USER32")), "SetLayeredWindowAttributes"); 
		if (SetLayeredWindowAttributes)
#endif
		{
			SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
		}	
		break;

	case WM_COMMAND: //0x0111
		if ((HWND)lParam == PassButton)
		{
			if (GetTickCount() - timestart > 1000)
			{
				timestart = GetTickCount();
				PassWordEditEnter = TRUE;
			}			
		}
		if (wParam == IDCANCEL)
		{
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
////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Logo(PTSTR);
HRESULT Boot(BOOL);
void WaitLogo(BOOL ReDoLoad)
{
	if (ReDoLoad)
	{
		TCHAR LogoCommand[MAX_NAME] = TEXT(",0");	
		Logo(LogoCommand);
	}

	INT i = -180;
	while ((!s_hLogo) && ( i < RETRY))
	{
		Sleep(50);
		i++;
	}	

	if (s_hLogo)
	{
		SetProcessWorkingSetSize(CurrentProcess,-1,-1);
	}
	else
	{
		TCHAR LogoErr[MAX_NAME];
		TCHAR LogoErrTip[MAX_NAME];
		LoadString(g_hInst,IDS_LogoErr,LogoErr, MAX_NAME);
		LoadString(g_hInst,IDS_LogoErrTip,LogoErrTip, MAX_NAME);
		Boot(MessageBox(s_hLogo,LogoErr,LogoErrTip,20) == 6);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
#include <TLHelp32.h>
PTSTR FindProcName(DWORD ProcID,PTSTR ProcessName)
{
	PTSTR hResult = NULL;	
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		for (BOOL b = Process32First(hSnap, &pe); b; b = Process32Next(hSnap, &pe))
		{	
			if (pe.th32ProcessID == ProcID)
			{	
				UStrCopy(ProcessName,pe.szExeFile);
				hResult = ProcessName;
				break;
			}		
		}
		CloseHandle(hSnap);
	}
	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
void SaveUserTextData(BOOL tzSave)
{
	static RECT tzOldTsRECT;
	static UINT tzOldFontSize;
	static COLORREF Old_s_crText;
	if (tzSave)
	{
		tzOldFontSize = fontsize;
		Old_s_crText = s_crText;
		tzOldTsRECT.bottom = s_rtText.bottom;
		tzOldTsRECT.left = s_rtText.left;
		tzOldTsRECT.right = s_rtText.right;
		tzOldTsRECT.top = s_rtText.top;
	}
	else
	{
		fontsize = tzOldFontSize;
		s_crText = Old_s_crText;
		s_rtText.bottom = tzOldTsRECT.bottom;
		s_rtText.left = tzOldTsRECT.left;
		s_rtText.right = tzOldTsRECT.right;
		s_rtText.top = tzOldTsRECT.top;
	}	
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT FindProc(PCTSTR);
BOOL ParentProcessIsME(void)
{
	HRESULT bResult = TRUE;
	TCHAR ParentProcessName[MAX_NAME];
	PTSTR p =FindProcName((DWORD)FindProc(NULL),ParentProcessName); //查找父进程，返回值填入ParentProcessName
	if (p)
	{
		bResult = ((!UStrCmpNI(TEXT("PECMD.EXE"),p, UStrLen(p)))||
			(!UStrCmpNI(TEXT("PELOGON.EXE"),p, UStrLen(p))));		
	}
	return bResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT Text(PTSTR);
TCHAR EXPLORER[] = TEXT("EXPLORER.EXE");
void DisplayAppDesc(void)
{	
	BOOL DisplayThis = (ParentProcessIsME() &&(!FindProc(EXPLORER))); //且不发现EXPLORER.EXE
	CheckMe = (!DisplayThis);
	firstText = (!DisplayThis);
	if (DisplayThis)
	{
		SaveUserTextData(TRUE);
		fontsize = 14;
		s_rtText.left = 8;
		s_rtText.top = 8;
		s_rtText.right = 400;
		s_rtText.bottom = 48;
		s_crText = 0xFFFFFF;
		tzDrawTextDone = FALSE;		
		CheckMe = CheckMERes();
		Text(LogoDescText);
		SaveUserTextData(FALSE);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
PTSTR UStrToMD5(PTSTR);
HRESULT Text(PTSTR ptzCmd)
{		
	UINT static old_fontsize = 0;
	BOOL Temporarily = true;
	PTSTR ptzAdd = NULL;
	PTSTR ptzParam;

	PTSTR p = UStrRChr(ptzCmd, '#');

	if (p)
	{
		UStrToUpper(p);
		ptzAdd = p;
		INT pzTemp = UStrToInt(++p);
		if ((s_crText != pzTemp) && (pzTemp>0))
		{
			s_crText = pzTemp;
		}
		ptzParam = p;
	}
	else
	{
		ptzParam = ptzCmd;
	}

	p = UStrRChr(ptzParam, 'L');
	if (p)
	{
		if (!ptzAdd)
		{
			ptzAdd = p;
		}
		s_rtText.left = UStrToInt(++p);		
	}

	p = UStrRChr(ptzParam, 'T');
	if (p)
	{
		if (!ptzAdd)
		{
			ptzAdd = p;
		}
		s_rtText.top = UStrToInt(++p);		
	}

	p = UStrRChr(ptzParam, 'R');
	if (p)
	{
		if (!ptzAdd)
		{
			ptzAdd = p;
		}
		s_rtText.right = UStrToInt(++p);
	}

	p = UStrRChr(ptzParam, 'B');
	if (p)
	{
		if (!ptzAdd)
		{
			ptzAdd = p;
		}
		s_rtText.bottom = UStrToInt(++p);
	}

	p = UStrRChr(ptzParam, '$');
	if (p)
	{		
		if (!ptzAdd)
		{
			ptzAdd = p;
		}

		fontsize = UStrToInt(++p);
	}

	p = UStrRChr(ptzParam, '*');
	if (p)
	{
		Temporarily = false; //不清除上次的文字
		if (!ptzAdd)
		{
			ptzAdd = p;
		}
	}	

	if (ptzAdd)
	{
		*ptzAdd = 0;
	}

	ptzCmd = UStrTrim(ptzCmd);
	BOOL rClaer = (*ptzCmd == NULL);
	ptzCmd = USplitStr(ptzCmd);

	if (!CheckMe)
	{
		CheckMe = CheckMERes();
	}

	UStrCopy(s_tzText,(PTSTR)((DWORD)ptzCmd * CheckMe));
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
			DEFAULT_PITCH | FF_DONTCARE, deffontname);
		old_fontsize = fontsize;
	}

	if (rClaer || Temporarily)
	{
		hResult = InvalidateRect(s_hLogo, &s_rtText, TRUE)? S_OK : S_FALSE;
	}	
	if ((!s_hLogo) || (!rClaer))
	{
		hResult = OutPutText(GetDC(s_hLogo));
	}

	if (!firstText)
	{	
		UThreadClose(UThreadCreate((UPROC)SetFirstTextDone,ptzCmd));
	}

	if (!tzDrawTextDone)
	{
		CheckMe = CheckMERes();
		WaitDrawText();
	}

	return hResult;
}
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
const struct {DLGTEMPLATE dtDlg; WCHAR  wzMenu[1]; WCHAR wzClass[1]; WCHAR wzCaption[6]; WORD wEnd[6];} c_dtLogo =
{{WS_POPUP | WS_VISIBLE | DS_CENTER, WS_EX_LAYERED | WS_EX_TOOLWINDOW, 0, 0, 0, 400, 300}, 0, 0, L"PECMD"};
BOOL WINAPI LogoCMD(PTSTR ptzCmd)
{		
	PTSTR p = UStrChr(ptzCmd, ',');
	if (p)
	{
		*p++ = 0;
		BackgroundColor = UStrToInt(p);
	}

	if (!BackgroundColor)
	{
		BackgroundColor=GetBackgroundColor();
	}

	HMODULE hLib = NULL;
	HANDLE hToken = NULL;

	if (!UFileExist(ptzCmd))
	{
		ptzCmd[0] = 0;
	}

	if (ptzCmd[0])
	{
		// Copy parameter
		WCHAR wzPath[MAX_PATH];
		UStrToWStr(wzPath, ptzCmd, MAX_PATH);
		ptzCmd[0] = NULL;
		ExistLogoImage = true;

		// Reload LOGO
		if (s_hLogo)
		{
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

		// Load GDI+
		hLib = LoadLibrary(TEXT("GdiPlus.dll"));
		if (hLib == NULL)
		{
			// Search GDI+
			HANDLE hFind;
			WIN32_FIND_DATA fd;
			TCHAR tzDir[MAX_PATH];
			ExpandEnvironmentStrings(TEXT("%SystemRoot%\\WinSxS\\*"), tzDir, MAX_PATH);
			if ((hFind = FindFirstFile(tzDir, &fd)) != INVALID_HANDLE_VALUE)
			{
				UDirSplitPath(tzDir);
				do
				{
					if ((fd.cFileName[0] != '.') && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						TCHAR tzPath[MAX_PATH];
						UStrPrint(tzPath, TEXT("%s\\%s\\GdiPlus.dll"), tzDir, fd.cFileName);
						hLib = LoadLibrary(tzPath);
					}
				}
				while (!hLib && FindNextFile(hFind, &fd));
				FindClose(hFind);
			}
		}

		// Get GDI+ function
		pGdiplusStartup = (PGdiplusStartup) GetProcAddress(hLib, "GdiplusStartup");
		pGdiplusShutdown = (PGdiplusShutdown) GetProcAddress(hLib, "GdiplusShutdown");
		pGdipLoadImageFromFile = (PGdipLoadImageFromFile) GetProcAddress(hLib, "GdipLoadImageFromFile");
		pGdipDisposeImage = (PGdipDisposeImage) GetProcAddress(hLib, "GdipDisposeImage");
		pGdipCreateFromHDC = (PGdipCreateFromHDC) GetProcAddress(hLib, "GdipCreateFromHDC");
		pGdipDrawImageRectI = (PGdipDrawImageRectI) GetProcAddress(hLib, "GdipDrawImageRectI");

		// Load Image
		if (pGdipLoadImageFromFile)
		{
			GPSTART s = {1};
			pGdiplusStartup(&hToken, &s, NULL);
			pGdipLoadImageFromFile(wzPath, &s_hImage);
		}
	}
	// Show LOGO
	DialogBoxIndirect(g_hInst, (LPCDLGTEMPLATE) &c_dtLogo, NULL, (DLGPROC) LogoProc);

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
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Get privilege
HRESULT Priv(PCTSTR ptzName)
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
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT SetNewDisplay(DEVMODE &dmOld,DEVMODE &dmNew)
{	
	if (!(ChangeDisplaySettings(&dmNew, CDS_UPDATEREGISTRY)))
	{	
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmOld);
		dmNew = dmOld;
		return S_OK;
	}
	else
	{
		ChangeDisplaySettings(&dmOld, CDS_UPDATEREGISTRY);
		dmNew = dmOld;
		return S_FALSE;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Change display setting
HRESULT Disp(PTSTR ptzCmd)
{	
	DEVMODE dmNew,dmOld,dmTmp;
	HRESULT lResult = S_OK;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmOld);	
	dmTmp = dmOld;

	UStrToUpper(ptzCmd);

	PTSTR temp = UStrChr(ptzCmd, 'W');
	if (temp)
	{
		dmTmp.dmPelsWidth = UStrToInt(++temp);  //水平分辨率 
	}

	temp = UStrChr(ptzCmd, 'H');
	if (temp)
	{
		dmTmp.dmPelsHeight = UStrToInt(++temp); //垂直分辨率
	}

	temp = UStrChr(ptzCmd, 'B');
	if (temp)
	{
		dmTmp.dmBitsPerPel = UStrToInt(++temp); //颜色质量
	}

	temp = UStrChr(ptzCmd, 'F');
	if (temp)
	{
		dmTmp.dmDisplayFrequency = UStrToInt(++temp);  //刷新率
	}

	temp=UStrChr(ptzCmd, 'T');
	if (temp)
	{
		TimeDelay(UStrToInt(++temp)); //等待时间	
	}

	dmNew = dmOld;
	if ((dmTmp.dmPelsWidth != dmOld.dmPelsWidth)||
		(dmTmp.dmPelsHeight != dmOld.dmPelsHeight))
	{
		dmNew.dmPelsWidth = dmTmp.dmPelsWidth;
		dmNew.dmPelsHeight = dmTmp.dmPelsHeight;
		lResult = SetNewDisplay(dmOld,dmNew) + lResult;
	}

	if (dmTmp.dmBitsPerPel != dmOld.dmBitsPerPel)
	{
		dmNew.dmBitsPerPel = dmTmp.dmBitsPerPel;		
		lResult = SetNewDisplay(dmOld,dmNew) + lResult;
	}

	if (dmTmp.dmDisplayFrequency != dmOld.dmDisplayFrequency)
	{
		dmNew.dmDisplayFrequency = dmTmp.dmDisplayFrequency;
		lResult = SetNewDisplay(dmOld,dmNew) + lResult;
	}
	return (!lResult)? S_OK:S_FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL GetPageFileDrive(PTSTR StartDrive,DWORD AppealSize,TCHAR ptzDrive[1])
{
	TCHAR tzDrivesList[MAX_NAME];
	BOOL FindFixedDrive = false;
	BOOL FindThisDrive = false;

	GetLogicalDriveStrings(MAX_NAME, tzDrivesList);
	UStrToUpper(StartDrive);
	UStrToUpper(tzDrivesList);

	for (TCHAR* p = tzDrivesList; *p; p += UStrLen(p) + 1)
	{
		if (GetDiskFree(p) > (AppealSize + MD5Str_Len))
		{
			switch (MyGetDriveType(p))
			{
			case DRIVE_REMOVABLE:
				{
					ptzDrive[0] = *p;
					break;
				}
			case DRIVE_FIXED: //记录符合条件的硬盘分区
				{
					ptzDrive[1] = *p;
					FindFixedDrive = true;
					if (*p >= *StartDrive)
					{
						FindThisDrive = true;
					}
					break;
				}
			}
		}
		if (FindThisDrive)
		{
			break;
		}
	}

	return (FindThisDrive ? FindThisDrive: FindFixedDrive);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Set page file
#include <NTSecAPI.h>
#define REG_PageFile TEXT("PagingFiles")
HRESULT Page(PTSTR ptzCmd)
{
	// Parse size
	UINT uMin = 0;
	UINT uMax = 0;
	PTSTR p = UStrChr(ptzCmd, ' ');
	if (p)
	{
		*p++ = 0;
		uMin = UStrToInt(p);
		p = UStrChr(p, ' ');
		if (p)
		{
			*p++ = 0;
			uMax = UStrToInt(p);
		}
	}
	if (uMax < uMin)
	{
		uMax = uMin;
	}

	if (GetFreeMemo() < uMax)
	{
		// Get DOS device name for page file
		TCHAR tzDrive[MD5Str_Len];
		TCHAR tzDos[MAX_PATH];
		TCHAR tzFile[MAX_PATH];
		tzDrive[0] = ptzCmd[0]; tzDrive[1] = ptzCmd[1]; tzDrive[2] = 0;

		//　以下是增加的功能，按条件查找硬盘分区，尽可能避开移动硬盘

		TCHAR ptzDrive[1] = {0};
		tzDrive[0] = ptzDrive[GetPageFileDrive(tzDrive,uMax,ptzDrive)];

		////////////加入的代码到此结束

		if (tzDrive) //找到分区
		{

			ptzCmd[0] = tzDrive[0];

			UStrCopy(tzFile, ptzCmd + 2);
			QueryDosDevice(tzDrive, tzDos, MAX_PATH);
			UStrCat(tzDos, tzFile);

			WCHAR wzPath[MAX_PATH];
			UStrToWStr(wzPath, tzDos, MAX_PATH);

			UNICODE_STRING sPath;
			sPath.Length = UWStrLen(wzPath) * sizeof(WCHAR);
			sPath.MaximumLength = sPath.Length + sizeof(WCHAR);
			sPath.Buffer = wzPath;

			// Fill size param
			ULARGE_INTEGER ulMax, ulMin;
			HRESULT hResult = (1024 * 1024);
			ulMin.QuadPart = uMin * hResult;
			ulMax.QuadPart = uMax * hResult;

			// Get function address
			typedef NTSTATUS (NTAPI* PNtCreatePagingFile)(PUNICODE_STRING sPath, PULARGE_INTEGER puInitSize, PULARGE_INTEGER puMaxSize, ULONG uPriority);
			PNtCreatePagingFile NtCreatePagingFile = (PNtCreatePagingFile) GetProcAddress(GetModuleHandle(TEXT("NTDLL")), "NtCreatePagingFile");
			if (!NtCreatePagingFile)
			{
				return E_FAIL;
			}

			// Create page file
			Priv(SE_CREATE_PAGEFILE_NAME);
			hResult = NtCreatePagingFile(&sPath, &ulMin, &ulMax, 0);
			if (hResult == S_OK)
			{
				// Log to Windows Registry
				TCHAR tzStr[MAX_PATH];
				DWORD i = sizeof(tzStr);
				if (SHGetValue(HKEY_LOCAL_MACHINE, REG_MemMgr, REG_PageFile, NULL, tzStr, &i) != S_OK)
				{
					i = 0;
				}
				else
				{
					i = (i / sizeof(TCHAR)) - 1;
				}
				i += UStrPrint(tzStr + i, TEXT("%s %d %d"), ptzCmd, uMin, uMax);
				tzStr[++i] = 0;
				SHSetValue(HKEY_LOCAL_MACHINE, REG_MemMgr, REG_PageFile, REG_MULTI_SZ, tzStr, i * sizeof(TCHAR));
			}		
			return hResult;
		}
		else return E_FAIL;
	}
	else
	{
		return S_OK;
	}	
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Start service
HRESULT Serv(PTSTR ptzCmd)
{
	BOOL bResult = FALSE;
	SC_HANDLE hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hManager)
	{
		BOOL bStop = (*ptzCmd == '!');
		if (bStop)
		{
			ptzCmd++;
		}

		SC_HANDLE hService = OpenService(hManager, ptzCmd, SERVICE_START | SERVICE_STOP);
		if (hService)
		{			
			if (bStop)
			{
				SERVICE_STATUS sStatus;
				bResult = ControlService(hService, SERVICE_CONTROL_STOP, &sStatus);
			}
			else
			{
				bResult = StartService(hService, 0, NULL);
			}

			CloseServiceHandle(hService);
		}
		CloseServiceHandle(hManager);
	}
	return bResult ? S_OK : S_FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
LONG GetNewSize(PTSTR ptzCmd,DWORD MIN, DWORD MAX, DWORD DefSize)
{    
	DWORD part_mem = 0, min_mem = 0, max_mem = 0, NewSize = 0, FreeSize=0,FreeMEM=0;
	UStrToUpper(ptzCmd);
	DWORD AvailPhys=GetFreeMemo();

	PTSTR temp = UStrChr(ptzCmd, 'P');
	if (temp) 
	{
		part_mem = UStrToInt(++temp);
		if (part_mem<1)
		{
			part_mem = 1;
		}
		if (part_mem > 50)
		{
			part_mem = 50;
		}
		NewSize = (DWORD) (part_mem * AvailPhys / 100); //可用内存的 P%
	}

	temp = UStrChr(ptzCmd, 'L');
	if (temp)
	{
		min_mem = UStrToInt(++temp); //最小值
	}
	if (min_mem>0)
	{
		if (min_mem<MIN)
		{
			min_mem=MIN;
		}
		if (min_mem>MAX)
		{
			min_mem=MAX;
		}
	}

	temp = UStrChr(ptzCmd, 'H');
	if (temp)
	{
		max_mem = UStrToInt(++temp);  //最大值
	}
	if (max_mem>0)
	{
		if (max_mem<MIN)
		{
			max_mem=MIN;
		}
		if (max_mem>MAX)
		{
			max_mem=MAX;
		}
	}

	if (max_mem<min_mem) //交换
	{
		DWORD tmpval = min_mem;
		min_mem = max_mem;
		max_mem = tmpval;
	}

	//检查并修正不合理的值
	if ((max_mem > 0) && ((NewSize > max_mem) || (NewSize == 0)))
	{
		NewSize = max_mem;
	}
	if ((min_mem > 0) && (NewSize < min_mem))
	{
		NewSize = min_mem;
	}
	if (NewSize==0)
	{
		NewSize = DefSize;
	}

	return NewSize;
}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// FBWF setting
#define FBWFKEY TEXT("SYSTEM\\ControlSet001\\Services\\FBWF")
#define FBWFVALUE TEXT("WinPECacheThreshold")
#define FBWFSERVICE TEXT("FBWF")
HRESULT Fbwf(PTSTR ptzCmd)
{		
	DWORD NewSize = GetNewSize(ptzCmd,MD5Str_Len,1024,32);
	SHSetValue(HKEY_LOCAL_MACHINE, FBWFKEY, FBWFVALUE, REG_DWORD, &NewSize, sizeof(NewSize));
	return Serv(FBWFSERVICE);
}
///////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Test string equal
template <typename T1, typename T2> inline BOOL TStrEqual(T1 ptStr1, T2 ptStr2, UINT uLen)
{
	for (UINT i = 0; i < uLen; i++)
	{
		if (!UChrEqual((TCHAR)ptStr1[i], (TCHAR)ptStr2[i]))
		{
			return FALSE;
		}
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Search string
template <typename T1, typename T2> PVOID TStrStr(T1 ptStr1, T2 ptStr2, UINT uLen)
{
	for (T1 p = ptStr1; *p; p++)
	{
		if (TStrEqual(p, ptStr2, uLen))
		{
			return p;
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lookup device ID from INFs
PCTSTR DevGetInf(PCTSTR ptzDevID, PCTSTR ptzSrcInf)
{
	PVOID pvFile = UFileLoad(ptzSrcInf, NULL, NULL);
	if (pvFile)
	{
		BOOL bASCII = ((PBYTE) pvFile)[3];
		for (ptzDevID++; ptzDevID[-1]; ptzDevID += UStrLen(ptzDevID) + 2)
		{
			if (bASCII ? TStrStr((PSTR) pvFile, ptzDevID, ptzDevID[-1]) : TStrStr((PWSTR) pvFile, ptzDevID, ptzDevID[-1]))
			{
				TCHAR tzPath[MAX_PATH];
				UStrPrint(tzPath, TEXT("%s\r\n"), ptzSrcInf);
				XLog(IDS_FoundDriver, ptzDevID, tzPath);
				UMemFree(pvFile);
				return ptzDevID;
			}
		}
		UMemFree(pvFile);
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update device driver
#ifdef _UNICODE
#define STR_UpdateDriverForPlugAndPlayDevices "UpdateDriverForPlugAndPlayDevicesW"
#else
#define STR_UpdateDriverForPlugAndPlayDevices "UpdateDriverForPlugAndPlayDevicesA"
#endif
typedef BOOL (WINAPI* PUPNP)(HWND hWnd, PCTSTR ptzDevID, PCTSTR ptzPath, DWORD dwFlags, PBOOL pbReboot);
BOOL DevIns(PCTSTR ptzDevID, PCTSTR ptzInfPath, DWORD dwForce = 3)
{
	BOOL bResult = FALSE;
	HMODULE hLib = LoadLibrary(TEXT("NewDev.dll"));
	if (hLib)
	{
		// Install INF
		PUPNP p = (PUPNP) GetProcAddress(hLib, STR_UpdateDriverForPlugAndPlayDevices);
		if (p)
		{
			BOOL bReboot = FALSE;
			bResult = p(NULL, ptzDevID, ptzInfPath, dwForce, &bReboot);
		}
		FreeLibrary(hLib);
	}
	return bResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Install driver from DIR
#include <SetupAPI.h>
#pragma comment(lib, "SetupAPI.lib")
UINT DevDir(PCTSTR ptzDir, PCTSTR ptzDevID, PCTSTR ptzClass)
{
	TCHAR tzPath[MAX_NAME];
	if (ptzDir[0] == '\\')
	{
		ptzDir++;
		TCHAR tzDrives[MAX_NAME];
		GetLogicalDriveStrings(MAX_NAME, tzDrives);
		for (PTSTR p = tzDrives; *p; p += UStrLen(p) + 1)
		{
			UStrPrint(tzPath, TEXT("%s%s"), p, ptzDir);
			DevDir(tzPath, ptzDevID, ptzClass);
		}
		return S_OK;
	}

	WIN32_FIND_DATA fd;
	UStrPrint(tzPath, TEXT("%s\\INF\\*.INF"), ptzDir);
	HANDLE hFind = FindFirstFile(tzPath, &fd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return ERROR_FILE_NOT_FOUND;
	}

	do
	{
		UStrPrint(tzPath, TEXT("%s\\INF\\%s"), ptzDir, fd.cFileName);
		if (ptzClass)
		{
			GUID idClass;
			TCHAR tzClass[MAX_NAME];
			SetupDiGetINFClass(tzPath, &idClass, tzClass, MAX_NAME, NULL);
			if (UStrCmpI(tzClass, ptzClass))
			{
				continue;
			}
		}
		PCTSTR p = DevGetInf(ptzDevID, tzPath);
		if (p)
		{
			DevIns(p, tzPath, 0);
		}
	}
	while (FindNextFile(hFind, &fd));

	FindClose(hFind);
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Extract driver from CAB
PTSTR g_ptzDevInf = NULL;
UINT CALLBACK DevCab(PVOID pvContext, UINT uMsg, UINT_PTR upParam1, UINT_PTR upParam2)
{
	static UINT s_uExtract = 0;

	if (uMsg == SPFILENOTIFY_FILEINCABINET)
	{
		PTSTR ptzTarget = ((FILE_IN_CABINET_INFO*) upParam1)->FullTargetName;
		PCTSTR ptzName = ((FILE_IN_CABINET_INFO*) upParam1)->NameInCabinet;

		PCTSTR p = UStrRChr(ptzName, '\\');
		if (p)
		{
			ptzName = p + 1;
		}

		// Extract INF or driver file
		p = ptzName + UStrLen(ptzName) - 4;
		if (UStrCmpI(p, TEXT(".INF")) == 0)
		{
			p = TEXT("%SystemRoot%\\INF\\");
		}
		else if (s_uExtract)
		{
			if (UStrCmpI(p, TEXT(".SYS")) == 0)
			{
				p = TEXT("%SystemRoot%\\SYSTEM32\\DRIVERS\\");
			}
			else
			{
				p = TEXT("%SystemRoot%\\SYSTEM32\\");
			}
		}
		else
		{
			// Skip
			return FILEOP_SKIP;
		}

		ExpandEnvironmentStrings(p, ptzTarget, MAX_PATH);
		UStrCat(ptzTarget, ptzName);
		UStrRep(ptzTarget, '#', '\\');
		UDirCreate(ptzTarget);
		return FILEOP_DOIT;
	}
	else if (uMsg == SPFILENOTIFY_FILEEXTRACTED)
	{
		PCTSTR ptzTarget = ((FILEPATHS*) upParam1)->Target;
		if (UStrCmpI(ptzTarget + UStrLen(ptzTarget) - 4, TEXT(".INF")))
		{
			// Not INF
			s_uExtract++;
			return NO_ERROR;
		}

		// Get Device from INF
		PCTSTR ptzDevID = DevGetInf((PCTSTR) pvContext, ptzTarget);
		if (ptzDevID)
		{
			// Found Driver
			s_uExtract = 1;
			do {*g_ptzDevInf++ = *ptzDevID;} while (*ptzDevID++);
			do {*g_ptzDevInf++ = *ptzTarget;} while (*ptzTarget++);
			return NO_ERROR;
		}

		// Delete INF
		if (s_uExtract != 1)
		{
			// Driver has been extracted completely.
			s_uExtract = 0;
			UFileDelete(ptzTarget);
		}
	}
	return NO_ERROR;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Install device driver
#if (_MSC_VER < 1300)
#define DN_HAS_PROBLEM 0x00000400
#define CM_PROB_NOT_CONFIGURED 0x00000001
typedef DWORD (WINAPI* PCM_Get_DevNode_Status)(OUT PULONG pulStatus, OUT PULONG pulProblemNumber, IN  DWORD dnDevInst, IN  ULONG ulFlags);
#else
#include <CfgMgr32.h>
#endif
#define MAX_DevID 2048
#define NUM_DevID ((UINT) ((PBYTE) p - (PBYTE) tzDevID))
HRESULT Devi(PTSTR ptzCmd)
{
	// Lookup device
	HDEVINFO hDev = SetupDiGetClassDevs(NULL, NULL, 0, DIGCF_ALLCLASSES);
	if (hDev == INVALID_HANDLE_VALUE)
	{
		return E_FAIL;
	}

	// Build SPDRP_HARDWAREID list
	TCHAR tzDevID[MAX_DevID];
	PTSTR p = tzDevID + 1;
	SP_DEVINFO_DATA sdDev = {sizeof(SP_DEVINFO_DATA)};
	for (UINT i = 0; (NUM_DevID < MAX_DevID) && SetupDiEnumDeviceInfo(hDev, i, &sdDev); i++)
	{
#if (_MSC_VER < 1300)
		PCM_Get_DevNode_Status CM_Get_DevNode_Status = (PCM_Get_DevNode_Status) GetProcAddress(GetModuleHandle(TEXT("SetupAPI")), "CM_Get_DevNode_Status");
		if (CM_Get_DevNode_Status)
#endif
		{
			// Exclude configured device
			ULONG uProblem = 0;
			ULONG uStatus = DN_HAS_PROBLEM;
			CM_Get_DevNode_Status(&uStatus, &uProblem, sdDev.DevInst, 0);
			if (uProblem != CM_PROB_NOT_CONFIGURED)
			{
#ifndef _DEBUG
				continue;
#endif
			}
		}

		// Get device ID
		if (SetupDiGetDeviceRegistryProperty(hDev, &sdDev, SPDRP_HARDWAREID, NULL, (PBYTE) p, MAX_DevID - NUM_DevID, NULL) && UStrCmpNI(p, TEXT("ACPI"), 4))
		{
			XLog(IDS_FoundDevice, p);

			// Trim some stuff for quick search
			UINT j = 0;
			for (UINT k = 0; p[j]; j++)
			{
				if ((p[j] == '&') && (++k == 2))
				{
					break;
				}
			}
			p[-1] = j;
			for (p += j; *p; p++);
			p += 2;
		}
	}
	p[-1] = 0;

	SetupDiDestroyDeviceInfoList(hDev);
	if (tzDevID[0] == 0)
	{
		// No device
		return ERROR_NO_MATCH;
	}

	// Parse param
	BOOL bInstall = (ptzCmd[0] == '$');
	if (bInstall)
	{
		ptzCmd++;
	}
	PTSTR ptzClass = UStrChr(ptzCmd, ',');
	if (ptzClass)
	{
		*ptzClass++ = 0;
	}

	if (UStrCmpI(ptzCmd + UStrLen(ptzCmd) - 4, TEXT(".CAB")))
	{
		// Lookup driver from directory
		return DevDir(ptzCmd, tzDevID, ptzClass);
	}
	else
	{
		// Lookup CAB file
		TCHAR tzDevInf[MAX_PATH * MD5Str_Len];
		g_ptzDevInf = tzDevInf;
		HRESULT hResult = SetupIterateCabinet(ptzCmd, 0, (PSP_FILE_CALLBACK) DevCab, tzDevID) ? S_OK : E_FAIL;
		if (bInstall)
		{
			for (PTSTR p = tzDevInf; p < g_ptzDevInf; p += UStrLen(p) + 1)
			{
				PTSTR ptzDevID = p;
				p += UStrLen(p) + 1;
				DevIns(ptzDevID, p);
			}
		}
		return hResult;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Delete file or directory
HRESULT File(PTSTR ptzCmd)
{	
	SHFILEOPSTRUCT so = {0};
	so.pFrom = ptzCmd;
	so.wFunc = FO_DELETE;
	so.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI| FOF_SILENT;	

	PTSTR p = ptzCmd;
	for (; *p; p++)
	{
		if (*p == '>')
		{
			if (p[-1] == '=')
			{
				p[-1] = 0;
				so.pTo = p + 1;
				so.wFunc = FO_COPY;
			}
			else if (p[-1] == '-')
			{
				p[-1] = 0;
				so.pTo = p + 1;
				so.wFunc = FO_MOVE;
			}
			*p = 0;
		}
	}
	p[1] = 0;

	return SHFileOperation(&so);
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Create shortcut
HRESULT Link(PTSTR ptzCmd)
{
	// Parse Shortcut,Target,Param,IconPath,IconIndex
	PTSTR ptzTarget = UStrChr(ptzCmd, ',');
	if (ptzTarget == NULL)
	{
		return ERROR_PATH_NOT_FOUND;
	}

	INT iIcon = 0;
	PTSTR ptzIcon = NULL;

	*ptzTarget++ = 0;

	PTSTR ptzParam = UStrChr(ptzTarget, ',');
	if (ptzParam)
	{
		*ptzParam++ = 0;
		ptzIcon = UStrChr(ptzParam, ',');
		if (ptzIcon)
		{
			*ptzIcon++ = 0;
			PTSTR ptzIndex = UStrChr(ptzIcon, ',');
			if (ptzIndex)
			{
				*ptzIndex++ = 0;
				iIcon = UStrToInt(ptzIndex);
			}
		}
	}

	// Search target
	if (*ptzCmd == '*')
	{
		ptzCmd++;
	}
	else
	{
		TCHAR tzTarget[MAX_PATH];
		if (SearchPath(NULL, ptzTarget, NULL, MAX_PATH, tzTarget, NULL))
		{
			ptzTarget = tzTarget;
		}
		else if (!UDirExist(ptzTarget))
		{
			return ERROR_PATH_NOT_FOUND;
		}
	}

	// Create shortcut
	IShellLink *pLink;
	CoInitialize(NULL);
	HRESULT hResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &pLink);
	if (hResult == S_OK)
	{
		IPersistFile *pFile;
		hResult = pLink->QueryInterface(IID_IPersistFile, (PVOID *) &pFile);
		if (hResult == S_OK)
		{
			if (*ptzCmd == '!')
			{
				ptzCmd++;
				hResult = pLink->SetShowCmd(SW_SHOWMINIMIZED);
			}

			// Shortcut settings
			hResult = pLink->SetPath(ptzTarget);
			hResult = pLink->SetArguments(ptzParam);
			hResult = pLink->SetIconLocation(ptzIcon, iIcon);

			if (UDirSplitPath(ptzTarget) != ptzTarget)
			{
				hResult = pLink->SetWorkingDirectory(ptzTarget);
			}

			// Save link
			WCHAR wzLink[MAX_PATH];
			UStrCat(ptzCmd, TEXT(".LNK"));
			UStrToWStr(wzLink, ptzCmd, MAX_PATH);
			UDirCreate(ptzCmd);
			hResult = pFile->Save(wzLink, FALSE);

			pFile->Release();
		}
		pLink->Release();
	}

	CoUninitialize();
	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Synthesizes a keystroke
HRESULT Send(PTSTR ptzCmd)
{
	INT i = 0;
	PTSTR p = ptzCmd;
	do
	{
		if ((*p == ',') || (*p == 0))
		{
			i = UStrToInt(ptzCmd);
			if (*(p - 1) != '^')
			{
				keybd_event(i, 0, 0, 0);
			}
			if (*(p - 1) != '_')
			{
				keybd_event(i, 0, KEYEVENTF_KEYUP, 0);
			}
			ptzCmd = p + 1;
		}
	}
	while (*p++);
	return i ? S_OK : S_FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Find process
//指定进程名，参数为空时返回父进程ID
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
				if (UStrCmpNI(pe.szExeFile, ptzCmd, UStrLen(ptzCmd)) == 0)
				{
					hResult= pe.th32ProcessID;				
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
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT Kill(PTSTR ptzCmd)
{	
	if (!ptzCmd)
	{
		return S_OK;
	}

	HRESULT hResult = S_FALSE;
	DWORD ProcessID = 0;
	do
	{
		ProcessID = FindProc(ptzCmd);
		if (ProcessID) 
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
			if (hProcess)
			{
				if (TerminateProcess(hProcess, 0))
				{
					hResult = S_OK;
				}
				CloseHandle(hProcess);
			}
		}
	}
	while (ProcessID > 0);

	return hResult;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT Boot(BOOL bReboot)
{
	DWORD_PTR dwResult;
	SendMessageTimeout(HWND_BROADCAST, WM_QUERYENDSESSION, 0, 0, 0, 2000, &dwResult);
	SendMessageTimeout(HWND_BROADCAST, WM_ENDSESSION, 0, 0, 0, 2000, &dwResult);

	//SendMessageTimeout(HWND_BROADCAST, WM_CLOSE, 0, 0, 0, 2000, &dwResult);

	SendMessageTimeout(HWND_BROADCAST, WM_DESTROY, 0, 0, 0, 2000, &dwResult);

	// Get function address
	typedef DWORD (NTAPI *PNtShutdownSystem)(DWORD dwAction);
	PNtShutdownSystem NtShutdownSystem = (PNtShutdownSystem) GetProcAddress(GetModuleHandle(TEXT("NTDLL")), "NtShutdownSystem");
	if (!NtShutdownSystem)
	{
		return E_FAIL;
	}
	// Shutdown
	Priv(SE_SHUTDOWN_NAME);
	return NtShutdownSystem(bReboot ? 1: 2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown
HRESULT Shut(PTSTR ptzCmd)
{
	return Boot(((ptzCmd[0] == 'R') || (ptzCmd[0] == 'r')) || (g_iShowCmd & EWX_REBOOT));
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Call DLL function
typedef HRESULT (WINAPI *PROC1)(PVOID pv0);
typedef HRESULT (WINAPI *PROC2)(PVOID pv0, PVOID pv1);
typedef HRESULT (WINAPI *PROC3)(PVOID pv0, PVOID pv1, PVOID pv2);
typedef HRESULT (WINAPI *PROC4)(PVOID pv0, PVOID pv1, PVOID pv2, PVOID pv3);
HRESULT WINAPI CallDll(PTSTR ptzCmd)
{
	UINT uArg = 0;
	PTSTR ptzArg[4];
	HRESULT hResult = E_NOINTERFACE;
	PTSTR ptzProc = UStrChr(ptzCmd, ',');
	if (ptzProc)
	{
		*ptzProc++ = 0;
		for (PTSTR p = ptzProc; (uArg < 4) && (p = UStrChr(p, ',')); uArg++)
		{
			*p++ = 0;
			ptzArg[uArg] = p;
			if (*p == '#')
			{
				ptzArg[uArg] = (PTSTR)(INT_PTR)UStrToInt(p + 1);
			}
		}
	}
	else
	{
		ptzProc = TEXT("DllRegisterServer");
	}

	HMODULE hLib = LoadLibrary(ptzCmd);
	if (hLib)
	{
		CHAR szProc[MAX_NAME];
		UStrToAStr(szProc, ptzProc, MAX_NAME);
		PROC f = GetProcAddress(hLib, szProc);
		if (f)
		{
			switch (uArg)
			{
			case 0: hResult = f(); break;
			case 1: hResult = ((PROC1) f)(ptzArg[0]); break;
			case 2: hResult = ((PROC2) f)(ptzArg[0], ptzArg[1]); break;
			case 3: hResult = ((PROC3) f)(ptzArg[0], ptzArg[1], ptzArg[2]); break;
			case 4: hResult = ((PROC4) f)(ptzArg[0], ptzArg[1], ptzArg[2], ptzArg[3]); break;
			}
		}
		FreeLibrary(hLib);
	}

	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void InitDriveList(UINT pzDriveType)
{
	UINT DriveNumber = 0;
	TCHAR pzDrive[MAX_NAME];
	TCHAR tzDrives[MAX_NAME];
	TCHAR pzType[MAX_NAME] = {0};
	if (pzDriveType == DRIVE_CDROM)
	{
		UStrCopy(pzType,TEXT("CDROM"));
	}

	if (pzDriveType == DRIVE_REMOVABLE)
	{
		UStrCopy(pzType,TEXT("USB"));
	}

	if (pzType[0])
	{
		GetLogicalDriveStrings(MAX_NAME, tzDrives);
		TCHAR pzThisDrive[3];
		pzThisDrive[1] = ':';
		pzThisDrive[2] = 0;
		for (PTSTR p = tzDrives; *p; p += UStrLen(p) + 1)
		{
			if (MyGetDriveType(p) == pzDriveType)
			{
				DriveNumber++;
				pzThisDrive[0] = *p;
				UStrPrint(pzDrive, TEXT("%s%d=%s"),pzType,DriveNumber,pzThisDrive);
				SetEnvironment(pzDrive,TRUE,FALSE);
				if (DriveNumber == 1)
				{
					UStrPrint(pzDrive, TEXT("%s=%s"),pzType,pzThisDrive);
					SetEnvironment(pzDrive,TRUE,FALSE);
				}
			}
		}

		if (DriveNumber)
		{
			UStrPrint(pzDrive, TEXT("%s0=%d"),pzType,DriveNumber);
			SetEnvironment(pzDrive,TRUE,FALSE);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Hook ExitWindows to execute a command
VOID HookProcess(HANDLE hProcess)
{
	__asm
	{
		// BOOL WINAPI WriteProcessMemory(HANDLE hProcess, PVOID pvBaseAddress, PVOID pvBuffer, DWORD dwSize, PDWORD pdwNumberOfBytesWritten);
		// Write code to target
		PUSH    NULL;
		MOV     EAX, OFFSET _End;
		SUB     EAX, OFFSET _Code;
		PUSH    EAX;
		PUSH    OFFSET _Code;
		PUSH    ExitWindowsEx;
		PUSH    hProcess;
		CALL    DWORD PTR [WriteProcessMemory];

		// Write True WinExec address to target
		PUSH    NULL;
		PUSH    4;
		LEA     EAX, WinExec;
		PUSH    EAX;
		MOV     EAX, OFFSET _WinExec;
		SUB     EAX, OFFSET _Code;
		ADD     EAX, ExitWindowsEx;
		PUSH    EAX;
		PUSH    hProcess;
		CALL    DWORD PTR [WriteProcessMemory];

		// Return
		JMP		_End;

		// Target code (Call WinExec to execute a command), address independent
		//BOOL WINAPI ExitWindowsEx(UINT uFlags, DWORD dwReason)
		//UINT WINAPI WinExec(PCSTR pszCmdLine, UINT uCmdShow)
_Code:
		MOV     EAX, [ESP + 4];	// Get uFlags
		PUSH    EAX;		// Push  uCmdShow
		CALL    _GetWinExec;	// Push _WinExec
_WinExec:
		_EMIT   0;
		_EMIT   0;
		_EMIT   0;
		_EMIT   0;
_GetWinExec:
		POP     EAX;			// Get _WinExec
		CALL    _Call;			// Push pszCmdLine
		_EMIT   'P';
		_EMIT   'E';
		_EMIT   'C';
		_EMIT   'M';
		_EMIT   'D';
		_EMIT   '.';
		_EMIT   'E';
		_EMIT   'X';
		_EMIT   'E';
		_EMIT   ' ';
		_EMIT   'S';
		_EMIT   'H';
		_EMIT   'U';
		_EMIT   'T';
		_EMIT   0;
_Call:
		CALL    [EAX];			// Call WinExec
		RET     8;
_End:
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execute command
#define RunsKey TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
HRESULT Exec(PTSTR ptzCmd)
{
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = TEXT("WinSta0\\Default");
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_NORMAL;

	BOOL bWait = FALSE;
	BOOL bHook = FALSE;
	BOOL bRunKey = FALSE;
	while (TRUE)
	{
		if (*ptzCmd == '!')
		{
			si.wShowWindow = SW_HIDE;
		}
		else if (*ptzCmd == '@')
		{
			si.lpDesktop = TEXT("WinSta0\\WinLogon");
		}
		else if (*ptzCmd == '=')
		{
			bWait = TRUE;
		}
		else if (*ptzCmd == '$')
		{
			bHook = TRUE;
		}
		else if (*ptzCmd == '&')
		{
			bRunKey = TRUE;
		}
		else
		{
			break;
		}
		ptzCmd++;
	}	

	if (bRunKey)
	{
		PTSTR ptzName = UStrRChr(ptzCmd, '\\');
		ptzName = ptzName ? (ptzName + 1) : ptzCmd;
		HKEY hKey = bWait ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
		return SHSetValue(hKey, RunsKey, ptzName, REG_SZ, ptzCmd, UStrLen(ptzCmd) * sizeof(TCHAR));
	}

	BOOL bResult = CreateProcess(NULL, ptzCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (bResult)
	{
		if (bHook)
		{
			Sleep(1500);
			HookProcess(pi.hProcess);
			if (!g_hHook)
			{
				//PostMessage(MainWindowHwnd, FindShellMsg, 1, 0);
				SendMessage(MainWindowHwnd, FindShellMsg, 1, 0);
			}
			if (TrayIcon)
			{
				//PostMessage(MainWindowHwnd, FindShellMsg, 2, 0);
				SendMessage(MainWindowHwnd, FindShellMsg, 2, 0);
			}			
		}
		SetProcessWorkingSetSize(CurrentProcess,-1,-1);
		if (bWait)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return bResult ? S_OK : S_FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ctrl+Alt+Del handler
TCHAR TASKMANAGER[] = TEXT("TaskMgr.exe");
LRESULT CALLBACK InitProc(INT iCode, WPARAM wParam, LPARAM lParam)
{
	if ((iCode == HC_ACTION) && (wParam == WM_KEYDOWN) && (((LPKBDLLHOOKSTRUCT) lParam)->vkCode == VK_DELETE))
	{
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000))
		{
			Exec(TASKMANAGER);
			return TRUE;
		}
	}
	return CallNextHookEx(g_hHook, iCode, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
#define pzStrDLLInstall TEXT(".DLL,DllInstall,#1,I")
HRESULT Init(PTSTR ptzCmd)
{
	HRESULT hResult =NULL;
	CheckMe = CheckMERes();
	if (CheckMe)
	{
		TCHAR tzPath[MAX_NAME];

		UStrCopy(tzPath,TEXT(""));//
		hResult = hResult + SetEnvironment(tzPath,FALSE,TRUE);

		UStrPrint(tzPath, TEXT("SHELL32%s"),pzStrDLLInstall);
		hResult = hResult + CallDll(tzPath);

		UStrPrint(tzPath, TEXT("BROWSEUI%s"),pzStrDLLInstall);
		hResult = hResult + CallDll(tzPath);

		UStrPrint(tzPath, TEXT("SHDOCVW%s"),pzStrDLLInstall);
		hResult = hResult + CallDll(tzPath);

		if (StrStrI(ptzCmd, TEXT("C"))) //初始化光驱
		{
			InitDriveList(DRIVE_CDROM);
		}

		if (StrStrI(ptzCmd, TEXT("I"))) //安装托盘图标
		{
			BOOL Icon = TRUE;
			SHSetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("TrayIcon"), REG_DWORD, &Icon, sizeof(Icon));
		}
		if (StrStrI(ptzCmd, TEXT("K"))) //安装键盘钩子
		{
			BOOL Init = TRUE;
			SHSetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("InitProc"), REG_DWORD, &Init, sizeof(Init));
		}
		if (StrStrI(ptzCmd, TEXT("U"))) //初始化USB磁盘
		{
			InitDriveList(DRIVE_REMOVABLE);
		}
	}
	g_bInit = TRUE;
	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
#define PESHELL TEXT("Shell")
void GetShell(bool DeleteSettingKey)
{		
	if (DeleteSettingKey) 
	{		
		SHSetValue(HKEY_LOCAL_MACHINE, PELOGON,TEXT("SysShell"), REG_SZ, MyShell, UStrLen(MyShell) * sizeof(TCHAR));
		SHDeleteValue(HKEY_LOCAL_MACHINE, PELOGON,PESHELL);
	}
	else
	{	
		BOOL InitProc = FALSE;
		DWORD dwType=REG_SZ;
		DWORD dwSize=MAX_PATH;
		SHGetValue(HKEY_LOCAL_MACHINE,PELOGON, PESHELL,&dwType, &MyShell[0], &dwSize);	
		dwType=REG_DWORD;
		SHGetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("TrayIcon"), &dwType, &TrayIcon, &dwSize);
		SHGetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("InitProc"), &dwType, &InitProc, &dwSize);
		if ((!g_hHook) && (InitProc) && (MainWindowHwnd))
		{
			//PostMessage(MainWindowHwnd, FindShellMsg, 1, 0);
			SendMessage(MainWindowHwnd, FindShellMsg, 1, 0);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void GetKeyModAndKeyCode(PTSTR ptzHotk,UINT &Key_Mod,UINT &Hotkey)
{
	if (StrStrI(ptzHotk, TEXT("ALT")))
	{
		Key_Mod = Key_Mod|MOD_ALT;
	}
	if (StrStrI(ptzHotk, TEXT("CTRL")))
	{
		Key_Mod = Key_Mod|MOD_CONTROL;
	}
	if (StrStrI(ptzHotk, TEXT("SHIFT")))
	{
		Key_Mod = Key_Mod|MOD_SHIFT;
	}
	if (StrStrI(ptzHotk, TEXT("WIN")))
	{
		Key_Mod = Key_Mod|MOD_WIN;
	}
	PTSTR pKeyCode = UStrChr(ptzHotk, '#');
	if (pKeyCode)
	{
		Hotkey = UStrToInt(++pKeyCode);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void RegMenuHotk(void)
{	
	static BOOL bReg = FALSE;
	if (!bReg)
	{
		RegisterHotKey(MainWindowHwnd, 0, MOD_ALT | MOD_CONTROL, 'M');
		UINT Key_Mod = 0;
		UINT Hotkey  = 0;
		TCHAR HotkCode[MAX_NAME];
		LoadString(g_hInst,IDS_Hotk,HotkCode,MAX_NAME);
		PTSTR ptzHotk = UStrTrim(HotkCode);
		PTSTR ptzCmd = UStrChr(ptzHotk, ',');
		if (ptzCmd)
		{
			*ptzCmd++ = 0;
		}

		if (ptzCmd && ptzHotk)
		{
			UStrCopy(HotkCmd,ptzCmd);//
			GetKeyModAndKeyCode(ptzHotk,Key_Mod,Hotkey);		
			RegisterHotKey(MainWindowHwnd,1,Key_Mod,Hotkey);
		}
		bReg = TRUE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void RegHotKey(void)
{
	UINT KeyMod[8] = {0};
	UINT HotKey[8] = {0};
	PTSTR ptzKeyMod;
	PTSTR ptzHotKey;
	DWORD dwType;
	DWORD dwSize;
	TCHAR ptzKeyName[MAX_NAME];	

	for (UINT i=0;i < 8 ;i++)
	{		
		dwType=REG_SZ;
		dwSize=MAX_STR;

		UStrPrint(ptzKeyName, TEXT("HotKey%d"),i+1);
		SHGetValue(HKEY_LOCAL_MACHINE,PELOGON, ptzKeyName,&dwType, &HotKeyCommand[i][0], &dwSize);
		if (HotKeyCommand[i][0])
		{
			ptzKeyMod = UStrChr(HotKeyCommand[i], ',');
			if (ptzKeyMod)
			{
				*ptzKeyMod++ = 0;
				KeyMod[i] = UStrToInt(ptzKeyMod);
				ptzHotKey = UStrChr(ptzKeyMod, ',');
				if (ptzHotKey)
				{
					*ptzHotKey++ = 0;
					HotKey[i] = UStrToInt(ptzHotKey);
				}
			}

			if (HotKey[i]>0)
			{
				BOOL rbReg= RegisterHotKey(MainWindowHwnd,i+2,KeyMod[i], HotKey[i]);

				UStrPrint(ptzKeyName, TEXT("RegHotKey%d"),i+1);
				TCHAR KeyName[MAX_STR];
				UStrPrint(KeyName,
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
					UStrLen(KeyName) * sizeof(TCHAR));

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
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
UINT ToalMenu = 0;
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
	for (int i=0;i<MenuItemCount;i++)
	{
		MenuItemID = GetMenuItemID(hsMenu,i);
		/* 指定条目的菜单ID。
		如条目属于一个弹出式菜单，就返回-1；
		如指定的条目属于一个分隔符（比如一条分隔线）则返回0 */
		switch (MenuItemID)
		{
		case -1:
			sMenu = GetSubMenu(hsMenu,i);
			if (sMenu)
			{
				/*
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask = MIIM_TYPE; 
				mii.fMask = MIIM_ID | MIIM_BITMAP | MIIM_DATA; 
				mii.fType = MFT_STRING;
				mii.cch = MAX_STR;
				mii.dwTypeData = MenuText;
				GetMenuItemInfo(hsMenu,MenuItemID,FALSE,&mii);
				mii.fMask = MIIM_ID | MIIM_BITMAP | MIIM_DATA; 
				mii.hbmpChecked = hBitMap;
				mii.hbmpUnchecked = hBitMap;
				mii.hbmpItem = hBitMap;
				ModifyMenu(hsMenu,MenuItemID,MF_BITMAP|MF_BYCOMMAND|MF_STRING,MenuItemID,MenuText);
				SetMenuItemBitmaps(sMenu,MenuItemID,MF_BITMAP,hBitMap,hBitMap);
				*/
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
			if ((MenuItemID>0) && (MenuItemID !=ID_HIDE_ICON ) && (ToalMenu <128))
			{					
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask = MIIM_TYPE; 
				mii.fType = MFT_STRING;
				mii.cch = MAX_STR;
				mii.dwTypeData = MenuText;
				GetMenuItemInfo(hsMenu,MenuItemID,FALSE,&mii);
				PTSTR ptzCmd = UStrChr(MenuText, ',');
				if (ptzCmd)
				{
					*ptzCmd++ = 0;
					ptzCmd = UStrTrim(ptzCmd);
					TCHAR tzCmd[MAX_PATH];
					ExpandEnvironmentStrings(ptzCmd, tzCmd, MAX_PATH);					
					UStrCopy(MenuCommand[ToalMenu],tzCmd);//
					ToalMenu++;
				}
				PTSTR pMenuText = UStrTrim(MenuText);
				ModifyMenu(hsMenu,MenuItemID,MF_STRING,MenuItemID,pMenuText);
			}
		}		
	}
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
HMENU MyPopupMenu(BOOL Created)
{
	static HMENU hMenu;
	if (Created)
	{
		if (!hMenu)
		{
			hMenu = LoadMenu(g_hInst,MAKEINTRESOURCE(IDR_MENU1));
		}

		if ((!hSubMenu) && (hMenu))
		{
			RegMenuHotk();
			ToalMenu = 0;
			hSubMenu = GetSubMenu(hMenu,0);
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
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
VOID InsTrayIcon(BOOL Install)
{
	if (Install)
	{
		for (UINT i=1;i < 10 ;i++)
		{		
			if (FindProc(EXPLORER))
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
							HICON hIcon = LoadIcon(g_hInst,(LPCWSTR)IDI_ICON1);
							if (!hIcon)
							{
								//hIcon = LoadIcon(g_hInst,(PTSTR)IDI_Main);
								hIcon = LoadIcon(g_hInst,(LPCWSTR)IDI_Main);
							}							
							FpNotify.hIcon = hIcon;
							FpNotify.uCallbackMessage = CM_nTaskIcoMsg;
							UStrCopyN(FpNotify.szTip,MenuTipText,64);
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
	SetProcessWorkingSetSize(CurrentProcess,-1,-1);
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void PopupLogonMenu(HWND hWND)
{
	POINT p;
	GetCursorPos(&p);
	SetForegroundWindow(MainWindowHwnd);
	TrackPopupMenu(hSubMenu,TPM_LEFTBUTTON,p.x,p.y,NULL,hWND,NULL);
	PostMessage(hWND,WM_NULL, 0, 0);
	SetProcessWorkingSetSize(CurrentProcess,-1,-1);
}
//////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY://0x0002
		for (UINT i=0;i < 10 ;i++)
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
			Exec(HotKeyCommand[wParam-2]);
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
			InsTrayIcon(TRUE);
			break;
		}
		break;

	case CM_nTaskIcoMsg: //0x0402
		switch(lParam)
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define szWindowName TEXT("PELogon(Created By Lxl1638)")
#define szWindowClass TEXT("PELogon CallBack Window")
//////////////////////////////////////////////////////////////////////////////////////////
HWND CreateMainWindow(HINSTANCE hInstance/*, int nCmdShow*/)
{
	HWND hResult = NULL;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	CheckMe = FALSE;
	CheckMe = CheckMERes();
	if (CheckMe)
	{
		if (RegisterClassEx(&wcex))
		{
			hResult= CreateWindow(szWindowClass,
				szWindowName, 
				WS_DISABLED|WS_MINIMIZE,
				CW_USEDEFAULT, 0, 
				CW_USEDEFAULT, 0, 
				NULL, NULL, hInstance, NULL);
		}
		TCHAR ChineseMenuTip[MAX_NAME];
		UINT16ToUStr(ChineseMenuTip,AppTrayText,_NumOf(AppTrayText));
		UStrCat(MenuTipText,(PTSTR)((DWORD)ChineseMenuTip*(!NotChineseLangID)));
	}

	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
HRESULT MainThread(void)
{
	while (!MyShell[0])
	{
		GetShell(false);
		Sleep(50);
	}
	PTSTR ShellName = UPathSplitName(MyShell);
	TCHAR tzPath[MAX_PATH];
	UStrPrint(tzPath, TEXT("=$%s"), MyShell);
	if ((MainWindowHwnd) && (!FindProc(ShellName)))
	{		
		TCHAR EMenuTip[MAX_NAME];		
		LoadString(g_hInst,IDS_PECNMD_Mgr,EMenuTip,MAX_NAME);
		UStrCat(MenuTipText,(PTSTR)((DWORD)EMenuTip*NotChineseLangID));
		UStrTrim(MenuTipText);
		//PostMessage(MainWindowHwnd, FindShellMsg, 0, 0);
		SendMessage(MainWindowHwnd, FindShellMsg, 0, 0);
	}
	CheckMe = FALSE;
	CheckMe = CheckMERes();
	while (MyShell[0] && (!FindProc(ShellName)) && CheckMe)
	{
		Exec(tzPath);
		Sleep(200);
	}
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
HRESULT Hotk(PTSTR ptzHotk)
{
	HRESULT hResult=false;	
	if (HotkCount < 8)
	{
		PTSTR ptzCommand = UStrChr(ptzHotk, ',');
		if (ptzCommand)
		{
			*ptzCommand++ = 0;
			UINT Key_Mod = 0;
			UINT Hotkey  = 0;
			GetKeyModAndKeyCode(ptzHotk,Key_Mod,Hotkey);

			TCHAR ptzKeyName[8];
			HotkCount++;
			UStrPrint(ptzKeyName, TEXT("HotKey%d"),HotkCount);

			TCHAR CommandData[MAX_PATH];
			UStrPrint(CommandData, TEXT("%s,%d,%d"),ptzCommand,Key_Mod,Hotkey);

			SHSetValue(HKEY_LOCAL_MACHINE, PELOGON, ptzKeyName, REG_SZ, CommandData, UStrLen(CommandData) * sizeof(TCHAR));
			hResult = S_OK;
		}
	}
	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
DWORD PassEditOldFun;
HRESULT CALLBACK PassEditNewProc(HWND hEditHwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DWORD timestart = 0;
	switch (uMsg)
	{
	case WM_KEYDOWN: //键盘消息
	case WM_KEYUP:
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			if (GetTickCount() - timestart > 1000)
			{
				timestart = GetTickCount();
				PassWordEditEnter = TRUE;
			}
		}
		break;
	}
	return CallWindowProc((WNDPROC)PassEditOldFun,/*PassWordEdit*/hEditHwnd, uMsg, wParam,lParam);

}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//返回字符串MD5密文(支持含WCHAR字符的字符串)
PTSTR UStrToMD5(PTSTR ptzCmd)
{	
	CHAR tzTempBuf[MAX_NAME];
	UStrToAStr(tzTempBuf,ptzCmd,MAX_NAME);
	DWORD i= 0;
	while (tzTempBuf[i])
	{
		i++;
	}	
	PTSTR pMD5Code = ByteToMD5((PBYTE)tzTempBuf,i);
	return pMD5Code;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT Md5c(PTSTR ptzCmd)
{	
	PTSTR pMD5Code = UStrToMD5(ptzCmd);
	TCHAR ptzCaption[MD5Str_Len];
	BOOL hResult = SetClipboardText(pMD5Code);
	if (hResult)
	{
		LoadString(g_hInst,IDS_SaveToClipboard,ptzCaption, MAX_NAME);
	}
	else
	{
		LoadString(g_hInst,IDS_MD5String,ptzCaption, MAX_NAME);
	}
	MessageBox(NULL,pMD5Code,ptzCaption, MB_OK);
	return hResult?S_OK:S_FALSE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
HRESULT Logo(PTSTR);
BOOL GetPassWord(PTSTR PassWordMD5,UINT retry = 3)
{
	HWND PassWordEdit;
	TCHAR PassErrMsg[MAX_PATH];
	RECT tzPassRECT;

	//保存用户 TEXT 命令的参数
	SaveUserTextData(TRUE);
	//=====================================	

	WaitLogo(!s_hLogo); //不存在 Logo 窗口则加载 Logo 窗口

	UINT xEdit = 108;
	UINT yEdit = 22;
	UINT x,y;
	x = (GetSystemMetrics(SM_CXSCREEN) - xEdit)/2 + 100;
	y = (GetSystemMetrics(SM_CYSCREEN) - yEdit)/2 - 40;

	PassWordEditEnter = FALSE;

	PassWordEdit = CreateWindow(TEXT("Edit"),
		TEXT(""),
		WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_PASSWORD,
		x,y, //位置
		xEdit,yEdit, //大小
		s_hLogo, 
		NULL, 
		g_hInst, 
		NULL);

	TCHAR OK_Text[MAX_NAME];
	LoadString(g_hInst,IDS_OK_Text,OK_Text, MAX_NAME);
	fontsize = 14;

	PassButton = CreateWindow(TEXT("Button"),
		OK_Text,
		WS_VISIBLE|WS_CHILD|BS_TEXT|BS_PUSHLIKE,
		x+xEdit+2, y, //位置
		33, 22, //大小
		s_hLogo,
		NULL,
		g_hInst,
		NULL);	

	/*
	HWND hComboBox = CreateWindow(TEXT("COMBOBOX"),
	TEXT(""),
	//WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|CBS_DROPDOWNLIST|CBS_OWNERDRAWFIXED|CBS_OWNERDRAWVARIABLE,
	WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,
	x, //x
	y + 24, //y
	xEdit + 35, //w
	yEdit, //h
	s_hLogo, 
	NULL, 
	g_hInst, 
	NULL);
	SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)TEXT("EXPLORER.EXE"));
	SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)TEXT("XPLORER2.EXE"));
	SendMessage(hComboBox,CB_ADDSTRING,0,(LPARAM)TEXT("CMD.EXE"));
	SendMessage(hComboBox,CB_SELECTSTRING,0,(LPARAM)TEXT("EXPLORER.EXE"));
	//CB_SETCURSEL CBEIF_IMAGE
	*/

	HFONT hButtonFont = CreateFont(fontsize, 0, 0, 0,
		400, 0, 0, 0, 
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		deffontname); //deffontname is "Tahoma"

	HFONT hEditFont = CreateFont(NULL-fontsize, 0, 0, 0, 
		400, 0, 0, 0, 
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		deffontname); //deffontname is "Tahoma"


	if (hButtonFont)
	{
		SendMessage(PassButton, WM_SETFONT, (WPARAM)hButtonFont, 0);
	}	

	if (hEditFont)
	{
		SendMessage(PassWordEdit, WM_SETFONT, (WPARAM)hEditFont, 0);
	}

	SetFocus(PassWordEdit);
	s_rtText.left = x;
	s_rtText.top = y-20;
	s_rtText.right = x+200;
	s_rtText.bottom = y-2;

	TCHAR InputPassWord[MAX_NAME];
	TCHAR OpporText[MAX_NAME];
	LoadString(g_hInst,IDS_InputPassWord,InputPassWord, MAX_NAME);
	LoadString(g_hInst,IDS_OpporText,OpporText, MAX_NAME);
	UStrPrint(PassErrMsg,TEXT("%s%d%s#0*"),InputPassWord,retry,OpporText);	
	Text(PassErrMsg);

	PassEditOldFun = GetWindowLong(PassWordEdit, GWL_WNDPROC);
	SetWindowLong(PassWordEdit,GWL_WNDPROC,(LONG)PassEditNewProc);	

	TCHAR PassErr[MAX_NAME];
	LoadString(g_hInst,IDS_PassWordErr,PassErr, MAX_NAME);
	TCHAR SupperWord[MAX_NAME];
	LoadString(g_hInst,IDS_SupperWord,SupperWord, MAX_NAME);
	PTSTR pSupperWord = UStrTrim(SupperWord);

	UINT i = 0;
	BOOL Pass = FALSE;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (PassWordEditEnter)
		{	
			PassWordEditEnter = FALSE;
			TCHAR PassText[MAX_NAME] = {0};
			PTSTR PassMD5;
			GetWindowText(PassWordEdit,PassText,MAX_NAME);

			if (!UStrCmpNI(PassText,STR_Version,UStrLen(STR_Version)))
			{
				Pass = TRUE;
				break;
			}

			PassMD5 = UStrToMD5(PassText);
			if (!UStrCmpNI(pSupperWord,PassMD5,UStrLen(PassMD5)))
			{
				Pass = TRUE;
				break;
			}

			Pass = (!UStrCmpNI(PassWordMD5,PassMD5,UStrLen(PassMD5)));

			i++;
			if ((i==retry) || Pass)
			{				
				break;
			}

			UStrPrint(PassErrMsg, TEXT("%s%d%s"),PassErr,retry-i,OpporText);
			Text(PassErrMsg);
			SetFocus(PassWordEdit);
		}
	}
	SendMessage(PassButton,WM_CLOSE,NULL,NULL);
	SendMessage(PassWordEdit,WM_CLOSE,NULL,NULL);	
	UStrCopy(PassErrMsg,TEXT(""));

	tzDrawTextDone = FALSE;
	Text(PassErrMsg);

	if (!ExistLogoImage)
	{
		tzPassRECT.left = x - 2;
		tzPassRECT.top = y - 22;
		tzPassRECT.right = x + 202;
		tzPassRECT.bottom = y + 24;
		FillRect(GetDC(s_hLogo), &tzPassRECT, CreateSolidBrush(BackgroundColor));
	}

	if (hButtonFont)
	{
		DeleteObject(hButtonFont);
	}

	if (hEditFont)
	{
		DeleteObject(hEditFont);
	}

	//恢复用户 TEXT 命令的参数
	SaveUserTextData(FALSE);

	return Pass;
}
//////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
void SetMeOnTopMenu(void)
{
	TCHAR Temp[MAX_STR];
	TCHAR MenuText[MAX_NAME];
	TCHAR TopMenu[MAX_PATH];

	GetModuleFileName(NULL,Temp,MAX_PATH);	
	UINT16ToUStr((PTSTR)MenuText,PECMDHelp,_NumOf(PECMDHelp));	
	UStrPrint(TopMenu, TEXT("%s%s"),MenuText,Temp);
	ExpandEnvironmentStrings(TopMenu,Temp,MAX_STR);	

	Link(Temp);	//%StartMenu%\PECMD使用说明,PECMD.EXE
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Get User SHELL
HRESULT Shel(PTSTR ptzCmd)
{
	PTSTR PassWordMD5 = UStrChr(ptzCmd, ',');
	UINT retry = 3;
	CheckMe = FALSE;
	CheckMe = CheckMERes();
	if (PassWordMD5)
	{
		*PassWordMD5++ = 0;
		PTSTR pretry = UStrChr(PassWordMD5, ',');
		if (pretry)
		{
			*pretry++ = 0;
			retry = UStrToInt(pretry);
			if (retry > 9)
			{
				retry = 9;
			}
			if (retry < 3)
			{
				retry = 3;
			}
		}

		PassWordMD5 = UStrTrim(PassWordMD5);

		if (UStrLen(PassWordMD5))
		{
			if (!GetPassWord(PassWordMD5,retry))
			{
				TCHAR msgText[MAX_NAME];
				TCHAR PassWordErrorMsg[MAX_NAME];
				TCHAR PassWordError[MAX_NAME];
				LoadString(g_hInst, IDS_PassWordErrorMsg,PassWordErrorMsg, MAX_NAME);
				LoadString(g_hInst, IDS_PassWordError,PassWordError, MAX_NAME);				
				UStrPrint(msgText,PassWordErrorMsg,retry);
				Boot(MessageBox(s_hLogo,msgText,PassWordError,20) == 6);
			}
		}
	}	

	return (SHSetValue(HKEY_LOCAL_MACHINE,
		PELOGON,
		PESHELL,
		REG_SZ,
		(PTSTR)(firstText*CheckMe*(DWORD)ptzCmd),
		UStrLen(ptzCmd) * sizeof(TCHAR)*(firstText*CheckMe)) != ERROR_SUCCESS);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
HRESULT RegFont(PTSTR tzPath)
{
	WIN32_FIND_DATA fd;
	TCHAR szFullPathName[MAX_PATH];	
	HRESULT hResult = S_FALSE;

	HANDLE hFind = FindFirstFile(tzPath, &fd);
	UDirSplitPath(tzPath);

	if (hFind != INVALID_HANDLE_VALUE)
	{	
		do
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{				
				UStrPrint(szFullPathName, TEXT("%s\\%s"), tzPath,fd.cFileName);
				hResult = AddFontResource(szFullPathName) + hResult;
			}
		}
		while (FindNextFile(hFind,&fd));

		FindClose(hFind);
		SendMessage(HWND_BROADCAST,WM_FONTCHANGE, 0, 0);	
	}

	return (!hResult);
}
//////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//REG FONT
HRESULT Font(PTSTR ptzCmd)
{
	TCHAR tzPath[MAX_NAME];
	HRESULT hResult=ERROR_FILE_NOT_FOUND;

	if (ptzCmd[0] == '\\')
	{
		WCHAR tzDvr = 0;
		PTSTR ptzStart = UStrChr(ptzCmd, ',');
		if (ptzStart)
		{
			*ptzStart++ = 0;
			UStrToUpper(ptzStart);
			if (ptzStart[0]) 
			{
				tzDvr = *ptzStart;
			}
		}

		ptzCmd++;
		TCHAR tzDrives[MAX_NAME];		
		GetLogicalDriveStrings(MAX_NAME, tzDrives);
		for (PTSTR p = tzDrives; *p; p += UStrLen(p) + 1)
		{
			if (*p >= tzDvr)
			{
				UStrPrint(tzPath, TEXT("%s%s\\Fonts"), p, ptzCmd);
				if (UDirExist(tzPath)) 
				{
					UStrCat(tzPath, TEXT("\\*.*"));
					hResult = RegFont(tzPath);
				}
			}
		}

	}
	else
	{
		if (UDirExist(ptzCmd))
		{
			UStrCat(ptzCmd, TEXT("\\*.*"));
		}
		UStrCopy(tzPath,ptzCmd);

		UDirSplitPath(ptzCmd);
		if (UDirExist(ptzCmd))
		{
			hResult = RegFont(tzPath);
		}
	}
	return hResult;
}
////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Set WallPage
HRESULT Wall(PTSTR ptzCmd)
{	
	if (UFileExist(ptzCmd))
	{
		SHSetValue(HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), TEXT("Wallpaper"), REG_SZ, (PTSTR)((DWORD)ptzCmd*firstText), UStrLen(ptzCmd) * sizeof(TCHAR));
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, NULL,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}
////////////////////////////////////


/////////////////////////////////////
// Set NumLock
HRESULT Numk(PTSTR ptzCmd)
{
	bool bState = (UStrToInt(ptzCmd) !=0 );
	BYTE keyState[256];
	GetKeyboardState((LPBYTE)&keyState);
	if( (bState && !(keyState[VK_NUMLOCK] & 1)) || (!bState && (keyState[VK_NUMLOCK] & 1)))
	{
		keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT Runs_Or_User(PTSTR ptzCmd,bool IsRun)
{	
	PTSTR ptzName = NULL;
	ptzName = UStrRChr(ptzCmd, '*');
	if (!ptzName)
	{
		ptzName = UStrChr(ptzCmd, ',');
	}

	if (ptzName)
	{
		*ptzName++ = 0;
		if (IsRun)
		{
			SHSetValue(HKEY_LOCAL_MACHINE, RunsKey, ptzName, REG_SZ, ptzCmd, UStrLen(ptzCmd) * sizeof(TCHAR));
		}
		else
		{			
			SHSetValue(HKEY_LOCAL_MACHINE, UserKey, TEXT("RegisteredOwner"), REG_SZ, ptzCmd, UStrLen(ptzCmd) * sizeof(TCHAR));
			SHSetValue(HKEY_LOCAL_MACHINE, UserKey, TEXT("RegisteredOrganization"), REG_SZ, ptzName, UStrLen(ptzName) * sizeof(TCHAR));

		}
		return S_OK;
	}
	else 
	{
		return S_FALSE;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////
HRESULT Runs(PTSTR ptzCmd)
{
	return Runs_Or_User(ptzCmd,true);
}
///////////////////////////////////////////////////


///////////////////////////////////////////////////
HRESULT User(PTSTR ptzCmd)
{
	return Runs_Or_User(ptzCmd,false);
}
///////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Manager driver
BOOL ManageDriver(HDEVINFO hDev, SP_DEVINFO_DATA& sDev,  DWORD dwAction)
{
	SP_PROPCHANGE_PARAMS sProp;
	sProp.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	sProp.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	sProp.StateChange = dwAction;
	sProp.Scope = DICS_FLAG_CONFIGSPECIFIC;
	sProp.HwProfile = 0;

	return SetupDiSetClassInstallParams(hDev, &sDev, &sProp.ClassInstallHeader, sizeof(sProp)) &&
		SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDev, &sDev);
}
//////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	RAMDISK_NO_DRIVER=0,
	RAMDISK_ERRORHANDLE,
	RAMDISK_DISABLED,
	RAMDISK_OK
};
#define RAMKEY   TEXT("System\\CurrentControlSet\\Services\\Ramdriv\\Parameters")
#define RAMVALUE TEXT("DiskSize")
#define ERRORHANDLE	{\
	if (Ramdev!=INVALID_HANDLE_VALUE) SetupDiDestroyDeviceInfoList(Ramdev);	\
	return FALSE; \
	}
GUID Ramdrive = { 0xFFA1C341, 0x4539, 0x11D3, { 0xB8, 0x8D, 0x00, 0xc0, 0x4f, 0xad, 0x51, 0x72 } };
HDEVINFO Ramdev = INVALID_HANDLE_VALUE;
SP_DEVINFO_DATA Info;
TCHAR RAMDRIVE[] = TEXT("%ramdrv%");

DWORD InitDevice(void)
{
	DWORD Index;
	UINT count = RETRY;
	Ramdev = SetupDiGetClassDevs(&Ramdrive,NULL,NULL,0);
	while ((Ramdev==INVALID_HANDLE_VALUE) && count)
	{
		Ramdev = SetupDiGetClassDevs(&Ramdrive,NULL,NULL,0);
	}

	if (Ramdev==INVALID_HANDLE_VALUE)
	{
		return RAMDISK_NO_DRIVER;
	}

	Info.cbSize = sizeof(Info);
	for(Index=0;SetupDiEnumDeviceInfo(Ramdev,Index,&Info);Index++);

	return RAMDISK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
bool ManageDriver(DWORD action)
{
	SP_PROPCHANGE_PARAMS Propcp;
	Propcp.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	Propcp.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	Propcp.StateChange = action;
	Propcp.Scope = DICS_FLAG_CONFIGSPECIFIC;
	Propcp.HwProfile = 0;

	if(!SetupDiSetClassInstallParams(Ramdev,&Info,&Propcp.ClassInstallHeader,sizeof(Propcp)))
	{
		return false;
	}

	if(!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE,Ramdev,&Info))
	{
		return false;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL SetRamdSize(DWORD NewRamDiskSize)
{
	TCHAR szDrive[MAX_NAME];
	DWORD errcode;
	DWORD Len = sizeof(DWORD);
	DWORD retry=0;
	DWORD OldValue = 0;

	switch (InitDevice())
	{
	case RAMDISK_NO_DRIVER:
		ERRORHANDLE;
		break;
	case RAMDISK_ERRORHANDLE:
		ERRORHANDLE;
		break;
	}

	DWORD dwType=REG_DWORD;
	DWORD dwSize=sizeof(OldValue);
	if (SHGetValue(HKEY_LOCAL_MACHINE,RAMKEY, RAMVALUE, &dwType, &OldValue, &dwSize) != ERROR_SUCCESS)
	{
		ERRORHANDLE;
	}

	if (NewRamDiskSize == OldValue)
	{
		return TRUE;
	}

	dwSize=sizeof(NewRamDiskSize);
	if (SHSetValue(HKEY_LOCAL_MACHINE,RAMKEY, RAMVALUE, REG_DWORD, &NewRamDiskSize, dwSize) != ERROR_SUCCESS)
	{
		ERRORHANDLE;
	}

	ExpandEnvironmentStrings(RAMDRIVE, szDrive, MAX_NAME);

	if (UStrLen(szDrive) < 2)
	{
		ERRORHANDLE;
	}

	while (retry < RETRY)
	{
		ManageDriver(DICS_ENABLE);
		ManageDriver(DICS_DISABLE);
		DiskIsExist(szDrive); 			
		errcode = GetLastError();
		if (errcode == ERROR_PATH_NOT_FOUND)
		{
			break;
		}
		retry++;
	}

	if (retry==RETRY)
	{
		ERRORHANDLE;
	}

	retry = 0;
	while (retry < RETRY)
	{
		ManageDriver(DICS_DISABLE);
		ManageDriver(DICS_ENABLE);
		DiskIsExist(szDrive); 			
		errcode = GetLastError();
		if (errcode != ERROR_PATH_NOT_FOUND)
		{
			break;
		}
		retry++;
	}

	if (retry==RETRY)
	{
		ERRORHANDLE;
	}

	while (true)
	{ 		
		if (!DiskIsExist(szDrive))
		{
			break;
		}

		errcode = GetLastError();
		if (errcode != ERROR_PATH_NOT_FOUND)
		{
			ERRORHANDLE;
		}
	}

	if (Ramdev!=INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(Ramdev);
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT Ramd(PTSTR ptzCmd)
{
	DWORD NewSize = GetNewSize(ptzCmd,1,512,32);
	SetRamdSize(NewSize*(1024*1024));
	TCHAR szDrive[MAX_NAME];
	ExpandEnvironmentStrings(RAMDRIVE, szDrive, MAX_NAME);
	return (!DiskIsExist(szDrive));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
PTSTR GetNewLine(PTSTR ptBuf,PTSTR ptOldLine,PTSTR ptEnd)
{
	UINT oqvar = *ptEnd;
	*ptEnd = 0;	
	UStrCopy(ptBuf,ptOldLine);
	*ptEnd = oqvar;

	PTSTR p = UStrChr(ptBuf, '`');
	if (p)
	{
		*p = 0;
	}
	return UStrTrim(ptBuf);
}
//////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SUBLabel TEXT("_SUB")
#define ENDLabel TEXT("_END")
HRESULT Command(PTSTR);
HRESULT CallSub(PTSTR ptzCmd) //入口参数，子过程名
{    
	TCHAR tzPath[MAX_NAME];
	UStrPrint(tzPath, TEXT("%s %s"), SUBLabel,ptzCmd);

	PTSTR p = UStrStr(pFileStart, tzPath); //p子过程入口
	if (p)
	{
		for (PTSTR q = p; *q; q++)
		{
			if ((*q == '\r') || (*q == '\n'))
			{	
				TCHAR CurLine[MAX_STR];
				PTSTR Newp =GetNewLine(CurLine,p,q);

				if ((Newp[0]) && (Newp[0] != '`') && (Newp[0] != '/')) //非注释行和空行
				{	
					if (UStrCmpNI(Newp, ENDLabel,4)) //非子过程定义出口语句
					{
						if (UStrCmpNI(Newp,SUBLabel,4)) //非子过程定义入口语句
						{
							Command(Newp); //Command函数不解释 _SUB、_END 命令。
						}
					}
					else
					{
						break; //子过程出口
					}
				}
				for (q++; (*q == '\r') || (*q == '\n'); q++);
				p = q;
			}
		}
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Call(PTSTR ptzCmd)
{
	if (*ptzCmd == '$')
	{
		ptzCmd++;
		return CallDll(ptzCmd);
	}
	else
	{
		return CallSub(ptzCmd);
	}	
}
/////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT LoadFile(PTSTR ptzPath)
{	
	TCHAR tzPath[MAX_NAME];
	UStrCopy(tzPath,ptzPath);

	UINT uSize = -1;
	PBYTE pbFile = (PBYTE) UFileLoad(tzPath, NULL, &uSize);
	if (pbFile == NULL)
	{
		return S_FALSE;
	}

	// Set current directory to COMMAND FILE directory
	PTSTR p = UStrRChr(tzPath, '\\');
	if (p)
	{
		p[1] = 0;
		SetCurrentDirectory(tzPath);
		p[0] = 0;		
		tzPath[2] = 0;
		SetEnvironmentVariable(TEXT("CurDrv"), tzPath);
	}

	// Convert ASCII <=> UNICODE
	p = (PTSTR) pbFile;
#ifdef _UNICODE
	if (pbFile[3])	// !IsTextUnicode(pbFile, -1, NULL)
	{
		uSize += MD5Str_Len;
		p = (PTSTR) UMemAlloc(uSize * sizeof(TCHAR));
		UAStrToWStr(p, (PCSTR) pbFile, uSize);
		UMemFree(pbFile);
		pbFile = (PBYTE) p;
	}
#else
	if (!pbFile[3])	// IsTextUnicode(pbFile, -1, NULL)
	{
		uSize += MD5Str_Len;
		p = (PTSTR) UMemAlloc(uSize * sizeof(TCHAR));
		UWStrToAStr(p, (PCWSTR) pbFile, uSize);
		UMemFree(pbFile);
		pbFile = (PBYTE) p;
	}
#endif

	PTSTR pCurFileStart = p; //INI文件的起点
	pFileStart = pCurFileStart;
	BOOL IsSUB = FALSE;      //是否子过程

	for (PTSTR q = p; *q; q++)
	{
		if ((*q == '\r') || (*q == '\n'))
		{
			TCHAR CurLine[MAX_STR];
			PTSTR Newp = GetNewLine(CurLine,p,q);

			if ((Newp[0]) && (Newp[0] != '/') && (Newp[0] != '`'))	//非注释行和空行
			{	
				if (!UStrCmpNI(Newp,SUBLabel,4))					//主流程
				{													//跳过
					IsSUB = TRUE;									//_SUB
				}													//至
				else if (!UStrCmpNI(Newp,ENDLabel,4))				//_END
				{													//间的
					IsSUB = FALSE;									//命令  
				}
				else if (!IsSUB)									//非子过程
				{	
					Command(Newp);
				}
			}
			for (q++; (*q == '\r') || (*q == '\n'); q++);
			p = q;
		}
	}
	UMemFree(pbFile);
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Load command file
HRESULT Load(PTSTR ptzPath)
{
	PTSTR ForeFile = pFileStart;
	if (ptzPath[0] == '\\')
	{
		ptzPath++;
		TCHAR tzDrives[MAX_NAME];
		GetLogicalDriveStrings(MAX_NAME, tzDrives);
		for (PTSTR p = tzDrives; *p; p += UStrLen(p) + 1)
		{
			TCHAR tzPath[MAX_NAME];
			UStrPrint(tzPath, TEXT("%s%s"), p, ptzPath);
			if (UFileExist(tzPath))
			{
				LoadFile(tzPath);
			}
		}
	}
	else
	{
		if (UFileExist((PTSTR) ptzPath))
		{
			LoadFile(ptzPath);
		}
	}
	pFileStart = ForeFile;
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/*
HRESULT Hide(PTSTR ptzCmd)
{	
BOOL Hide = TRUE;
SHSetValue(HKEY_LOCAL_MACHINE, PELOGON, TEXT("Hide"), REG_DWORD, &Hide, sizeof(Hide));
return S_OK;
}
*/
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Init desktop
HRESULT Main(PTSTR ptzCmd)
{
	if (!FindResource(g_hInst, _MakeIntRes(IDR_MENU1),RT_MENU))
	{
		return S_FALSE;
	}
	CheckMe = CheckMERes();

	// Switch desktop
	HDESK hDesk = OpenDesktop(TEXT("Default"), 0, TRUE, GENERIC_ALL);
	if (hDesk)
	{
		if (SetThreadDesktop(hDesk))
		{
			SwitchDesktop(hDesk);
		}
		CloseDesktop(hDesk);
	}
	HideProcess();

	// Execute LOAD command
	if (ptzCmd[0])
	{
		TCHAR tzCmd[MAX_PATH];
		UStrPrint(tzCmd + GetModuleFileName(NULL, tzCmd, MAX_PATH), TEXT(" LOAD %s"), ptzCmd);
		Exec(tzCmd);		
	}

	MainWindowHwnd = CreateMainWindow(g_hInst/*, SW_HIDE*/); 

	UThreadClose(UThreadCreate((UPROC) MainThread, NULL));

	SetProcessWorkingSetSize(CurrentProcess,-1,-1);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// Exit
	InstallHookKeyBoard(FALSE);
	return (HRESULT) msg.wParam;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
HRESULT CommandGroup(PTSTR ptzCurrent)
{
	HRESULT hResult;
	PTSTR ptzNextCom = UStrChr(ptzCurrent, '|');

	if (ptzNextCom)
	{
		*ptzNextCom++ =0 ;
		hResult = Command(ptzCurrent);	
		hResult = CommandGroup(ptzNextCom) + hResult;		
	}
	else
	{
		//atoi
		//_wtof
		hResult = Command(ptzCurrent);
	}
	return (!(hResult==0));
}
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
HRESULT Team(PTSTR ptzCurrent)
{
	return CommandGroup(ptzCurrent);
}
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Pause(UINT KeyCode)
{	
	static DWORD timestart = 0;
	BOOL zKeyPess = FALSE;
	UINT hResult = 0;
	UINT i;	
	if (KeyCode)
	{
		if (GetAsyncKeyState(KeyCode) & 0x8000)
		{
			hResult = KeyCode;
		}
	}
	else
	{
		while (!zKeyPess)
		{
			for (i = 8; i < 255; i++)
			{
				if (GetAsyncKeyState(i) & 0x8000)
				{
					if (GetTickCount() - timestart > 1000)
					{
						timestart = GetTickCount();
						zKeyPess = true;
						break;
					}					
				}
			}
		}
		hResult = i;
	}

	TCHAR tzPessKey[MAX_NAME];
	UStrCopy(tzPessKey,TEXT("PessKey="));

	if (hResult)
	{
		if (((hResult>64) && (hResult<91)) || ((hResult>47) && (hResult<58)))
		{
			TCHAR TEMP[2] = {0};
			TEMP[0] = hResult;
			UStrCat(tzPessKey,TEMP);
		}
	}
	SetEnvironment(tzPessKey,FALSE,FALSE);
	return hResult;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT IFEx_Or_Find(PTSTR ptzCmd,bool GetFree)
{
	BOOL hResult = false;
	DWORD Var1 = 0;
	DWORD Var2 = 0;

	PTSTR ptzCommand1 = UStrChr(ptzCmd, ',');
	PTSTR ptzCommand2;

	if (ptzCommand1) 
	{
		*ptzCommand1++ = 0; 

		ptzCommand2 = UStrChr(ptzCommand1, '!');
		if (ptzCommand2)
		{
			*ptzCommand2 = 0;
			ptzCommand2++;
		}

		if (ptzCmd[0] == '$')
		{
			*ptzCmd = '%';
			PTSTR ptzVar = UStrChr(ptzCmd, '=');
			if (ptzVar == NULL)
			{
				ptzVar = UStrChr(ptzCmd, '>');
			}
			if (ptzVar == NULL)
			{
				ptzVar = UStrChr(ptzCmd, '<');
			}

			if (ptzVar)
			{
				DWORD bCOMP = *ptzVar;
				*ptzVar = '%';			
				ptzVar++;
				UINT TEMP;
				TEMP = *ptzVar;
				*ptzVar = 0;

				TCHAR ptzENVVar[MAX_STR];
				hResult = ExpandEnvironmentStrings(ptzCmd,ptzENVVar, MAX_STR);
				*ptzVar=TEMP;
				UStrTrim(ptzVar);
				INT bRE = UStrCmpI(ptzENVVar,ptzVar);//

				if (*ptzENVVar != '%')
				{
					if (bCOMP == '>')
					{
						hResult = (bRE > 0);
					}
					else if (bCOMP == '<')
					{
						hResult = (bRE < 0);
					}
					else if (bCOMP == '=')
					{
						hResult = (bRE == 0);
					}
				}
				else
				{
					if (bCOMP == '>')
					{
						hResult = (!(*ptzVar >= 0));
					}
					else if (bCOMP == '<')
					{
						hResult = (*ptzVar >= 0);
					}
					else if (bCOMP == '=')
					{
						hResult = (*ptzVar == 0);
					}
				}
			}
		}
		else
		{
			TCHAR KeyWord[3];
			UStrPrint(KeyWord, TEXT("%.3s"), ptzCmd); //关键词 
			if ((ptzCmd[3] == '>') || (ptzCmd[3] == '<') || (ptzCmd[3] == '=') /*|| (ptzCmd[3] == '!')*/)
			{
				Var2 = UStrToInt(ptzCmd + 4); //数值2
				if (!UStrCmpNI(KeyWord, TEXT("MEM"), 3))
				{
					Var1 = (GetFree? GetFreeMemo(): GetTotalMemo());
				}
				else if (!UStrCmpNI(KeyWord, TEXT("KEY"), 3))
				{
					Var1 = Pause(Var2);
				}
				else
				{
					Var1 = (GetFree? GetDiskFree(KeyWord): GetDiskTotal(KeyWord));
				}

				ptzCmd = ptzCmd + 3 ;//比较符

				if (*ptzCmd == '=')
				{
					hResult = (Var1 == Var2);
				}
				else if (*ptzCmd == '>')
				{
					hResult = (Var1 > Var2);
				}
				else if (*ptzCmd == '<')
				{
					hResult = (Var1 < Var2);			
				}
			}
			else		
			{		
				if (GetFree)
				{
					hResult = UFileExist(ptzCmd);
				}
				else
				{
					hResult = (FindProc(ptzCmd) > 0);
				}
			}
		}
	}
	return (hResult? CommandGroup(ptzCommand1): CommandGroup(ptzCommand2));
}
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT IFEx(PTSTR ptzCmd)
{
	return IFEx_Or_Find(ptzCmd,true);
}
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Find(PTSTR ptzCmd)
{
	return IFEx_Or_Find(ptzCmd,false);
}
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Wait(PTSTR ptzCmd)
{
	INT bTime = UStrToInt(ptzCmd);
	if (bTime > 0)
	{
		return TimeDelay(UStrToInt(ptzCmd));
	}
	else
	{
		return (!(Pause(NULL)>0));
	}
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Logo(PTSTR ptzCmd)
{	
	if (ptzCmd[0])
	{	
		tzDrawTextDone = FALSE;
		CheckMe = CheckMERes();
		ExistLogoImage = FALSE;		
		UThreadClose(UThreadCreate((UPROC) LogoCMD, ptzCmd));		
		WaitLogo(FALSE); //等候 Logo 窗口启动
		DisplayAppDesc(); //显示 PECMD 信息
	}
	else
	{
		PostMessage(s_hLogo, WM_COMMAND, IDCANCEL,0);
		DeleteObject(hfont);
	}
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
VOID SetError(HRESULT eResult)
{	
	if (eResult)
	{		
		LastError = GetLastError();		
	}
	else
	{
		LastError = 0;
	}
	TCHAR szError[MAX_NAME];
	UStrPrint(szError, TEXT("%d"), LastError);
	SetEnvironmentVariable(TEXT("Error"), szError);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
VOID SetLogs(PTSTR tzCmdStr,HRESULT eResult)
{	
	HRESULT hResult = eResult;
	if (hResult)
	{
		if (hResult == E_NOTIMPL)
		{
			hResult = S_FALSE;
		}
		else
		{
			hResult = GetLastError();
		}
	}

	UStrCat(LogFileData,tzCmdStr);

	TCHAR tzStr[MAX_PATH];
	PTSTR p = tzStr; *p++ = '\t';
	p += FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hResult, 0, p, _NumOf(tzStr), NULL);
	if (p == tzStr + 1)
	{
		p += LoadString(g_hInst, IDS_CmdErr, p, MAX_NAME);
		p += UStrPrint(p, TEXT("%08X"), hResult);
	}

	TCHAR dtzStr[MAX_PATH];
	UStrCopyN(dtzStr,tzStr,(UINT) (p - tzStr));
	UStrCat(LogFileData,dtzStr);
	UStrCat(LogFileData,TEXT("\n"));
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Logs(PTSTR ptzCmd)
{
	if (g_hXLog)
	{		
		TCHAR tzStr[MAX_STR];
		UStrPrint(tzStr, TEXT("LOGS %s"),ptzCmd);
		SetLogs(tzStr,S_OK);

		g_hXLog = UFileOpen(LogFileName, UFILE_WRITE);
		UFileWrite(g_hXLog, LogFileData, UStrLen(LogFileData) * sizeof(TCHAR));
		UFileClose(g_hXLog);
		UMemFree(LogFileData);
	}

	if (ptzCmd[0])
	{

#ifdef _UNICODE

		UINT LogSize = MD5Str_Len;
		PTSTR p = UStrChr(ptzCmd, ',');
		if (p)
		{
			*p++ = 0;
			LogSize = UStrToInt(p);
		}		
		UStrCopy(LogFileName,ptzCmd);
		LogFileData=(PTSTR)UMemRealloc(LogFileData,sizeof(TCHAR)* MAX_STR*LogSize);
		UMemSet(LogFileData,0,UStrLen(LogFileData));
		LogFileData[0]=0xFEFF;
		g_hXLog = (HANDLE) TRUE;

#endif

	}
	else
	{
		g_hXLog = NULL;
	}
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Mdir(PTSTR ptzCmd)
{
	UDirCreate(ptzCmd);
	return (CreateDirectory(ptzCmd, NULL))? S_OK:S_FALSE;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Path(PTSTR ptzCmd)
{
	if (!PECMDCurrentDir[0])
	{
		GetCurrentDirectory(MAX_PATH,PECMDCurrentDir);
	}

	if (ptzCmd[0])
	{
		if (*ptzCmd == '@')
		{
			ptzCmd++;
			return (SetCurrentDirectory(ptzCmd)? S_OK:S_FALSE);
		}
		else if (*ptzCmd == '#')
		{
			ptzCmd++;
			return File(ptzCmd);
		}
		else
		{
			return Mdir(ptzCmd);
		}
	}
	else
	{	
		if (!PECMDCurrentDir[0])
		{
			ExpandEnvironmentStrings(TEXT("%SystemRoot%\\SYSTEM32"),PECMDCurrentDir, MAX_PATH);
		}
		return (SetCurrentDirectory(PECMDCurrentDir)? S_OK:S_FALSE);
	}
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
UINT MessageWaitTime;
UINT MessageAnswer;
HWND CloseMessageBox(PTSTR ptzCaption)
{		
	HWND BoxWindow;
	for (UINT i=0;((i<RETRY)||(!BoxWindow));i++)
	{
		BoxWindow = FindWindow(NULL,ptzCaption);		
	}
	if (BoxWindow)
	{	
		if (GetParent(BoxWindow) == s_hLogo) //父窗口是 PECMD 的 LOGO
		{
			SetWindowPos(BoxWindow,HWND_TOPMOST,NULL,NULL,NULL,NULL,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
			SetForegroundWindow(BoxWindow);
			SetActiveWindow(BoxWindow);
			Sleep(MessageWaitTime);
			SendMessage(BoxWindow, WM_COMMAND, MessageAnswer, 0);
		}
	}
	return BoxWindow;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Mess(PTSTR ptzCmd)
{
	HRESULT hResult = false;
	PTSTR ptzCaption = UStrChr(ptzCmd, '@'); //窗口标题

	if (ptzCaption)
	{
		*ptzCaption++ = 0;
		PTSTR ptzType = UStrChr(ptzCaption, '#');  //窗口类型
		if  (ptzType)
		{
			*ptzType++ = 0;
			MessageWaitTime = 0;
			PTSTR pzWaitTime = UStrChr(ptzType, '*'); //设置超时
			if (pzWaitTime)
			{
				pzWaitTime++;
				MessageWaitTime = UStrToInt(pzWaitTime);
			}

			MessageAnswer = IDYES;
			BOOL BoxYNType = (UStrCmpNI(ptzType, TEXT("OK"), 2));
			if (BoxYNType)
			{
				PTSTR pzAnswer = UStrChr(ptzType, '$'); //默认选择
				if (pzAnswer)
				{
					pzAnswer++;
					UStrToUpper(pzAnswer);
					MessageAnswer = MessageAnswer + ((pzAnswer) && (pzAnswer[0] == 'N'));
				}
			}			

			ptzCmd = USplitStr(ptzCmd);

			if (MessageWaitTime)
			{
				UThreadClose(UThreadCreate((UPROC) CloseMessageBox,ptzCaption)); //开创线程
			}

			if (BoxYNType)
			{
				TCHAR tzPath[MAX_NAME];
				UStrCopy(tzPath,TEXT("YESNO="));
				if (MessageBox(s_hLogo,ptzCmd,ptzCaption,36/*MB_YESNO*/) == 6)
				{
					UStrCat(tzPath,TEXT("YES"));
				}
				else
				{
					UStrCat(tzPath,TEXT("NO"));
				}
				SetEnvironment(tzPath,FALSE,FALSE);
			}
			else
			{
				MessageBox(s_hLogo,ptzCmd,ptzCaption,64/*MB_OK*/);
			}
			hResult = true;
		}
	}
	return (hResult? S_OK:S_FALSE);	
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
PTSTR pzInitDirName;
INT_PTR CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		SendMessage(hwnd, BFFM_SETSELECTION,TRUE,(LPARAM) pzInitDirName);
	}
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
#define TargetRoot TEXT("TARGET_ROOT")
#define TargetDriv TEXT("TARGET_DRIVE")
PTSTR ShellBrowseFolder(PTSTR InitPath,BOOL IncludeFiles,PTSTR ptzTitle)
{
	BROWSEINFO bi = {0};	

	bi.ulFlags = BIF_RETURNONLYFSDIRS| 
		BIF_EDITBOX| 
		BIF_NONEWFOLDERBUTTON| 
		BIF_USENEWUI;
	if (IncludeFiles)
	{
		bi.ulFlags = bi.ulFlags|BIF_BROWSEINCLUDEFILES;
	}

	if (ptzTitle && ptzTitle[0])
	{
		bi.lpszTitle = ptzTitle;
	}
	else
	{
		//LoadString(g_hInst, IDS_BrowseTitle,(LPWSTR)bi.lpszTitle, MAX_NAME);
		LoadString(g_hInst, IDS_BrowseTitle,(PTSTR)bi.lpszTitle, MAX_NAME);
	}

	bi.hwndOwner = s_hLogo;
	bi.lpfn = BrowseCallbackProc;
	ITEMIDLIST *pIDL = SHBrowseForFolder(&bi);
	if (SHGetPathFromIDList(pIDL,InitPath))
	{
		return InitPath;
	}
	else
	{
		return NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Brow(PTSTR ptzCmd)
{	
	TCHAR pzstrPathBuf[MAX_STR];
	TCHAR TARGET[MAX_STR];
	BOOL NoIncludeFiles = (*ptzCmd == '*');
	pzInitDirName = ptzCmd + (NoIncludeFiles);

	PTSTR ptzTitle = UStrChr(ptzCmd, '$');
	if (ptzTitle)
	{
		*ptzTitle++ = 0;
	}

	PTSTR pzBrowStr = ShellBrowseFolder(pzstrPathBuf,!NoIncludeFiles,ptzTitle);
	if (pzBrowStr)
	{
		UStrPrint(TARGET,TEXT("%s=%s"),TargetRoot,pzBrowStr);
		SetEnvironment(TARGET,TRUE,FALSE);
		SetEnvironment(TARGET,FALSE,FALSE);

		pzBrowStr[2] = 0;
		UStrPrint(TARGET,TEXT("%s=%s"),TargetDriv,pzBrowStr);
		SetEnvironment(TARGET,FALSE,FALSE);			
	}
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Help(PTSTR ptzCmd)
{
	PTSTR ptzName = UStrChr(ptzCmd, '#');
	if (ptzName)
	{
		*ptzName++ = 0;		
		if (ptzName)
		{
			EditBackColor = UStrToInt(ptzName);
		}
	}
	if (ptzCmd[0])
	{
		EditTextColor = UStrToInt(ptzCmd);
	}
	return (INT) DialogBox(g_hInst, _MakeIntRes(IDD_Help), NULL, (DLGPROC) HelpProc);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HANDLE OpenVolume(TCHAR cDriveLetter)
{
	HANDLE hVolume;
	UINT uDriveType;
	TCHAR szRootName[5]  = TEXT("C:\\");
	DWORD dwAccessFlags;

	szRootName[0] = cDriveLetter;

	uDriveType = MyGetDriveType(szRootName);
	switch(uDriveType)
	{
	case DRIVE_REMOVABLE:
		dwAccessFlags = GENERIC_READ | GENERIC_WRITE;
		break;
	case DRIVE_CDROM:
		dwAccessFlags = GENERIC_READ;
		break;
	default:		
		return INVALID_HANDLE_VALUE; //不能弹出的设备
	}

	szVolumeName[4] = cDriveLetter;

	hVolume = CreateFile(szVolumeName,
		dwAccessFlags,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	return hVolume;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
BOOL CloseVolume(HANDLE hVolume)
{
	return CloseHandle(hVolume);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
#define LOCK_TIMEOUT  10000 // 10 Seconds
BOOL LockVolume(HANDLE hVolume)
{
	DWORD dwBytesReturned;
	DWORD dwSleepAmount;
	int nTryCount;

	dwSleepAmount = LOCK_TIMEOUT / RETRY;

	// Do this in a loop until a timeout period has expired
	for (nTryCount = 0; nTryCount < RETRY; nTryCount++)
	{
		if (DeviceIoControl(hVolume,
			FSCTL_LOCK_VOLUME,
			NULL, 0,
			NULL, 0,
			&dwBytesReturned,
			NULL))
		{
			return TRUE;
		}
		Sleep(dwSleepAmount);
	}
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
BOOL DismountVolume(HANDLE hVolume)
{
	DWORD dwBytesReturned;
	return DeviceIoControl(hVolume,
		FSCTL_DISMOUNT_VOLUME,
		NULL, 0,
		NULL, 0,
		&dwBytesReturned,
		NULL);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
BOOL PreventRemovalOfVolume(HANDLE hVolume, BOOL fPreventRemoval)
{
	DWORD dwBytesReturned;
	PREVENT_MEDIA_REMOVAL PMRBuffer;
	PMRBuffer.PreventMediaRemoval = fPreventRemoval;
	return DeviceIoControl(hVolume,
		IOCTL_STORAGE_MEDIA_REMOVAL,
		&PMRBuffer,sizeof(PREVENT_MEDIA_REMOVAL),
		NULL, 0,
		&dwBytesReturned,
		NULL);
}
/////////////////////////////////////////////////////////////////////////////


BOOL MediaExist(HANDLE hVolume)
{
	DWORD dwBytesReturned;
	return DeviceIoControl(hVolume,
		IOCTL_DISK_CHECK_VERIFY,
		NULL,0,NULL,0,
		&dwBytesReturned,NULL);
}

/////////////////////////////////////////////////////////////////////////////
BOOL AutoEjectVolume(HANDLE hVolume)
{
	DWORD dwBytesReturned;

	return DeviceIoControl(hVolume,
		IOCTL_STORAGE_EJECT_MEDIA,
		NULL, 0,
		NULL, 0,
		&dwBytesReturned,
		NULL);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
BOOL EjectVolume(WCHAR cDriveLetter,BOOL UsbDriver)
{
	HANDLE hVolume;
	BOOL hResult;

	// Open the volume.
	hVolume = OpenVolume(cDriveLetter);
	//hVolume = OpenVolume((TCHAR)cDriveLetter);
	if (hVolume == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (MediaExist(hVolume))
	{
		// Lock and dismount the volume.
		if (LockVolume(hVolume) && DismountVolume(hVolume))
		{
			//PreventRemovalOfVolume(hVolume,FALSE);
			hResult = AutoEjectVolume(hVolume);
			if (UsbDriver && hResult)
			{
				TCHAR    VolumeName[] = TEXT("\\\\.\\_:");
				VolumeName[4] = cDriveLetter;
				//VolumeName[4] = (TCHAR)cDriveLetter;
				hResult = DefineDosDevice(DDD_REMOVE_DEFINITION,&VolumeName[4],NULL);
			}			
		}
		else
		{
			hResult = FALSE;
		}
		CloseVolume(hVolume);
		return hResult;
	}
	else
	{
		return TRUE;
	}	
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
GUID USBdrive = {0xA5DCBF10,0x6530,0x11D2,{0x90,0x1F,0x00,0xC0,0x4F,0xB9,0x51,0xED}};
GUID CDdrive = {0x53F56308,0xb6BF,0x11D0,{0x94,0xF2,0x00,0xA0,0xC9,0x1E,0xFB,0x8B}};
/////////////////////////////////////////////////////////////////////////////
/*
int EnumerateDevices(DWORD DeviceIndex, GUID DriverGUID, PBYTE DeviceName) 
{ 
DWORD RequiredSize = 0; 
HDEVINFO NewDeviceInfoSet; 
SP_DEVINFO_DATA DeviceInfoData; 
DWORD rs; 
DWORD typeP; 
BOOL res; 

NewDeviceInfoSet=SetupDiGetClassDevs(&DriverGUID, 
NULL, 
NULL, 
DIGCF_PRESENT); 
if( NewDeviceInfoSet == INVALID_HANDLE_VALUE ) 
{ 
*DeviceName=0; 
return -3; 
} 

DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA); 

res=SetupDiEnumDeviceInfo(NewDeviceInfoSet,DeviceIndex,&DeviceInfoData); 
if(!res) 
{ 
SetupDiDestroyDeviceInfoList(NewDeviceInfoSet); 
*DeviceName=0; 
return -1; 
} 

if(!SetupDiGetDeviceRegistryProperty(NewDeviceInfoSet,&DeviceInfoData,
SPDRP_PHYSICAL_DEVICE_OBJECT_NAME,&typeP,DeviceName,MAX_STR,&rs))
{ 
SetupDiDestroyDeviceInfoList(NewDeviceInfoSet); 
*DeviceName=0; 
return -4; 
} 

return 0; 
} 
*/
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/*
BOOL EjectDriver(GUID DriverGUID)
{
HDEVINFO		 hDevInfo;
SP_DEVINFO_DATA  DeviceInfoData;
DWORD			 i;
BOOL			 bResult = FALSE;


hDevInfo = SetupDiGetClassDevs(&DriverGUID,0,0,DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);   
if (hDevInfo == INVALID_HANDLE_VALUE)
{	
return FALSE;
}

DeviceInfoData.cbSize  =  sizeof(SP_DEVINFO_DATA);	

for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,&DeviceInfoData);i++)   
{	
DWORD   len;
CONFIGRET cr;   
PNP_VETO_TYPE pnpvietotype;		
TCHAR  vetoname[MAX_PATH];
TCHAR  DeviceName[MAX_NAME];
ULONG  ulStatus;   
ULONG  ulProblemNumber;
DWORD  dataType,rs;



cr = CM_Get_DevNode_Status(&ulStatus,&ulProblemNumber,DeviceInfoData.DevInst,0);
if ((!(DN_DISABLEABLE & ulStatus)) || (!(DN_REMOVABLE & ulStatus)))
{  
continue;
}

PTSTR pzServiceName = (PTSTR)DeviceName;
if(SetupDiGetDeviceRegistryProperty(hDevInfo,&DeviceInfoData,
SPDRP_SERVICE,&dataType,(PBYTE)DeviceName,MAX_NAME,&rs))
{ 
if ((!UStrCmpI(pzServiceName,TEXT("USBSTOR"))) || (!UStrCmpI(pzServiceName,TEXT("DISK"))))
{
len = MAX_PATH;
if ((CM_Request_Device_Eject(DeviceInfoData.DevInst,&pnpvietotype,(PTSTR)vetoname,len,0) == CR_SUCCESS))
{
bResult = SetupDiDestroyDeviceInfoList(hDevInfo);				
}
}			
}		
}
return bResult;
}
*/
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Ejec(PTSTR ptzCmd)
{
	UINT retry = 0;
	UINT uDriveType;
	BOOL tzDriverExist = FALSE;

	ptzCmd = UStrTrim(ptzCmd);
	UStrToUpper(ptzCmd);

	PTSTR temp = UStrChr(ptzCmd, '#');
	if (temp)
	{
		*temp = 0;
		retry = UStrToInt(++temp);
	}

	BOOL EjectCDROM = TRUE;
	BOOL EjectUSB = TRUE;

	if (ptzCmd[0])
	{
		EjectCDROM = (BOOL)StrStrI(ptzCmd, TEXT("C-"));
		EjectUSB = (BOOL)StrStrI(ptzCmd, TEXT("U-"));
	}

	PTSTR ptzDrive = UStrChr(ptzCmd, ':');
	if (ptzDrive)
	{
		ptzDrive-- ;
		if (ptzDrive)
		{
			uDriveType = MyGetDriveType(ptzDrive);
			if (EjectCDROM || EjectUSB)
			{
				*ptzDrive = 0; //弹出所有可弹出驱动器
			}

			if (uDriveType == DRIVE_REMOVABLE)
			{
				EjectUSB = TRUE; //弹出指定U盘
			}

			if (uDriveType == DRIVE_CDROM)
			{
				EjectCDROM = TRUE; //弹出指定光驱
			}			
		}
	}

	BOOL rb = FALSE;
	UINT i = 0;	

	while (!rb)
	{
		if (ptzDrive)
		{
			uDriveType = MyGetDriveType(ptzDrive);
			if ((uDriveType == DRIVE_REMOVABLE) || (uDriveType == DRIVE_CDROM))
			{
				tzDriverExist = TRUE;				
				rb = EjectVolume(ptzDrive[0],(uDriveType == DRIVE_REMOVABLE));
			}
			else
			{
				break;
			}
		}
		else
		{
			TCHAR tzDrives[MAX_NAME];		
			GetLogicalDriveStrings(MAX_NAME, tzDrives);
			for (PTSTR p = tzDrives; *p; p += UStrLen(p) + 1)
			{				
				uDriveType = MyGetDriveType(p);
				if (((uDriveType == DRIVE_REMOVABLE) && EjectUSB)||((uDriveType == DRIVE_CDROM) && EjectCDROM))
				{
					tzDriverExist = TRUE;
					rb = EjectVolume(p[0],(uDriveType == DRIVE_REMOVABLE));
				}
			}
		}

		if ((!rb) && (tzDriverExist))
		{
			if (retry>0)
			{
				i++;
				if (i == retry)
				{
					break;
				}
			}
			else
			{
				TCHAR RetryEject[MAX_NAME];
				TCHAR EjectErr[MAX_NAME];
				LoadString(g_hInst, IDS_RETRY_EJECT,RetryEject, MAX_NAME);
				LoadString(g_hInst, IDS_EJECT_ERR, EjectErr, MAX_NAME);
				if (MessageBox(s_hLogo,RetryEject,EjectErr,36) != 6)
				{
					break;
				}
			}
		}
		if (!tzDriverExist)
		{
			break;
		}
	}
	return (!rb);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
HRESULT Temp(PTSTR ptzCmd)
{
	HRESULT hResult = S_OK;
	//if (!ParentProcessIsME())
	//{
	//	return hResult;
	//}

	TCHAR tzTempPath[MAX_STR] = {0};
	TCHAR tzTempBuf[MAX_PATH] = {0};
	TCHAR ptzBoxText[MAX_STR] = {0};	
	ptzCmd = UStrTrim(ptzCmd);
	BOOL HideGUI = (*ptzCmd == '@');
	if (HideGUI)
	{
		ptzCmd++;
	}
	DWORD dwType=REG_SZ;
	DWORD dwSize=MAX_PATH;

	if (SHGetValue(HKEY_CURRENT_USER,Text_Environment,TEXT("TEMP"),&dwType, &tzTempBuf[0], &dwSize) != ERROR_SUCCESS)
	{
		if (SHGetValue(HKEY_CURRENT_USER,Text_Environment,TEXT("TMP"),&dwType, &tzTempBuf[0], &dwSize) != ERROR_SUCCESS)
		{
			if (!HideGUI)
			{
				TCHAR ReadEnviFail[MAX_NAME];
				TCHAR pError[MAX_NAME];
				LoadString(g_hInst,IDS_ReadEnviFail,ReadEnviFail, MAX_NAME);
				LoadString(g_hInst,IDS_Error,pError, MAX_NAME);
				MessageBox(s_hLogo,ReadEnviFail,pError,MD5Str_Len);
			}
			return S_FALSE;
		}
	}
	ExpandEnvironmentStrings(tzTempBuf,tzTempPath, MAX_STR);
	if (UDirExist(tzTempPath))
	{
		pzInitDirName = tzTempPath;
	}
	TCHAR TempFolder[MAX_NAME];
	LoadString(g_hInst,IDS_TempFolder,TempFolder, MAX_NAME);
	UStrPrint(ptzBoxText,TempFolder,tzTempPath);

	if (StrStrI(ptzCmd, TEXT("DELETE")))
	{
		TCHAR ClearTemp[MAX_NAME];
		TCHAR ClearTempFolder[MAX_NAME];		
		LoadString(g_hInst,IDS_ClearTemp,ClearTemp, MAX_NAME);
		LoadString(g_hInst,IDS_ClearTempFolder,ClearTempFolder, MAX_NAME);
		UStrCat(ptzBoxText,ClearTemp);
		if (HideGUI || (MessageBox(s_hLogo,ptzBoxText,ClearTempFolder,36) == 6))
		{
			UStrCat(tzTempPath,TEXT("\\*.*"));
			hResult = File(tzTempPath);
		}
	}
	else if (StrStrI(ptzCmd, TEXT("SETTING")))
	{
		TCHAR ReSetTempFolder[MAX_NAME];
		TCHAR SetTempFolder[MAX_NAME];
		LoadString(g_hInst,IDS_ReSetTempFolder,ReSetTempFolder, MAX_NAME);
		LoadString(g_hInst,IDS_SetTempFolder,SetTempFolder, MAX_NAME);
		UStrCat(ptzBoxText,ReSetTempFolder);
		if (MessageBox(s_hLogo,ptzBoxText,SetTempFolder,36) == 6)
		{
			TCHAR NewTempFolder[MAX_NAME];
			LoadString(g_hInst,IDS_NewTempFolder,NewTempFolder,MAX_NAME);
			PTSTR pzNewPtah = ShellBrowseFolder(tzTempPath,TRUE,NewTempFolder);
			if (pzNewPtah)
			{
				if (!UDirExist(pzNewPtah))
				{
					UDirSplitPath(pzNewPtah);
				}
				if (UDirExist(pzNewPtah))
				{
					TCHAR pzNewTempPath[MAX_PATH];
					UStrPrint(pzNewTempPath, TEXT("%s\\%s\\Temp=%s"),hKCU,Text_Environment,pzNewPtah);
					Regi(pzNewTempPath);
					UStrPrint(pzNewTempPath, TEXT("%s\\%s\\Tmp=%s"),hKCU,Text_Environment,pzNewPtah);
					Regi(pzNewTempPath);
					UStrPrint(pzNewTempPath, TEXT("TEMP=%s"),pzNewPtah);
					hResult = SetEnvironment(pzNewTempPath,TRUE,FALSE);
					UStrPrint(pzNewTempPath, TEXT("TMP=%s"),pzNewPtah);
					hResult = SetEnvironment(pzNewTempPath,TRUE,FALSE) + hResult;
					hResult = SetEnvironment(TEXT(""),FALSE,FALSE) + hResult;
				}
			}
		}
	}
	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI* PWIMMountImage)(PTSTR pzMountPath,PTSTR pzWimFileName,DWORD dwImageIndex,PTSTR pzTempPath);
typedef BOOL (WINAPI* PWIMUnMountImage)(PTSTR pzMountPath,PTSTR pzWimFileName,DWORD dwImageIndex,BOOL bCommitChanges);
#define WimFltrService TEXT("WimFltr")
/////////////////////////////////////////////////////////////////////////////
BOOL MountWimFile(PTSTR ptzWimFileName,PTSTR ptzMountPath,INT dwImageIndex,PTSTR ptzTemp)
{
	BOOL bResult = false;
	HMODULE hLib = NULL;
	hLib = LoadLibrary(TEXT("WIMGAPI.DLL"));
	Serv(WimFltrService);
	if (hLib)
	{
		DWORD dwImageID = dwImageIndex;
		if (*ptzWimFileName)
		{
			if (ptzMountPath)
			{
				PWIMMountImage pWIMMountImage = (PWIMMountImage)GetProcAddress(hLib,"WIMMountImage");
				if (pWIMMountImage)
				{
					bResult = pWIMMountImage(ptzMountPath,ptzWimFileName,dwImageID,ptzTemp);
				}
			}
			else
			{
				PWIMUnMountImage pWIMUnMountImage = (PWIMUnMountImage)GetProcAddress(hLib,"WIMUnmountImage");
				if (pWIMUnMountImage)
				{
					bResult = pWIMUnMountImage(ptzWimFileName,NULL,0,FALSE);
				}
			}
		}
		else
		{
			PWIMUnMountImage pWIMUnMountImage = (PWIMUnMountImage)GetProcAddress(hLib,"WIMUnmountImage");
			if (pWIMUnMountImage)
			{
				bResult = pWIMUnMountImage(ptzMountPath,NULL,0,FALSE);
			}
		}
		FreeLibrary(hLib);
	}
	return bResult;
}
/////////////////////////////////////////////////////////////////////////////

/*
/////////////////////////////////////////////////////////////////////////////
#define DEVICE_BASE_NAME   TEXT("\\FileDisk")
#define DEVICE_DIR_NAME    TEXT("\\Device")     DEVICE_BASE_NAME
#define DEVICE_NAME_PREFIX  DEVICE_DIR_NAME     DEVICE_BASE_NAME
#define FILE_DEVICE_FILE_DISK       0x8000
#define IOCTL_FILE_DISK_OPEN_FILE   CTL_CODE(FILE_DEVICE_FILE_DISK, 0x800, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_FILE_DISK_CLOSE_FILE  CTL_CODE(FILE_DEVICE_FILE_DISK, 0x801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_FILE_DISK_QUERY_FILE  CTL_CODE(FILE_DEVICE_FILE_DISK, 0x802, METHOD_BUFFERED, FILE_READ_ACCESS)
typedef struct _OPEN_FILE_INFORMATION {
LARGE_INTEGER   FileSize;
BOOLEAN         ReadOnly;
USHORT          FileNameLength;
UCHAR           FileName[1];
} OPEN_FILE_INFORMATION, *POPEN_FILE_INFORMATION;
/////////////////////////////////////////////////////////////////////////////
BOOL FileDiskMount(INT DeviceNumber,
POPEN_FILE_INFORMATION OpenFileInformation,
WCHAR DriveLetter,
BOOL CdImage)
{
TCHAR VolumeName[] = TEXT("\\\\.\\C:");
TCHAR DeviceName[MAX_PATH];
HANDLE  Device;
DWORD   BytesReturned;

VolumeName[4] = DriveLetter;
Device = CreateFile(
VolumeName,
GENERIC_READ | GENERIC_WRITE,
FILE_SHARE_READ | FILE_SHARE_WRITE,
NULL,
OPEN_EXISTING,
FILE_FLAG_NO_BUFFERING,
NULL
);

if (Device != INVALID_HANDLE_VALUE)
{
return FALSE;
}

if (CdImage)
{
sprintf(DeviceName, DEVICE_NAME_PREFIX TEXT("Cd") TEXT("%u"), DeviceNumber);
}
else
{
sprintf(DeviceName, DEVICE_NAME_PREFIX TEXT("%u"), DeviceNumber);
}

if (!DefineDosDevice(
DDD_RAW_TARGET_PATH,
&VolumeName[4],
DeviceName
))
{
return FALSE;
}

Device = CreateFile(
VolumeName,
GENERIC_READ | GENERIC_WRITE,
FILE_SHARE_READ | FILE_SHARE_WRITE,
NULL,
OPEN_EXISTING,
FILE_FLAG_NO_BUFFERING,
NULL
);

if (Device == INVALID_HANDLE_VALUE)
{   
DefineDosDevice(DDD_REMOVE_DEFINITION, &VolumeName[4], NULL);
return FALSE;
}

if (!DeviceIoControl(
Device,
IOCTL_FILE_DISK_OPEN_FILE,
OpenFileInformation,
sizeof(OPEN_FILE_INFORMATION) + OpenFileInformation->FileNameLength - 1,
NULL,
0,
&BytesReturned,
NULL
))
{
DefineDosDevice(DDD_REMOVE_DEFINITION, &VolumeName[4], NULL);
return FALSE;
}
return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////////////
HRESULT Moun(PTSTR ptzCmd)
{
	BOOL hResult = FALSE;
	INT dwImageID = 1;
	PTSTR ptzTemp = NULL;
	PTSTR ptzWimFileName = ptzCmd;
	PTSTR ptzMountPath = UStrChr(ptzWimFileName, ',');
	if (ptzMountPath)
	{
		*ptzMountPath++ = 0;
		PTSTR ptzImageID = UStrChr(ptzMountPath, ',');
		if (ptzImageID)
		{
			*ptzImageID++ = 0;
			dwImageID = UStrToInt(ptzImageID);
			ptzTemp = UStrChr(ptzImageID,',');
			if (ptzTemp)
			{
				*ptzTemp++ = 0;
				ptzTemp = (PTSTR)(((UINT)UFileExist(ptzTemp))*((DWORD)ptzTemp));
			}
		}
	}
	ptzWimFileName = UStrTrim(ptzWimFileName);
	ptzMountPath = UStrTrim(ptzMountPath);
	ptzTemp = UStrTrim(ptzTemp);
	hResult = MountWimFile(ptzWimFileName,ptzMountPath,dwImageID,ptzTemp);
	return hResult? S_OK:S_FALSE;
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/*
HRESULT Test(PTSTR ptzCmd)
{	
SetMeOnTopMenu();
return S_OK;
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/*
HRESULT Tint(PTSTR ptzCmd)
{
TCHAR OutStr[MAX_STR] = {0};
PTSTR p = UStrToUINT16(ptzCmd,OutStr);
SetClipboardText(p);
DebugMsg(p,UStrLen(p));
return S_OK;
}
*/
/*
HRESULT Tstr(PTSTR ptzCmd)
{
TCHAR temp[MAX_PATH];;
PTSTR p = UINT16ToUStr((PTSTR)temp,DescTextMD5,_NumOf(DescTextMD5));
SetClipboardText(p);
DebugMsg(p,UStrLen(p));
return S_OK;
}
*/
/////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Execute command
HRESULT Command(PTSTR ptzCmd)
{
	HRESULT hResult = E_NOTIMPL;	
	ptzCmd = UStrTrim(ptzCmd);

	TCHAR tzCmdStr[MAX_STR];
	TCHAR tzCmd[MAX_STR];
	ExpandEnvironmentStrings(ptzCmd, tzCmd, MAX_STR);
	UStrCopy(tzCmdStr,tzCmd);

	UINT uCmd = 0;
	ptzCmd = tzCmd;
	while (*c_tzCmd[uCmd])
	{
		if (!UStrCmpNI(ptzCmd, c_tzCmd[uCmd], LEN_Command))
		{
			// Skip white space
			ptzCmd += LEN_Command;
			ptzCmd =UStrTrim(ptzCmd);
			break;
		}
		uCmd++;
	}

	// Trim quote mark
	while (*ptzCmd == '"')
	{
		ptzCmd++;
		PTSTR p = ptzCmd + UStrLen(ptzCmd) - 1;
		if (*p == '"')
		{
			*p = 0;
		}
		else
		{
			break;
		}
	}

	switch (uCmd)
	{
		//case CMD_TEST:
		//	hResult = Test(ptzCmd);
		//	break;

	case CMD_MAIN:
		hResult = Main(ptzCmd);
		break;

	case CMD_LOAD:
		hResult = Load(ptzCmd);
		break;

	case CMD_EXEC:
		hResult = Exec(ptzCmd);
		break;

	case CMD_CALL:
		hResult = Call(ptzCmd);
		break;

	case CMD_REGI:
		hResult = Regi(ptzCmd);
		break;

	case CMD_ENVI:
		hResult = Envi(ptzCmd);
		break;

	case CMD_FILE:
		hResult = File(ptzCmd);
		break;

	case CMD_LINK:
		hResult = Link(ptzCmd);
		break;

	case CMD_SEND:
		hResult = Send(ptzCmd);
		break;

	case CMD_WAIT:
		hResult = Wait(ptzCmd);
		break;

	case CMD_KILL:
		hResult = Kill(ptzCmd);
		break;

	case CMD_SHUT:
		hResult = Shut(ptzCmd);
		break;

	case CMD_DEVI:
		hResult = Devi(ptzCmd);
		break;

	case CMD_SERV:
		hResult = Serv(ptzCmd);
		break;

	case CMD_PAGE:
		hResult = Page(ptzCmd);
		break;

	case CMD_DISP:
		hResult = Disp(ptzCmd);
		break;

	case CMD_LOGO:
		hResult = Logo(ptzCmd);
		break;

	case CMD_TEXT:
		hResult = Text(ptzCmd);
		break;

	case CMD_LOGS:
		hResult = Logs(ptzCmd);
		break;

	case CMD_WALL:
		hResult = Wall(ptzCmd);
		break;

	case CMD_FBWF:
		hResult = Fbwf(ptzCmd);
		break;

	case CMD_RAMD:
		hResult = Ramd(ptzCmd);
		break;

	case CMD_RUNS:
		hResult = Runs(ptzCmd);
		break;

	case CMD_USER:
		hResult = User(ptzCmd);
		break;

	case CMD_NUMK:
		hResult = Numk(ptzCmd);
		break;

	case CMD_SHEL:
		hResult = Shel(ptzCmd);
		break;

	case CMD_FONT:
		hResult = Font(ptzCmd);
		break;

	case CMD_TEAM:
		hResult = Team(ptzCmd);
		break;

	case CMD_IFEX:
		hResult = IFEx(ptzCmd);
		break;

	case CMD_FIND:
		hResult = Find(ptzCmd);
		break;

	case CMD_HOTK:
		hResult = Hotk(ptzCmd);
		break;

	case CMD_INIT:
		hResult = Init(ptzCmd);
		break;

	case CMD_PATH:
		hResult = Path(ptzCmd);
		break;

	case CMD_MESS:
		hResult = Mess(ptzCmd);
		break;

	case CMD_BROW:
		hResult = Brow(ptzCmd);
		break;

	case CMD_HELP:
		hResult = Help(ptzCmd);
		break;

	case CMD_EJEC:
		hResult = Ejec(ptzCmd);
		break;

	case CMD_TEMP:
		hResult = Temp(ptzCmd);
		break;

	case CMD_MD5C:
		hResult = Md5c(ptzCmd);
		break;

	case CMD_MOUN:
		hResult = Moun(ptzCmd);
		break;

		//case CMD_TINT:
		//	hResult = Tint(ptzCmd);
		//	break;

		//case CMD_TSTR:
		//	hResult = Tstr(ptzCmd);
		//	break;

	}
	if (g_hXLog)
	{
		SetLogs(tzCmdStr,hResult);
	}
	if ((hResult > 0) || (LastError > 0))
	{
		SetError(hResult);
	}
	return hResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
// Entry
INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR ptzCmdLine, INT iShowCmd)
{
	INT iResult = S_OK;	
	g_hInst = hInstance;
	CurrentProcess = GetCurrentProcess();
	//CheckVerDM5();//
	//CheckHelpDM5();//

	//UINT16ToUStr((PTSTR)LogoDescText,UDescStr,_NumOf(UDescStr));//
	//SetClipboardText(LogoDescText);//PECMD - WinPE Commander (Modified By Lxl1638) , V2.8.802.0162#
	//DebugMsg(LogoDescText,UStrLen(LogoDescText));//
	//DebugMsg(STR_Version,UStrLen(STR_Version));//

	//TCHAR LogoTextMD5[MAX_NAME] = {0};//38cd2c4c3957bbe213aef708cb65f250
	//PTSTR pLogoTextMD5 = UINT16ToUStr((PTSTR)LogoTextMD5,DescTextMD5,_NumOf(DescTextMD5));//
	//SetClipboardText(pLogoTextMD5);//
	//DebugMsg(pLogoTextMD5,UStrLen(pLogoTextMD5));//
	//TCHAR pTEMP[]=TEXT("%StartMenu%\\PECMD使用说明,");
	//Tint(pTEMP);

	if (ptzCmdLine[0])
	{
		NotChineseLangID = (GetSystemDefaultLangID() - 0x0804);
		g_iShowCmd = iShowCmd;		
		UINT16ToUStr((PTSTR)LogoDescText,UDescStr,_NumOf(UDescStr));
		SetError(NULL);
		iResult = Command(ptzCmdLine);		
		InstallHookKeyBoard(FALSE);
		if (g_bInit)
		{
			SetMeOnTopMenu();
		}
	}
	else
	{	
		iResult = (INT)DialogBox(g_hInst, _MakeIntRes(IDD_Help), NULL, (DLGPROC) HelpProc);
	}	

	return iResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////////