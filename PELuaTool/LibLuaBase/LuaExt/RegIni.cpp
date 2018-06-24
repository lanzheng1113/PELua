/**
 * \file RegIni.cpp 
 * \brief 实现LUA的RegIni模块
 */
#include "Stdafx.h"
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include "RegIni.h"
#include "util/StringEx.h"
#include "util/Logger.h"
#include <map>
#include <iostream>

using std::endl;
using std::cout;

namespace LuaExt
{
	namespace RegIni
	{

		/*
		Flag name	    Value	Description
		KEY_WOW64_64KEY	0x0100	Access a 64-bit key from either a 32-bit or 64-bit application.
		KEY_WOW64_32KEY	0x0200	Access a 32-bit key from either a 32-bit or 64-bit application.
		*/
		static DWORD gs_alter_reg_view = 0;
		#define AlterRegistrySAM(sam) (sam | gs_alter_reg_view)

		enum myGetProcAddressFunctions 
		{
			MGA_SetDefaultDllDirectories, // Win8+ but also exists on Vista/2008/7/2008R2 if KB2533623 is installed
#ifndef _WIN64
			MGA_GetDiskFreeSpaceEx,
			MGA_GetUserDefaultUILanguage,
			MGA_RegDeleteKeyEx,
#endif
			MGA_InitiateShutdown,
			MGA_IsUserAnAdmin,
			MGA_SHAutoComplete, // x64 can link to shlwapi directly but as long as MGA_SHGetFolderPath is used we can stick with myGetProcAddress
			MGA_SHGetFolderPath, // TODO: This can probably call something else directly on x64
			MGA_GetFileVersionInfoSize, // Version.dll exists in all Windows versions, it is delay loaded to avoid DLL hijacking [bug #1125]
			MGA_GetFileVersionInfo,
			MGA_VerQueryValue
		};

		// Jim Park: Keep these as chars since there's only ANSI version of
		// GetProcAddress.
		struct MGA_FUNC
		{
			const char *dll;
			const char *func;
		};

		struct MGA_FUNC MGA_FUNCS[] = 
		{
#ifdef _UNICODE
			{"KERNEL32", "SetDefaultDllDirectories"},
#ifndef _WIN64
			{"KERNEL32", "GetDiskFreeSpaceExW"},
			{"KERNEL32", "GetUserDefaultUILanguage"},
			{"ADVAPI32", "RegDeleteKeyExW"},
#endif
			{"ADVAPI32", "InitiateShutdownW"},
			{"SHELL32", (CHAR*) 680}, // IsUserAnAdmin
			{"SHLWAPI",  "SHAutoComplete"},
			{"SHFOLDER", "SHGetFolderPathW"},
			{"VERSION",  "GetFileVersionInfoSizeW"},
			{"VERSION",  "GetFileVersionInfoW"},
			{"VERSION",  "VerQueryValueW"}

		};
#else
			{"KERNEL32", "SetDefaultDllDirectories"},
#ifndef _WIN64
			{"KERNEL32", "GetDiskFreeSpaceExA"},
			{"KERNEL32", "GetUserDefaultUILanguage"},
			{"ADVAPI32", "RegDeleteKeyExA"},
#endif
			{"ADVAPI32", "InitiateShutdownA"},
			{"SHELL32", (CHAR*) 680}, // IsUserAnAdmin
			{"SHLWAPI",  "SHAutoComplete"},
			{"SHFOLDER", "SHGetFolderPathA"},
			{"VERSION",  "GetFileVersionInfoSizeA"},
			{"VERSION",  "GetFileVersionInfoA"},
			{"VERSION",  "VerQueryValueA"}
		};
#endif

		HMODULE LoadSystemLibrary(LPCSTR name)
		{
			LPCTSTR fmt = sizeof(*fmt) > 1 ? TEXT("%s%S.dll") : TEXT("%s%s.dll"); // The module name is always ANSI
			BYTE bytebuf[(MAX_PATH+1+20+1+3+!0) * sizeof(*fmt)]; // 20+4 is more than enough for 
			LPTSTR path = (LPTSTR) bytebuf;                      // the dllnames we are using.

			UINT cch = GetSystemDirectory(path, MAX_PATH);
			if (cch > MAX_PATH) // MAX_PATH was somehow not large enough and we don't support 
				cch = 0;          // \\?\ paths so we have to settle for just the name.
			wsprintf(path + cch, fmt, TEXT("\\") + (!cch || path[cch-1] == '\\'), name);

			return LoadLibraryEx(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		}

		/**
		 * Given a function enum, it will load the appropriate DLL and get the
		 * process address of the function and return the pointer.  It's up to
		 * the caller to know how to call that function, however.
		 *
		 * @param func Enum value that indexes the MGA_FUNCS array.
		 * @return Pointer to the function identified by the enum value.
		 */
		void* myGetProcAddress(const enum myGetProcAddressFunctions func)
		{
		  const char *dllname = MGA_FUNCS[func].dll;
		  HMODULE hModule;

		  hModule = GetModuleHandleA(dllname);    // Avoid LoadLibrary if possible because 
		  if (!hModule)                           // it can crash on 64-bit dlls if 
			hModule = LoadSystemLibrary(dllname); // WoW64 FS redirection is off.

		  return hModule ? GetProcAddress(hModule, MGA_FUNCS[func].func) : (FARPROC)hModule; // Optimized "return NULL;"
		}

		// based loosely on code from Tim Kosse
		// in win9x this isn't necessary (RegDeleteKey() can delete a tree of keys),
		// but in win2k you need to do this manually.
		static LONG myRegDeleteKeyEx(HKEY thiskey, LPCTSTR lpSubKey, int onlyifempty)
		{
			HKEY key;
			int retval=RegOpenKeyEx(thiskey,lpSubKey,0,AlterRegistrySAM(KEY_ENUMERATE_SUB_KEYS),&key);
			if (retval==ERROR_SUCCESS)
			{
				// NB - don't change this to static (recursive function)
				TCHAR buffer[MAX_PATH+1];
				while (RegEnumKey(key,0,buffer,MAX_PATH+1)==ERROR_SUCCESS)
				{
					if (onlyifempty)
					{
						RegCloseKey(key);
						return !ERROR_SUCCESS;
					}
					if ((retval=myRegDeleteKeyEx(key,buffer,0)) != ERROR_SUCCESS) break;
				}
				RegCloseKey(key);
				{
					typedef LONG (WINAPI * RegDeleteKeyExPtr)(HKEY, LPCTSTR, REGSAM, DWORD);
					RegDeleteKeyExPtr RDKE = (RegDeleteKeyExPtr)
#ifdef _WIN64
						RegDeleteKeyEx;
#else
						myGetProcAddress(MGA_RegDeleteKeyEx);
#endif
					if (RDKE)
						retval=RDKE(thiskey,lpSubKey,AlterRegistrySAM(0),0);
					else
						retval=RegDeleteKey(thiskey,lpSubKey);
				}
			}
			return retval;
		}

		HKEY KeyRootFromShortName(const std::string& strRoot)
		{
			HKEY hKey = 0;
			if (strRoot == "HKLM")
			{
				hKey = HKEY_LOCAL_MACHINE;
			}
			else if (strRoot == "HKCU")
			{
				hKey = HKEY_CURRENT_USER;
			}
			else if (strRoot == "HKCR")
			{
				hKey = HKEY_CLASSES_ROOT;
			}
			else if (strRoot == "HKUS")
			{
				hKey = HKEY_USERS;
			}
			else if (strRoot == "HKPD")
			{
				hKey = HKEY_PERFORMANCE_DATA;
			}
			else if (strRoot == "HKPT")
			{
				hKey = HKEY_PERFORMANCE_TEXT;
			}
			else if (strRoot == "HKPN")
			{
				hKey = HKEY_PERFORMANCE_NLSTEXT;
			}
			else if (strRoot == "HKCC")
			{
				hKey = HKEY_CURRENT_CONFIG;
			}
			else if (strRoot == "HKDD")
			{
				hKey = HKEY_DYN_DATA;
			}
			else if (strRoot == "HKCULS")
			{
				hKey = HKEY_CURRENT_USER_LOCAL_SETTINGS;
			}
			else
			{
				;
			}
			return hKey;
		}

		BOOL DeleteRegKey(const std::string& strRoot,const std::string& strKeyPath,BOOL bIfEmpty)
		{
			HKEY hKey = KeyRootFromShortName(strRoot);
			if (0 == hKey)
			{
				return FALSE;
			}
			std::wstring wstrKeyPath = String(strKeyPath).toStdWString();
			LONG lResult = myRegDeleteKeyEx(hKey,wstrKeyPath.c_str(),bIfEmpty);
			return lResult == ERROR_SUCCESS ? TRUE:FALSE;
		}

		BOOL DeleteRegValue(const std::string& strRegRoot,const std::string& strRegSubKey,const std::string& strKeyName)
		{
			LSTATUS res = -1;
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return FALSE;
			}
			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();

			if (RegOpenKeyEx(hKey, wstrRegSubKey.c_str(), 0, AlterRegistrySAM(KEY_SET_VALUE), &hKey) == ERROR_SUCCESS)
			{
				std::wstring wstrKeyName = String(strKeyName).toStdWString();
				res = RegDeleteValue(hKey,wstrKeyName.c_str());
				RegCloseKey(hKey);
			}
			
			if (res != ERROR_SUCCESS)
				return FALSE;
			else
				return TRUE;
		}

		std::vector<std::string> MyEnumRegKey(const std::string& strRegRoot, const std::string& strRegSubKey)
		{
			std::vector<std::string> vecRet;
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return vecRet;
			}
			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegOpenKeyEx(hKey, wstrRegSubKey.c_str(), 0, AlterRegistrySAM(KEY_READ), &hKey) == ERROR_SUCCESS)
			{
				const int MAX_KEY_LENGTH = 255;
				const int MAX_VALUE_NAME = 16383;

				TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
				DWORD    cbName;                   // size of name string 
				TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
				DWORD    cchClassName = MAX_PATH;  // size of class string 
				DWORD    cSubKeys=0;               // number of subkeys 
				DWORD    cbMaxSubKey;              // longest subkey size 
				DWORD    cchMaxClass;              // longest class string 
				DWORD    cValues;              // number of values for key 
				DWORD    cchMaxValue;          // longest value name 
				DWORD    cbMaxValueData;       // longest value data 
				DWORD    cbSecurityDescriptor; // size of security descriptor 
				FILETIME ftLastWriteTime;      // last write time 

				DWORD i, retCode; 

				//TCHAR  achValue[MAX_VALUE_NAME]; 
				DWORD cchValue = MAX_VALUE_NAME; 

				// Get the class name and the value count. 
				retCode = RegQueryInfoKey(
					hKey,                    // key handle 
					achClass,                // buffer for class name 
					&cchClassName,           // size of class string 
					NULL,                    // reserved 
					&cSubKeys,               // number of subkeys 
					&cbMaxSubKey,            // longest subkey size 
					&cchMaxClass,            // longest class string 
					&cValues,                // number of values for this key 
					&cchMaxValue,            // longest value name 
					&cbMaxValueData,         // longest value data 
					&cbSecurityDescriptor,   // security descriptor 
					&ftLastWriteTime);       // last write time 

				// Enumerate the subkeys, until RegEnumKeyEx fails.

				if (cSubKeys)
				{
					printf( "\nNumber of subkeys: %d\n", cSubKeys);

					for (i=0; i<cSubKeys; i++) 
					{ 
						cbName = MAX_KEY_LENGTH;
						retCode = RegEnumKeyEx(hKey, i,
							achKey, 
							&cbName, 
							NULL, 
							NULL, 
							NULL, 
							&ftLastWriteTime); 
						if (retCode == ERROR_SUCCESS) 
						{
							//_tprintf(TEXT("(%d) %s\n"), i+1, achKey);
							std::string astrKey = String::fromStdWString(achKey);
							vecRet.push_back(astrKey);
						}
					}
				} 

				RegCloseKey(hKey);
			}
			return vecRet;
		}

		std::vector<std::string> MyEnumRegValue(const std::string& strRegRoot, const std::string& strRegSubKey)
		{
			std::vector<std::string> vecRet;
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return vecRet;
			}
			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegOpenKeyEx(hKey, wstrRegSubKey.c_str(), 0, AlterRegistrySAM(KEY_READ), &hKey) == ERROR_SUCCESS)
			{
				const int MAX_KEY_LENGTH = 255;
				const int MAX_VALUE_NAME = 16383;

//				TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
//				DWORD    cbName;                   // size of name string 
				TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
				DWORD    cchClassName = MAX_PATH;  // size of class string 
				DWORD    cSubKeys=0;               // number of subkeys 
				DWORD    cbMaxSubKey;              // longest subkey size 
				DWORD    cchMaxClass;              // longest class string 
				DWORD    cValues;              // number of values for key 
				DWORD    cchMaxValue;          // longest value name 
				DWORD    cbMaxValueData;       // longest value data 
				DWORD    cbSecurityDescriptor; // size of security descriptor 
				FILETIME ftLastWriteTime;      // last write time 

				DWORD i, retCode; 

				TCHAR  achValue[MAX_VALUE_NAME]; 
				DWORD cchValue = MAX_VALUE_NAME; 

				// Get the class name and the value count. 
				retCode = RegQueryInfoKey(
										hKey,                    // key handle 
										achClass,                // buffer for class name 
										&cchClassName,           // size of class string 
										NULL,                    // reserved 
										&cSubKeys,               // number of subkeys 
										&cbMaxSubKey,            // longest subkey size 
										&cchMaxClass,            // longest class string 
										&cValues,                // number of values for this key 
										&cchMaxValue,            // longest value name 
										&cbMaxValueData,         // longest value data 
										&cbSecurityDescriptor,   // security descriptor 
										&ftLastWriteTime);       // last write time 

				// Enumerate the key values. 

				if (cValues) 
				{
					printf( "\nNumber of values: %d\n", cValues);

					for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
					{ 
						cchValue = MAX_VALUE_NAME; 
						achValue[0] = '\0'; 
						retCode = RegEnumValue(hKey, i, 
							achValue, 
							&cchValue, 
							NULL, 
							NULL,
							NULL,
							NULL);

						if (retCode == ERROR_SUCCESS ) 
						{ 
							//_tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
							std::string astrKey = String::fromStdWString(achValue);
							vecRet.push_back(astrKey);
						} 
					}
				}
				RegCloseKey(hKey);
			}
			return vecRet;
		}

		/**
		 * @see Lua_ReadRegDWORD
		 */
		std::string ReadRegDWORD(const std::string& strRegRoot, const std::string& strRegSubKey, const std::string& strRegName)
		{
			std::string strRet;
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return strRet;
			}

			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegOpenKeyEx(hKey, wstrRegSubKey.c_str(), 0, AlterRegistrySAM(KEY_READ), &hKey) == ERROR_SUCCESS)
			{
				BYTE buffer[4096] = {0};
				DWORD len = 4096*sizeof(TCHAR);
				DWORD dwType;

				// Jim Park: If plain text in p or binary data in p,
				// user must be careful in accessing p correctly.
				std::wstring wstrRegName = String(strRegName).toStdWString();
				if (RegQueryValueEx(hKey,wstrRegName.c_str(),NULL,&dwType,(LPBYTE)buffer,&len) != ERROR_SUCCESS ||
					(dwType != REG_DWORD))
				{
					strRet = "";
				}
				else
				{
					if (dwType==REG_DWORD)
					{
						strRet = String::fromUnsignedInt(*((DWORD*)buffer));
					}
					else
					{
						strRet = "";
					}
				}
				RegCloseKey(hKey);
			}

			return strRet;
		}

		/**
		 * @see Lua_ReadRegStr
		 */
		std::string ReadRegStr(const std::string& strRegRoot, const std::string& strRegSubKey, const std::string& strRegName)
		{
			std::string strRet;
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return strRet;
			}

			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegOpenKeyEx(hKey, wstrRegSubKey.c_str(), 0, AlterRegistrySAM(KEY_READ), &hKey) == ERROR_SUCCESS)
			{
				BYTE buffer[4096] = {0};
				DWORD len = 4096*sizeof(TCHAR);
				DWORD dwType;

				// Jim Park: If plain text in p or binary data in p,
				// user must be careful in accessing p correctly.
				std::wstring wstrRegName = String(strRegName).toStdWString();
				if (RegQueryValueEx(hKey,wstrRegName.c_str(),NULL,&dwType,(LPBYTE)buffer,&len) != ERROR_SUCCESS ||
					(dwType != REG_SZ && dwType != REG_EXPAND_SZ))
				{
					strRet = "";
				}
				else
				{
					buffer[4096-1]=0; // RegQueryValueEx adds a null terminator, UNLESS the value is buffer max long
					std::wstring wstr = (wchar_t*)buffer;
					strRet = String::fromStdWString(wstr);
				}
				RegCloseKey(hKey);
			}

			return strRet;
		}

		//十六进制字符串转换为字节流  
		int HexStrToByte(const char* source, unsigned char* dest, int sourceLen)  
		{  
			int i = 0;  
			unsigned char highByte, lowByte;  

			for (i = 0; i < sourceLen; i += 2)  
			{  
				highByte = toupper(source[i]);  
				lowByte  = toupper(source[i + 1]);  

				if (highByte > 0x39)  
					highByte -= 0x37;  
				else  
					highByte -= 0x30;  

				if (lowByte > 0x39)  
					lowByte -= 0x37;  
				else  
					lowByte -= 0x30;  

				dest[i / 2] = (highByte << 4) | lowByte;  
			}  
			return i/2;  
		} 

		BOOL WriteRegBin( const std::string& strRegRoot, const std::string& strRegSubKey, const std::string& strRegName, const std::string& strValueData )
		{
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return FALSE;
			}
			BOOL bRet = FALSE;
			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegCreateKeyEx(hKey,wstrRegSubKey.c_str(),0,0,0,AlterRegistrySAM(KEY_SET_VALUE),0,&hKey,0) == ERROR_SUCCESS)
			{
				BYTE data[4096] = {0};
				std::wstring wstrRegName = String(strRegName).toStdWString();
				/* hex string to binary data*/
				int strLen = strValueData.length();
				const char* pdata = strValueData.c_str();
				int datasize = HexStrToByte(pdata,data,strLen);
				if (datasize >= 0 && RegSetValueEx(hKey,wstrRegName.c_str(),0,REG_BINARY,data,datasize) == ERROR_SUCCESS)
				{
					bRet = TRUE;
				}
				RegCloseKey(hKey);
			}
			return bRet;
		}

		BOOL WriteRegDWORD( const std::string& strRegRoot, const std::string& strRegSubKey, const std::string& strRegName, DWORD dwValue )
		{
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return FALSE;
			}
			BOOL bRet = FALSE;
			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegCreateKeyEx(hKey,wstrRegSubKey.c_str(),0,0,0,AlterRegistrySAM(KEY_SET_VALUE),0,&hKey,0) == ERROR_SUCCESS)
			{
				BYTE data[16] = {0};
				*(DWORD*)data = dwValue;

				std::wstring wstrRegName = String(strRegName).toStdWString();
				if (RegSetValueEx(hKey,wstrRegName.c_str(),0,REG_DWORD,data,sizeof(DWORD)) == ERROR_SUCCESS)
				{
					bRet = TRUE;
				}
				RegCloseKey(hKey);
			}
			return bRet;
		}

		BOOL WriteRegStr( const std::string& strRegRoot, const std::string& strRegSubKey, const std::string& strRegName, const std::string& strValueData )
		{
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return FALSE;
			}
			BOOL bRet = FALSE;
			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegCreateKeyEx(hKey,wstrRegSubKey.c_str(),0,0,0,AlterRegistrySAM(KEY_SET_VALUE),0,&hKey,0) == ERROR_SUCCESS)
			{
				std::wstring wstrRegName = String(strRegName).toStdWString();
				std::wstring wstrValue = String(strValueData).toStdWString();
				if (RegSetValueEx(hKey,wstrRegName.c_str(),0,REG_SZ,(LPBYTE)wstrValue.c_str(),wstrValue.length()*2+2) == ERROR_SUCCESS)
				{
					bRet = TRUE;
				}
			}
			return bRet;
		}

		BOOL WriteRegExpandStr( const std::string& strRegRoot, const std::string& strRegSubKey, const std::string& strRegName, const std::string& strValueData )
		{
			HKEY hKey = KeyRootFromShortName(strRegRoot);
			if (0 == hKey)
			{
				return FALSE;
			}
			BOOL bRet = FALSE;
			std::wstring wstrRegSubKey = String(strRegSubKey).toStdWString();
			if (RegCreateKeyEx(hKey,wstrRegSubKey.c_str(),0,0,0,AlterRegistrySAM(KEY_SET_VALUE),0,&hKey,0) == ERROR_SUCCESS)
			{
				std::wstring wstrRegName = String(strRegName).toStdWString();
				std::wstring wstrValue = String(strValueData).toStdWString();
				if (RegSetValueEx(hKey,wstrRegName.c_str(),0,REG_EXPAND_SZ,(LPBYTE)wstrValue.c_str(),wstrValue.length()*2+2) == ERROR_SUCCESS)
				{
					bRet = TRUE;
				}
			}
			return bRet;
		}
	};
};

/**
 * \defgroup LUA_RegIni LUA脚本:RegIni模块
 * \{
 */

#define REGINI_VERSION "1.0.0.1"
#define REGINI_LIBNAME "RegIni"

/**
 * \brief LUA导出脚本 RegIni.DeleteINISec的实现。删除INI文件中的一节。
 * \param  l LUA栈参数。其中参数为：
 * - INI文件的路径。
 * - INI配置的节名
 * \return 总是1.表示LUA栈返回值被压入一个返回值，类型为boolean。如果删除失败返回false,否则返回true.
 * \remark 
 * For example:
 * WriteINIStr $TEMP\something.ini section1 something 123
 * WriteINIStr $TEMP\something.ini section1 somethingelse 1234
 * WriteINIStr $TEMP\something.ini section2 nsis true
 * DeleteINISec $TEMP\something.ini section1
 */
static const char* TempPtr = NULL;
int LFDeleteINISec(lua_State* l)
{
	String strIniFilePath = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strIniSectionName = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	BOOL bRet = WritePrivateProfileStringA(strIniSectionName.c_str(),NULL,NULL,strIniFilePath.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * \brief LUA导出脚本RegIni.DeleteINIStr(ini_path,ini_secion,ini_key)的实现。删除INI文件中的一对键值
 * \param l LUA栈参数。其中参数为：
 * - 文件名
 * - 节名
 * - 键名
 * \return 总是1.表示LUA栈返回值被压入一个返回值，类型为boolean。如果删除失败返回false,否则返回true.
 * \remark 
 * For example
 * WriteINIStr("$TEMP\something.ini","section1","something","123")
 * WriteINIStr("$TEMP\something.ini","section1","somethingelse","1234")
 * DeleteINIStr("$TEMP\something.ini","section1","somethingelse")
 */
int LFDeleteINIStr(lua_State* l)
{
	String strIniFilePath = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strIniSectionName = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strKeyName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	BOOL bRet = WritePrivateProfileStringA(strIniSectionName.c_str(),strKeyName.c_str(),NULL,strIniFilePath.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * \brief LUA导出脚本RegIni.DeleteRegKey(root_key,subkey,ifempty)的实现。删除注册表子健
 * \param l LUA栈参数。其中参数为：
 * - 根键(string)。
 * - 子键路径(string)。
 * - ifempty(boolean)。 If the parameter [ifempty] set to TRUE(1), 
 *   the registry key will only be deleted if it has no subkeys (otherwise, the whole registry tree will be removed). 
 * \return 总是1.表示LUA栈返回值被压入一个返回值，类型为boolean。如果删除失败返回false,否则返回true.
 * \remark 
 * Valid values for root_key are listed under WriteRegStr. 
 * For example:
 * DeleteRegKey("HKLM","Software\My Company\My Software",FALSE)
 * DeleteRegKey("HKLM","Software\A key that might have subkeys",TRUE)
*/
int LFDeleteRegKey(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegKeyPath = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	BOOL bIfEmpty = lua_toboolean(l,3);
	BOOL bRet = LuaExt::RegIni::DeleteRegKey(strRegRoot,strRegKeyPath,bIfEmpty);
	LOG_INFO("删除注册表键[%s][%s],结果=%d", strRegRoot.c_str(), strRegKeyPath.c_str(), bRet);
	lua_pushboolean(l,bRet);
	return 1;
}

/*
4.9.2.4 DeleteRegValue
Deletes a registry value. Valid values for root_key are listed under WriteRegStr. 

For example:
DeleteRegValue("HKLM","Software\My Company\My Software" "some value")

@param:
	root_key
	subkey
	key_name
@return:
	Return FALSE(0) if the value could not be removed from the registry (or if it didn't exist to begin with,or it can't be open.).
*/
int LFDeleteRegValue(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strKeyName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	BOOL ret = LuaExt::RegIni::DeleteRegValue(strRegRoot,strRegSubKey,strKeyName);

	lua_pushboolean(l,ret);

	return 1;
}

/*
4.9.2.5 EnumRegKey
Enum all subkey in "root_key\subkey" and return as a table.
Valid values for root_key are listed under WriteRegStr. 
@param:
	1.root_key
	2.subkey
@return:
	Returns an empty table if there are no any subkey, and returns an empty table and sets the error flag if there is an error.

For example:

local TableOfEnumRegKey = EnumRegKey("HKLM","SoftWare\\YourCompany\\YourProduct")
for key,value in pairs(TableOfEnumRegKey) do
	print("key: " .. tostring(key) .. ", value: " .. tostring(value))
end
*/
int LFEnumRegKey(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	std::vector<std::string> vecOfRegKey = LuaExt::RegIni::MyEnumRegKey(strRegRoot,strRegSubKey);
	/* create table. */  
	lua_newtable(l);  
	/* push (key, value). */ 
	for (int i=0; i!=vecOfRegKey.size(); i++)
	{
		lua_pushinteger(l,i+1); //从1开始
		lua_pushstring(l,vecOfRegKey[i].c_str());
		lua_settable(l,-3);
	}
	/* deal return. */
	return 1;
}

/*
4.9.2.6 EnumRegValue
Enum all values in "root_key\subkey" and return as a table.
Valid values for root_key are listed under WriteRegStr. 
@param:
	1.root_key
	2.subkey
@return:
	Returns an empty table if there are no any value, and returns an empty table and sets the error flag if there is an error.

For example:

local TableOfEnumRegValue = EnumRegValue("HKLM","SoftWare\\YourCompany\\YourProduct")
for key,value in pairs(TableOfEnumRegKey) do
	print("key: " .. tostring(key) .. ", value: " .. tostring(value))
end
*/
int LFEnumRegValue(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	std::vector<std::string> vecOfRegKey = LuaExt::RegIni::MyEnumRegValue(strRegRoot,strRegSubKey);
	/* create table. */  
	lua_newtable(l);  
	/* push (key, value). */ 
	for (int i=0; i!=vecOfRegKey.size(); i++)
	{
		lua_pushinteger(l,i+1); //从1开始
		lua_pushstring(l,vecOfRegKey[i].c_str());
		lua_settable(l,-3);
	}
	return 1;
}

/**
 * \brief 
 * LUA 导出接口RegIni.FlushINI的实现
 * 刷新INI文件的缓冲区，Windows 9x 在内存中保存INI文件的任何改变。使用这个LUA函数将所有INI文件的改变立即写入到文件中。
 * 当使用WriteINIStr, DeleteINISec or DeleteINStr之后调用这个函数刷新缓冲区。在XP之后这个函数实质上已经废弃。
 * \param l LUA栈参数。栈中一个参数，代表的是INI文件的路径。
 * \return 总是0。表示LUA没有返回值。
 * \remark 
 * - RegIni.FlushINI的参数：INI文件的路径。
 * - RegIni.FlushINI的返回值是一个代表操作成功或者失败的BOOLEAN值。
 * For example:
 * WriteINIStr($TEMP\something.ini,test,test,test)
 * FlushINI($TEMP\something.ini)
 * Delete($TEMP\something.ini)
 */
int LFFlushINI(lua_State* l)
{
	String strIniFile = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	BOOL bRet = WritePrivateProfileStringA(NULL,NULL,NULL,strIniFile.c_str());
	lua_pushboolean(l,bRet);
	return 0;
}

/**
 * \brief 从读取一个环境变量的值
 * \param l LUA栈参数。栈中一个参数，环境变量名
 * \return 总是1.表示LUA栈返回值被压入一个返回值。如果读取环境变量失败返回一个空字符串，否则返回环境变量的值。
 * \remark 
 * For example:
 * local envstr = ReadEnvStr("%WINDIR%")
 * local envsr2 = ReadEnvStr("%TEMP%")
 */
int LFReadEnvStr(lua_State* l)
{
	String EnvStrSrc = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	char szRet[1024] = {0};
	memset(szRet,0,sizeof(szRet));
	ExpandEnvironmentStringsA(EnvStrSrc.c_str(),szRet,sizeof(szRet));
	lua_pushstring(l,szRet);
	return 1;
}

/*
 * \brief LUA 导出接口RegIni.ReadINIStr的实现。RegIni.ReadINIStr从INI中读取一个键的值。
 * \param l LUA栈参数。栈中三个RegIni.ReadINIStr的参数：
 *  - 第一个参数是INI文件的路径
 *  - 第二个参数是INI文件的节名
 *  - 第三个参数是INI文件的键名
 * \return 总是1.代表RegIni.ReadINIStr有一个返回值。RegIni.ReadINIStr的返回值为字符串格式返回对应键的值。
 * \remark 
 * For example:
 * str = ReadINIStr("$INSTDIR\winamp.ini","winamp","outname")
*/
int LFReadINIStr(lua_State* l)
{
	String strIniFileName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strSectionName = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strEntryName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	char szRet[1024] = {0};
	GetPrivateProfileStringA(strSectionName.c_str(),strEntryName.c_str(),NULL,szRet,sizeof(szRet),strIniFileName.c_str());
	lua_pushstring(l,szRet);
	return 1;
}

/**
 * \brief LUA 导出接口RegIni.ReadRegDWORD的实现。RegIni.ReadRegDWORD从注册表中读取一个32位的DWORD值，
 * \param l LUA栈参数。栈中三个RegIni.ReadRegDWORD的参数：
 * - 根键(string)，有效的根键在WriteRegStr中有列出
 * - 子健路径(string)
 * - 键名(string)
 * \return 总是1，表示RegIni.ReadRegDWORD有一个返回值，RegIni.ReadRegDWORD返回的类型为string。
 * \remark 如果RegIni.ReadRegDWORD返回值为空字符串，说明获取的过程中发生了错误。
 * For example:
 * ReadRegDWORD("HKLM", "Software\\NSIS","VersionBuild")
 */

int LFReadRegDWORD(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strRegName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	std::string strRet = LuaExt::RegIni::ReadRegDWORD(strRegRoot,strRegSubKey,strRegName);
	lua_pushstring(l,strRet.c_str());
	return 1;
}

/**
 * \brief LUA 导出接口RegIni.ReadRegStr的实现。从注册表中读取字符串值。
 * \param l LUA栈参数。栈中三个RegIni.ReadRegStr的参数：
 * - 根键(string)，有效的根键在WriteRegStr中有列出
 * - 子健路径(string)
 * - 键名(string)
 * \return 总是1，表示RegIni.ReadRegStr有一个返回值，RegIni.ReadRegStr返回的类型为string。
 * \remark 如果RegIni.ReadRegStr返回值为空字符串，说明获取的过程中发生了错误。
 * For example:
 * 	rt = ReadRegStr("HKLM","Software\\NSIS","")
 * 	print("NSIS is installed at:" .. rt)
 */
int LFReadRegStr(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strRegName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	std::string strRet = LuaExt::RegIni::ReadRegStr(strRegRoot,strRegSubKey,strRegName);
	lua_pushstring(l,strRet.c_str());
	return 1;
}

/**
 * \brief LUA 导出接口RegIni.WriteINIStr的实现。从INI文件中写入字符串值。
 * \param l LUA栈参数。栈中四个RegIni.WriteINIStr的参数依次如下：
 * - INI文件的路径(string)
 * - INI的节名(string)
 * - 键名(string)
 * - 值(string)
 * \return 总是1，表示RegIni.WriteINIStr返回一个值。RegIni.WriteINIStr返回值为boolean类型的一个值代表操作是否成功。
 * \remark 
 *  For example:
 *  WriteINIStr("$TEMP\something.ini","section1","something","123")
 *  WriteINIStr("$TEMP\something.ini","section1","somethingelse","1234")
 *  WriteINIStr("$TEMP\something.ini","section2","nsis","true")
 */
int LFWriteINIStr(lua_State* l)
{
	String strIniFileName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strSectionName = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strEntryName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	String strValue = ((TempPtr = lua_tostring(l,4)) ? TempPtr : "");
	BOOL bRet = WritePrivateProfileStringA(strSectionName.c_str(),strEntryName.c_str(),strValue.c_str(),strIniFileName.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * \brief LUA 导出接口RegIni.WriteRegBin的实现。从注册表中写入二进制值。If the registry key doesn't exist it will be created.
 * \param l LUA栈参数。栈中四个RegIni.WriteRegBin的参数依次如下：
 * - 根键(string)，有效的根键在WriteRegStr中有列出
 * - 子健路径(string)
 * - 键名(string)
 * - 二进制值(string),Valuedata is in hexadecimal (e.g. DEADBEEF01223211151). 
 * \return 总是为1，代表有一个返回值。返回值的类型为boolean成功返回true失败返回false.
 * \remark 
 * For example:
 * WriteRegBin("HKLM","Software\My Company\My Software","Binary Value","DEADBEEF01223211151")
 */
int LFWriteRegBin(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strRegName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	String strValueData = ((TempPtr = lua_tostring(l,4)) ? TempPtr : "");
	BOOL bRet = LuaExt::RegIni::WriteRegBin(strRegRoot,strRegSubKey,strRegName,strValueData);
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * \brief LUA 导出接口RegIni.WriteRegDWORD的实现。往注册表中写入一个DWORD。If the registry key doesn't exist it will be created.
 * \param l LUA栈参数。栈中四个RegIni.WriteRegDWORD的参数依次如下：
 * - 根键(string)，有效的根键在WriteRegStr中有列出
 * - 子健路径(string)
 * - 键名(string)
 * - DWORD值(integer)
 * \return 总是为1，代表有一个返回值。返回值的类型为boolean成功返回true失败返回false.
 * \remark
 *	WriteRegDWORD("HKLM","Software\\My Company\\My Software","DWORD Value",0xDEADBEEF)
*/
int LFWriteRegDWORD(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strRegName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	DWORD dwValue = (DWORD)lua_tointeger(l,4);
	BOOL bRet = LuaExt::RegIni::WriteRegDWORD(strRegRoot,strRegSubKey,strRegName,dwValue);
	LOG_INFO("设置[%s][%s],[%s]=%d,结果=%d", strRegRoot.c_str(), strRegSubKey.c_str(), strRegName.c_str(), dwValue, bRet);
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * \brief LUA 导出接口RegIni.WriteRegStr的实现。往注册表中写入一个字符串。If the registry key doesn't exist it will be created.
 * \param l LUA栈参数。栈中四个RegIni.WriteRegStr的参数依次如下：
 * - 根键(string)，有效的根键在WriteRegStr中有列出
 * - 子健路径(string)
 * - 键名(string)
 * - 字符串值(string)
 * \return 总是为1，代表有一个返回值。返回值的类型为boolean成功返回true失败返回false.
 * \remark
 * root key must be one of:
 * - HKCR or HKEY_CLASSES_ROOT
 * - HKLM or HKEY_LOCAL_MACHINE
 * - HKCU or HKEY_CURRENT_USER
 * - HKUS or HKEY_USERS
 * - HKCC or HKEY_CURRENT_CONFIG
 * - HKDD or HKEY_DYN_DATA
 * - HKPD or HKEY_PERFORMANCE_DATA
 * For example:
 *	WriteRegStr( "HKLM" ,"Software\\My Company\\My Software" ,"String Value" ,"dead beef")
*/
int LFWriteRegStr(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strRegName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	String strValueData = ((TempPtr = lua_tostring(l,4)) ? TempPtr : "");
	BOOL bRet = LuaExt::RegIni::WriteRegStr(strRegRoot,strRegSubKey,strRegName,strValueData);
	LOG_INFO("写注册表字符串[%s][%s],[%s]=[%s],结果=%d", strRegRoot.c_str(), strRegSubKey.c_str(), strRegName.c_str(), strValueData.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * \brief LUA 导出接口RegIni.WriteRegExpandStr的实现。往注册表中写入一个Expand字符串。If the registry key doesn't exist it will be created.
 * The type of the string will be REG_SZ for WriteRegStr, or REG_EXPAND_STR for WriteRegExpandStr. 
 * If the registry key doesn't exist it will be created.
 * \param l LUA栈参数。栈中四个RegIni.WriteRegExpandStr的参数依次如下：
 * - 根键(string)，有效的根键在WriteRegStr中有列出
 * - 子健路径(string)
 * - 键名(string)
 * - 字符串值(string)
 * \return 总是为1，代表有一个返回值。返回值的类型为boolean成功返回true失败返回false.
 * \remark
 * For example:
 *	WriteRegExpandStr( "HKLM" ,"Software\My Company\My Software" ,"Expand String Value" ,"%WINDIR%\notepad.exe")
*/
int LFWriteRegExpandStr(lua_State* l)
{
	String strRegRoot = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	String strRegSubKey = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	String strRegName = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	String strValueData = ((TempPtr = lua_tostring(l,4)) ? TempPtr : "");
	BOOL bRet = LuaExt::RegIni::WriteRegExpandStr(strRegRoot,strRegSubKey,strRegName,strValueData);
	lua_pushboolean(l,bRet);
	return 1;
}

/*
 * \brief LUA 导出接口RegIni.SetRegView的实现。设置32位程序/64位程序注册表访问位置。
 * \param l LUA栈参数。栈中有一个参数，只能是32、64或者0。
 * \return 总是为0，代表RegIni.SetRegView没有返回值。
 * \remark
 * For example:
 *	SetRegView(64) --访问64位系统注册表目录
 *	SetRegView(32) --访问32为系统注册表目录
 *	SetRegView(0) --不指定，使用系统默认值。
 */
int LFSetRegView(lua_State* l)
{
	int RegView = lua_tointeger(l,1);
	if (RegView == 64)
	{
		LuaExt::RegIni::gs_alter_reg_view = KEY_WOW64_64KEY;
	}
	else if (RegView == 32)
	{
		LuaExt::RegIni::gs_alter_reg_view = KEY_WOW64_32KEY;
	}
	else if (RegView == 0)
	{
		//set to default value.
		LuaExt::RegIni::gs_alter_reg_view = 0;
	}
	return 0;
}

/**
 * Assumes the table is on top of the stack.
 */
static void set_info (lua_State *L) 
{
	lua_pushliteral(L, "Copyright (C) 2015 - 2016 Lua everywhere");
	lua_setfield(L, -2, "_COPYRIGHT");
	lua_pushliteral(L, "RegIni is a Lua library developed to encapsure common os function.");
	lua_setfield(L, -2, "_DESCRIPTION");
	lua_pushliteral(L, "RegIni " REGINI_VERSION);
	lua_setfield(L, -2, "_VERSION");
}

/**
 * \brief RgeIniLib结构体定义RgeIni模块LUA接口和内部实现的映射关系。
 * 每个成员的第一个是导出接口名，第二个是内部实现函数名。
 * 有关LUA接口的使用见内部实现明的说明。点击链接可以查看。
 */
static const struct luaL_Reg RgeIniLib[] = {
	{"ReadINIStr", LFReadINIStr},
	{"FlushINI", LFFlushINI},
	{"DeleteINISec", LFDeleteINISec},
	{"DeleteINIStr", LFDeleteINIStr},
	{"WriteINIStr", LFWriteINIStr},
	{"DeleteRegKey", LFDeleteRegKey},
	{"DeleteRegValue", LFDeleteRegValue},
	{"EnumRegKey", LFEnumRegKey},
	{"EnumRegValue", LFEnumRegValue},
	{"ReadRegDWORD", LFReadRegDWORD},
	{"ReadRegStr", LFReadRegStr},
	{"SetRegView", LFSetRegView},
	{"WriteRegBin", LFWriteRegBin},
	{"WriteRegDWORD", LFWriteRegDWORD},
	{"WriteRegStr", LFWriteRegStr},
	{"WriteRegExpandStr", LFWriteRegExpandStr},
	{"ReadEnvStr", LFReadEnvStr},
	{NULL, NULL},
};

/**
 * \} 
 */

#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

int luaopen_regini( lua_State* l )
{
	luaL_newlib (l, RgeIniLib);
	lua_pushvalue(l, -1);
	lua_setglobal(l, REGINI_LIBNAME);
	set_info (l);
	return 1;
}