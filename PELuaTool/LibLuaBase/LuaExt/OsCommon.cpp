/**
 * \file OsCommon.cpp 
 * \brief 常用的系统功能封装
 * \code
 * static const struct luaL_Reg fslib[] = {
 * {"Sleep", Lua_sleep},
 * {"MessageBox", Lua_MessageBoxA},
 * {"ForceCreateDir", Lua_ForceCreateDir},
 * {"ForceCreateFile", Lua_ForceCreateFile},
 * {"CreateMyDir", Lua_CreateMyDir},
 * {"IsDirCanCreateFile", Lua_IsDirCanCreateFile},
 * {"GetAppFullPath",Lua_GetAppFullPath},
 * {"GetAppPath", Lua_GetAppPath},
 * {"GetFileName", Lua_GetFileName},
 * {"GetPath", Lua_GetPath},
 * {"GetLastFolderName", Lua_GetLastFolderName},
 * {"GetParentPath", Lua_GetParentPath},
 * {"GetUrlFileName", Lua_GetUrlFileName},
 * {"GetUrlPath", Lua_GetUrlPath},
 * {"IsAllowDenyOpen", Lua_IsAllowDenyOpen},
 * {"DeepDeleteFile", Lua_DeepDeleteFile},
 * {"DeleteAllFiles", Lua_DeleteAllFiles},
 * {"RemoveDirectoryRecursively", Lua_RemoveDirectoryRecursively},
 * {"DirAddBackSlash", Lua_DirAddBackSlash},
 * {"CopyFolder", Lua_CopyFolder},
 * {"DeepCopyFile", Lua_DeepCopyFile},
 * {"SetClipboardText", Lua_SetClipboardText},
 * {"GetFileVersion", Lua_GetFileVersion},
 * {"GetFileInfo", Lua_GetFileInfo},
 * {"CompareVersion", Lua_CompareVersion},
 * {"GetIEVersion", Lua_GetIEVersion},
 * {"GetLastError", Lua_GetLastError},
 * {"PathFileExist", Lua_PathFileExist},
 * {"IsWow64",Lua_IsWow64},
 * {"DisableWow64FsRedir",Lua_DisableWow64FsRedir},
 * {"RestoreWow64FsRedir",Lua_RestoreWow64FsRedir},
 * {"EnumTopWindows",Lua_EnumTopWindows},
 * {"PostMessage",Lua_PostMessage},
 * {"SendMessage",Lua_SendMessage},
 * {"Exec",Lua_Exec},
 * {"ExecWait",Lua_ExecWait},
 * {"ShellExecute",Lua_ShellExecute},
 * {NULL, NULL},
 * };
 * \endcode
 *
 * \author Duo Core Fan
 * \date 20160813
 */

#include "Stdafx.h"
#include <string>
#include <sstream>
#include "util/StringEx.h"
#include "../SystemCommon.h"
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};
#include "OsCommon.h"
#include <shlwapi.h>
#include "util/OsVersion.h"
#include "util/File.h"
#include <shellapi.h>
#include <shlobj.h>
#include "../Macros.h"
#include "util/CommonWinFuns.h"
#include "util/Logger.h"

/**
 * \defgroup LUA_OSEXT LUA脚本:OsExt模块
 * \{
 */


/**
 * .1 Lua_sleep
 * 封装Sleep函数
 * 
 * For example:
 *	Sleep(100) --等待100毫秒
 */
static int LFSleep(lua_State *l)
{
	int Ti=lua_tointeger(l,1);
	Sleep(Ti);
	lua_pushinteger(l,1);
	return 1;
}

static const char* TempPtr = NULL;

/**
 * .2 MessageBox
 * 封装MessageBox，与Windows API的参数一一对应。
 *
 * For example:
 *		IDOK=1
 *		IDCANCEL=2
 *		IDABORT=3
 *		IDRETRY=4
 *		IDIGNORE=5
 *		IDYES=6
 *		IDNO=7
 *		MB_YESNO = 4
 *		iRet = MessageBox(0,"测试弹窗随便选","测试",MB_YESNO)
 *		print(iRet)
 *		if iRet == IDYES then
 *			print("选择了是")
 *		elseif iRet == IDNO then
 *			print("选择了否")
 *		elseif iRet == IDCANCEL then
 *			print("选择了取消")
 *		else
 *			print("未知的选项")
 *		end
 */
static int LFMessageBox(lua_State *l)
{
	int hWnd = lua_tointeger(l,1);
	std::string strContent = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	std::string strTitle = ((TempPtr = lua_tostring(l,3)) ? TempPtr : "");
	int option = lua_tointeger(l,4);
	int iRet = MessageBoxA((HWND)hWnd,strContent.c_str(),strTitle.c_str(),(UINT)option);
	lua_pushinteger(l,iRet);
	return 1;
}

/**
 * .3 ForceCreateDir 
 * 创建一个目录，如果此目录的上级目录不存在也将一块创建。
 * @param 1 : 
 *		[字符串]路径
 * @return :
 *		如果目录已经存在或者创建成功返回TRUE。创建失败返回FALSE，使用GetLastError可以获取错误码。
 * For example:
 *		BooleanVal = ForceCreateDir("D:\\Folder1\\Folder2\\Folder3")
 *		if BooleanVal == true then
 *			print "创建目录D:\\Folder1\\Folder2\\Folder3成功了"
 *		else
 *			print "创建目录D:\\Folder1\\Folder2\\Folder3失败了"
 *		end
 *		BooleanVal = ForceCreateDir("D:\\Folder1\\Folder2\\Folder3\\Folder4")
 *		if BooleanVal == true then
 *			print "创建目录D:\\Folder1\\Folder2\\Folder3成功了"
 *		else
 *			print "创建目录D:\\Folder1\\Folder2\\Folder3失败了"
 *		end
 */
static int LFCreateDirRecursively(lua_State *l)
{
	String strTargetPath = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wtrTargetPath = strTargetPath.toStdWString();
	os::FilePath::DirAddBackSlash(wtrTargetPath);
	BOOL bRet = os::FilePath::ForceCreateDir(wtrTargetPath.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .4 ForceCreateFile
 * 创建一个文件，如果此目录不存在那么将逐级创建
 * @param 1 : 
 *	[字符串]文件全路径
 * @return :
 *  文件创建成功返回TRUE,否则返回FALSE，使用GetLastError可以获取错误码。
 * For example:
 *		ForceCreateFile("D:\\Folder5\\Folder6\\1.txt")
 *		ForceCreateFile("D:\\Folder5\\Folder6\\2.bin")
 */
static int LFCreateFileRecursively(lua_State *l)
{
	String strTargetFile = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetFile = strTargetFile.toStdWString();
	BOOL bRet = os::FilePath::ForceCreateFile(wstrTargetFile.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .5 CreateMyDir
 * 创建目录,存在和创建成功返回true
 * @param 1 :
 *		[字符串]目录的路径,父目录必须存在不支持逐级创建。
 * @return :
 *		创建成功返回TRUE,否则返回FALSE，使用GetLastError可以获取错误码。
 * For example:
 *		CreateMyDir("D:\\Folder5\\Folder6")
 *		CreateMyDir("D:\\FolderNotExist\\Folder6")
 */
static int LFCreateDir(lua_State *l)
{
	String strTargetDir = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetDir = strTargetDir.toStdWString();
	os::FilePath::DirAddBackSlash(wstrTargetDir);
	BOOL bRet = os::FilePath::CreateMyDir(wstrTargetDir.c_str());
	LOG_INFO("创建目录%s,%d", strTargetDir.c_str(), bRet);
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .6 IsDirCanCreateFile
 * 判断一个目录是否为只读
 * @param 1 :
 *		[字符串]目录路径
 * @return : 
 *		只读返回TRUE,否则返回FALSE，目录不存在也返回FALSE。
 * For example:
 *		IsDirCanCreateFile("D:\\Folder1\\Folder2\\Folder3")
 */
static int LFIsDirReadOnly(lua_State *l)
{
	String strTargetDir = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetDir = strTargetDir.toStdWString();
	os::FilePath::DirAddBackSlash(wstrTargetDir);
	BOOL bRet = os::FilePath::IsDirCanCreateFile(wstrTargetDir.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * 获取当前实例的全路径，包括文件名
 */
static int LFGetAppFullPath(lua_State *l)
{
	TCHAR wszPath[MAX_PATH]={};
	::GetModuleFileName( GetModuleHandle(NULL), wszPath, MAX_PATH );
	std::string strAppPath = String::fromStdWString(wszPath);
	lua_pushstring(l,strAppPath.c_str());
	return 1;
}

/**
 * .7 GetAppPath
 * 获得当前实例的路径，如果有值一定以反斜杠结尾。
 * @return :
 *		返回当前程序的路径。
 * For example:
 *		StringVal = GetAppPath()
 *		print("Current Path is " .. StringVal)
 */
static int LFGetAppPath(lua_State *l)
{
	std::wstring wstrAppPath = os::FilePath::GetAppPath();
	std::string strAppPath = String::fromStdWString(wstrAppPath);
	lua_pushstring(l,strAppPath.c_str());
	return 1;
}

/**
 * .8 GetFileName
 * 从路径中分离文件名
 * @param 1 :
 *		[字符串]文件全路径
 * @return :
 *		文件名
 * For example:
 *		StringVal1 = GetAppPath()
 *		print("Current Path is " .. StringVal1)
 *		StringVal2 = GetFileName(StringVal1)
 *		print("My file name is " .. StringVal2);
 */
static int LFGetFileName(lua_State *l)
{
	String strPathName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrPathName = strPathName.toStdWString();
	std::wstring wstrFileName = os::FilePath::GetFileName(wstrPathName.c_str());
	std::string strFileNameRet = String::fromStdWString(wstrFileName);
	lua_pushstring(l,strFileNameRet.c_str());
	return 1;
}

/**
 * .9 GetPath
 * 从路径中分离路径名（去除文件名）
 * @param 1 :
 *		[字符串]文件全路径
 * @return :
 *		去除文件名之后的路径。
 * For example:
 *		StringVal1 = GetAppPath()
 *		print("Current Path is " .. StringVal1)
 *		StringVal2 = GetPath(StringVal1)
 *		print("My path is " .. StringVal2);
 */
static int LFGetPath(lua_State *l)
{
	String strPathName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrPathName = strPathName.toStdWString();
	std::wstring wstrPath = os::FilePath::GetPath(wstrPathName.c_str());
	std::string strPath = String::fromStdWString(wstrPath);
	lua_pushstring(l,strPath.c_str());
	return 1;
}

/**
 * .10 GetLastFolderName
 * 从路径中分离最后一个文件夹名（去除文件名）：
 * @param 1 :
 *		文件全路径
 * @return :
 *		路径中的最后一个文件夹
 * For example:
 *		StringVal1 = GetAppPath()
 *		print("Current Path is " .. StringVal1)
 *		StringVal2 = GetLastFolderName(StringVal1)
 *		print("My last folder name is " .. StringVal2);
 */

static int LFGetLastFolder(lua_State *l)
{
	String strPathName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrPathName = strPathName.toStdWString();
	std::wstring wstrLastFolderName = os::FilePath::GetLastFolderName(wstrPathName);
	std::string strLastFolderName = String::fromStdWString(wstrLastFolderName);
	lua_pushstring(l,strLastFolderName.c_str());
	return 1;
}

/**
 * .11 GetParentPath
 * 取一个路径的上一级目录
 * @param 1 :
 *		全路径
 * @return:
 * For example:
 *		StringVal1 = GetAppPath()
 *		print("Current Path is " .. StringVal1)
 *		StringVal3 = GetParentPath(StringVal1)
 *		print("parent of file is " .. StringVal3)
 *		StringVal2 = GetPath(StringVal1)
 *		print("My path is " .. StringVal2)
 *		StringVal3 = GetParentPath(StringVal2)
 *		print("parent of path is " .. StringVal3)
 */
static int LFGetParentPath(lua_State *l)
{
	String strPathName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrPathName = strPathName.toStdWString();
	std::wstring wstrParentPath = os::FilePath::GetParentPath(wstrPathName);
	std::string strParentPath = String::fromStdWString(wstrParentPath);
	lua_pushstring(l,strParentPath.c_str());
	return 1;
}

/**
 * .12 GetUrlFileName
 * 从Url路径中分离文件名
 * @param 1 :
 *		全路径
 * @return:
 *		Return file name which is contained in URL.
 * For example:
 *		FullUrl = "http://yun.dadajiasu.com/Public/package/201605/procexp.zip"
 *		FileName = GetUrlFileName(FullUrl)
 *		print("The file name part of url is " .. FileName)
 */
static int LFGetFileNameFromURL(lua_State *l)
{
	String strPathName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrPathName = strPathName.toStdWString();
	std::wstring wstrUrlFileName = os::FilePath::GetUrlFileName(wstrPathName);
	std::string strUrlFileName = String::fromStdWString(wstrUrlFileName);
	lua_pushstring(l,strUrlFileName.c_str());
	return 1;
}

/**
 * .13 GetUrlPath
 * 从Url路径中分离路径名（去除文件名）
 * @param 1 :
 *		URL全路径
 * @return :
 *		Return the URL without file name contained in input URL.
 * For example:
 *		FullUrl = "http://yun.dadajiasu.com/Public/package/201605/procexp.zip"
 *		PathName = GetUrlPath(FullUrl)
 *		print("The path part of url is " .. PathName)
 */
static int LFGetUrlPath(lua_State *l)
{
	String strPathName = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrPathName = strPathName.toStdWString();
	std::wstring wstrUrlPath = os::FilePath::GetUrlPath(wstrPathName);
	std::string strUrlPath = String::fromStdWString(wstrUrlPath);
	lua_pushstring(l,strUrlPath.c_str());
	return 1;
}

/**
 * .14 IsAllowDenyOpen
 * 判断一个文件是否可以用独占方式打开。使用此方法的前提是文件存在，否则可能导致错误的结果。
 * @param 1 :
 *		文件全路径
 * @return :
 *		TRUE 可以。FALSE 不能。
 * For example:
 *		FullPath = "C:\\Program Files (x86)\\Internet Explorer\\iexplore.exe"
 *		BooleanVal = IsAllowDenyOpen(FullPath)
 *		if BooleanVal == true then:
 *			print "IsAllowDenyOpen return TRUE"
 *		else
 *			print "IsAllowDenyOpen return FALSE"
 *		end
 */
static int LFIsFileCanExclusiveOpen(lua_State *l)
{
	String strTargetFile = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetFile = strTargetFile.toStdWString();
	BOOL bRet = os::FilePath::IsAllowDenyOpen(wstrTargetFile.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .15 DeepDeleteFile
 * 删除文件,如果文件被占用则尝试重命名并设定为重启后删除。注意，如果输入的文件不存在，同样返回TRUE。
 * @param 1 :
 *		文件全路径
 * @return :
 *		成功删除或者重命名设定重启后删除时返回TRUE，否则返回FALSE
 * For example:
 *		jsonStr = "this is test json"
 *		file = io.open("out.json", "w")
 *		if (file ~= nil) then
 *			file:write(jsonStr)
 *			file:close()
 *		end	
 *		DeepDeleteFile("out.json")
 */

static int LFDeepDeleteFile(lua_State *l)
{
	String strTargetFile = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetFile = strTargetFile.toStdWString();
	BOOL bRet = os::FilePath::DeepDeleteFile(wstrTargetFile.c_str());
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .16 DeleteAllFiles
 * 删除该文件夹下的所有文件
 * @param 1:
 *		目标文件夹路径
 * @return :
 *		无返回值
 * For example:
 *		TempFolder = os.getenv("TEMP")
 *		print "The temp folder is : " .. TempFolder
 *		if TempFolder == "TEMP" then
 *			print("Get system temp folder failed with error : %d" .. GetLastError())
 *		else
 *			DeleteAllFile(TempFolder)
 *		end
 */
static int LFDeleteAllFiles(lua_State *l)
{
	String strTargetPath = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetPath = strTargetPath.toStdWString();
	os::FilePath::DeleteAllFiles(wstrTargetPath.c_str());
	return 0;
}

/**
 * .17 RemoveDirectoryRecursively
 * 删除该文件夹下所有的子文件夹和所有文件，如果文件被占用则可能会失败。
 * 参数1 文件夹路径
 * 返回值:
 *		无返回值。
 *	For example:
 *		TargetFolder = "D:\\临时目录"
 *		print "Try to remove directory recursively,target is" .. TargetFolder
 *		RemoveDirectoryRecursively(TargetFolder)
 */
static int LFRemoveDirectoryRecursively(lua_State *l)
{
	String strTargetPath = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetPath = strTargetPath.toStdWString();
	os::FilePath::RemoveDirectoryRecursively(wstrTargetPath.c_str());
	return 0;
}

/**
 * .18 DirAddBackSlash
 * 在文件夹路径末尾加反斜线表示文件夹的结束（如果没有）
 * 参数1，文件夹路径
 * 返回值1 BOOL值表示成功或者失败
 * 返回值2 字符串值表示最终的字符串。
 * For example:
 *		TempFolder = os.getenv("TEMP")
 *		print ("The temp folder is : " .. TempFolder)
 *		BooleanValue,StringVal = DirAddBackSlash(TempFolder)
 *		if BooleanValue == true then
 *			print ("Dir add back slash successfully,the result is : " .. StringVal)
 *		else
 *			print ("Dir " .. TempFolder .. " add back slash failed with error : " .. GetLastError())
 *		end
 */
static int LFDirAddBackSlash(lua_State *l)
{
	String strTargetPath = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetPath = strTargetPath.toStdWString();
	BOOL bRet = os::FilePath::DirAddBackSlash(wstrTargetPath);
	std::string strRet = String::fromStdWString(wstrTargetPath);
	lua_pushboolean(l,bRet);
	lua_pushstring(l,strRet.c_str());
	return 2;
}

/**
 * .19 DirRemoveBackSlash
 * 在文件夹路径末尾移除反斜杠
 * 参数1，文件夹路径
 * 返回值1 BOOL值表示成功或者失败
 * 返回值2 字符串值表示最终的字符串。
 * For example:
 *		TempFolder = os.getenv("TEMP")
 *      TempFolder = TempFolder .. "\\"
 *		print ("The temp folder with back slash is : " .. TempFolder)
 *		BooleanValue,StringVal = DirRemoveBackSlash(TempFolder)
 *		if BooleanValue == true then
 *			print ("Dir remove back slash successfully,the result is : " .. StringVal)
 *		else
 *			print ("Dir " .. TempFolder .. " remove back slash failed with error : " .. GetLastError())
 *		end
 */
static int LFDirRemoveBackSlash(lua_State *l)
{
	String strTargetPath = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrTargetPath = strTargetPath.toStdWString();
	BOOL bRet = os::FilePath::DirRemoveBackSlash(wstrTargetPath);
	std::string strRet = String::fromStdWString(wstrTargetPath);
	lua_pushboolean(l,bRet);
	lua_pushstring(l,strRet.c_str());
	return 2;
}

/**
 * .19 CopyFolder
 * 拷贝该文件夹及下边的所有文件
 * 参数1 拷贝源目录
 * 参数2 目的目录
 * 参数3 强制覆盖
 * 返回 成功返回TRUE,失败返回FALSE
 * For example:
 *		SourceFolder = os.getenv("WINDIR")
 *		SourceFolder = SourceFolder .. "\\System32\\drivers"
 *		TargetFolder = GetAppPath();
 *		TargetFolder = GetPath(TargetFolder)
 *		BooleanVal,TargetFolder = DirAddBackSlash(TargetFolder)
 *		TargetFolder = TargetFolder .. "DriverBackup\\"
 *		CopyFolder(SourceFolder,TargetFolder,true)
 */
static int LFCopyFolder(lua_State *l)
{
	String strSource = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrSource = strSource.toStdWString();
	String strDest = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	std::wstring wstrDest = strDest.toStdWString();
	BOOL bForce = lua_toboolean(l,3);
	BOOL bRet = os::FilePath::CopyFolder(wstrSource,wstrDest,bForce);
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .20 DeepCopyFile
 * 深拷贝文件
 * 参数1 源文件路径
 * 参数2 目标文件路径
 * 返回值1 成功返回TRUE，失败返回FALSE。
 * 返回值2 如果目标已经存在，返回重命名后的文件
 * For example:
 *		SourceFolder = os.getenv("WINDIR")
 *		SourceFolder = SourceFolder .. "\\System32\\drivers"
 *		TargetFolder = GetAppPath();
 *		TargetFolder = GetPath(TargetFolder)
 *		BooleanVal,TargetFolder = DirAddBackSlash(TargetFolder)
 *		TargetFolder = TargetFolder .. "DriverBackup\\"
 *		SourceFilePath = SourceFolder .. "\\tcpip.sys"
 *		DeepCopyFile(SourceFilePath,TargetFolder,true)
 */
static int LFDeepCopyFile(lua_State *l)
{
	String strSource = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrSource = strSource.toStdWString();
	String strDest = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	std::wstring wstrDest = strDest.toStdWString();
	std::wstring wstrTemp;
	BOOL bRet = os::FilePath::DeepCopyFile(wstrSource,wstrDest,wstrTemp);
	std::string strRet = String::fromStdWString(wstrTemp);
	lua_pushboolean(l,bRet);
	lua_pushstring(l,strRet.c_str());
	return 2;
}

/**
 * .21 SetClipboardText
 * 设置指定窗口剪贴板的文本内容
 * 参数1 文本内容[字符串]
 * 参数2 窗口句柄[int]
 */
static int LFSetClipboardText(lua_State *l)
{
	String strContent = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrContent = strContent.toStdWString();
	HWND hWnd = (HWND)lua_tointeger(l,2);
	int iRet = os::FilePath::SetClipboardText(wstrContent.c_str(),hWnd);
	lua_pushinteger(l,iRet);
	return 1;
}

/**
 * .22 Boolean,String GetFileVersion(String)
 * 获得文件的版本号
 * 参数1 文件路径[字符串]
 * 返回值1 成功或者失败[布尔]
 * 返回值2 版本字符串
 * For example:
 *		Folder = os.getenv("WINDIR")
 *		TargetFile = Folder .. "\\System32\\drivers\\tcpip.sys"
 *		StringVal = GetFileVersion(TargetFile)
 *		print "Target version of tcpip.sys is : " .. StringVal
 */
static int LFGetFileVersion(lua_State *l)
{
	String strFile = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrFile = strFile.toStdWString();
	wchar_t szVersion[1024] = {0};
	memset(szVersion,0,sizeof(szVersion));
	BOOL bRet = os::FileVersion::GetFileVersion(wstrFile.c_str(), szVersion, sizeof(wstrFile)/sizeof(wchar_t));
	lua_pushboolean(l,bRet);
	std::string strRet = String::fromStdWString(szVersion);
	lua_pushstring(l,strRet.c_str());
	return 2;
}

/**
 * .23 GetFileInfo
 * 获得文件的信息
 * 参数1，文件路径[字符串]
 * 参数2，需要查询的文件信息[字符串]
 *			Comments	InternalName	ProductName
 *			CompanyName	LegalCopyright	ProductVersion
 *			FileDescription	LegalTrademarks	PrivateBuild
 *			FileVersion	OriginalFilename	SpecialBuild
 * 返回值1，查询结果TRUE OR FALSE.
 * 返回值2，查询文件信息的字符串结果。
 *
 * For example:
 *		Folder = os.getenv("WINDIR")
 *		TargetFile = Folder .. "\\System32\\drivers\\tcpip.sys"
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"Comments")
 *		print "Comments: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"InternalName")
 *		print "InternalName: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"ProductName")
 *		print "ProductName: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"CompanyName")
 *		print "CompanyName: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"LegalCopyright")
 *		print "LegalCopyright: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"ProductVersion")
 *		print "ProductVersion: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"FileDescription")
 *		print "FileDescription: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"LegalTrademarks")
 *		print "LegalTrademarks: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"PrivateBuild")
 *		print "PrivateBuild: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"FileVersion")
 *		print "FileVersion: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"OriginalFilename")
 *		print "OriginalFilename: " .. StringVal
 *		BooleanVal,StringVal = GetFileInfo(TargetFile,"SpecialBuild")
 *		print "SpecialBuild: " .. StringVal
 */

static int LFGetFileInfo(lua_State *l)
{
	String strFile = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrFile = strFile.toStdWString();
	String strInfoName = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	std::wstring wstrInfoName = strInfoName.toStdWString();
	wchar_t szInfo[1024] = {0};
	memset(szInfo,0,sizeof(szInfo));
	BOOL bRet = os::FileVersion::GetFileInfo(wstrFile.c_str(),wstrInfoName.c_str(),szInfo,sizeof(szInfo)/sizeof(wchar_t));
	std::string strRet = String::fromStdWString(szInfo);
	lua_pushboolean(l,bRet);
	lua_pushstring(l,strRet.c_str());
	return 2;
}

/**
 * .24 CompareVersion
 * 比较两个版本
 * 参数1 版本号1 [字符串]
 * 参数2 版本号2 [字符串]
 * 返回值 ： 版本号1>版本号2时返回1，版本号1<版本号2时返回-1，版本1=版本2号时返回0
 *
 * For example:
	 Folder = os.getenv("WINDIR")
	 TargetFile = Folder .. "\\System32\\drivers\\tcpip.sys"
	 BooleanValue,StringVal1 = OsExt.GetFileVersion(TargetFile)
	 TargetFile = Folder .. "\\System32\\drivers\\tcpipreg.sys"
	 BooleanValue,StringVal2 = OsExt.GetFileVersion(TargetFile)
	 print("The version of tcpip.sys is : " .. StringVal1 .. ".Version of tcpipreg.sys is : " .. StringVal2);
	 IntegerVal = OsExt.CompareVersion(StringVal1,StringVal2);
	 print("The compare result of them is : " )
	 StringVal1 = "1.0.0.1"
	 StringVal2 = "2.0.2.2"
	 StringVal3 = "2.0.2.2"
	 IntegerVal = OsExt.CompareVersion(StringVal1,StringVal2)
	 print("The result of compare " .. StringVal1 .. " and " .. StringVal2 .. " is " .. IntegerVal)
	 IntegerVal = OsExt.CompareVersion(StringVal3,StringVal2)
	 print("The result of compare " .. StringVal3 .. " and " .. StringVal2 .. " is " .. IntegerVal)
 */
static int LFCompareVersion(lua_State *l)
{
	String srcVersion = ((TempPtr = lua_tostring(l,1)) ? TempPtr : "");
	std::wstring wstrSrcVersion = srcVersion.toStdWString();
	String destVersion = ((TempPtr = lua_tostring(l,2)) ? TempPtr : "");
	std::wstring wstrDestVersion = destVersion.toStdWString();	
	int iComp = os::FileVersion::CompareVersion(wstrSrcVersion.c_str(),wstrDestVersion.c_str());
	lua_pushinteger(l,iComp);
	return 1;
}

/**
 * .25 GetIEVersion
 * 获取IE版本
 * For example :
 *		IntegerVal = GetIEVersion()
 *		print("The IE Version is " .. IntegerVal)
 */
static int LFGetIEVersion(lua_State *l)
{
	int iVersion = os::FileVersion::GetIEVersion();
	lua_pushinteger(l,iVersion);
	return 1;
}

/**
 * .26 GetLastError
 * 获取GetLastError
 * For example:
 *		jsonStr = "{"key":"value"}"
 *		file = io.open("some file not exist", "w")
 *		if (file ~= nil) then
 *			file:write(jsonStr)
 *			file:close()
 *		else
 *			print "open file failed with error : %d" .. GetLastError()
 *		end
 */
static int LFGetLastError(lua_State *l)
{
	DWORD dwLastError = GetLastError();
	lua_pushinteger(l,dwLastError);
	return 1;
}

/**
 * .27 Boolean PathFileExist(String)
 * 判断路径或者文件是否存在
 * 
 * For example;
 * TempFolder = os.getenv("TEMP")
 * BooleanValue = OsExt.PathFileExist(TempFolder)
 * if BooleanValue == false then
 * print( TempFolder .. "is not exist")
 * else
 * print( TempFolder .. "is exist")
 * end
 * TempFolder = "G:\\Foldernotexist\\somevalue"
 * BooleanValue = OsExt.PathFileExist(TempFolder)
 * if BooleanValue == false then
 * print( TempFolder .. " is not exist")
 * else
 * print( TempFolder .. " is exist")
 * end
 */
BOOL IsFileExistA(const char* filename)
{
	return File::exists(filename);
}


static int LFPathFileExist(lua_State *l)
{
	const char* pInputStr = lua_tostring(l,1);
	BOOL bRet = FALSE;
	if (pInputStr)
	{
		bRet = IsFileExistA(pInputStr);
		LOG_INFO("%s，返回%d", pInputStr, bRet);
//		bRet = PathFileExistsA(pInputStr);
	}
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .28 Boolean IsWow64(VOID)
 * 判断是否是WOW64进程，对于32位程序来说可以用这个函数判断是否运行在64位系统上。
 * For example:
 BooleanValue = IsWow64()
 if BooleanValue == true then
	print "I am a WOW64 process."
 else
	print "I am not a WOW64 process."
 end
 */
static int LFIsWow64(lua_State *l)
{
	BOOL bRet = isWow64();
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .29 VOID DisableWow64FsRedir(VOID)
 * 禁用WOW64进程的重定向，仅适用于WOW64进程
 * For example:
 DisableWow64FsRedir()
 */
static int LFDisableWow64FsRedir(lua_State *l)
{
	BOOL bRet = DisableWow64FsRedir();
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .30 VOID RestoreWow64FsRedir(VOID)
 * 重设WOW64进程的重定向，仅适用于WOW64进程
 * For example:
 RestoreWow64FsRedir()
 */
static int LFRestoreWow64FsRedir(lua_State *l)
{
	BOOL bRet = RestoreWow64FsRedir();
	lua_pushboolean(l,bRet);
	return 1;
}

static std::vector<HWND> WndHandleVector;
BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	wchar_t *TargetName = (wchar_t*)lParam;
 	if ( GetParent(hwnd) == NULL ) /*&& IsWindowVisible(hwnd) &&
 		(::GetWindowLong(hwnd, GWL_EXSTYLE)&WS_EX_TOOLWINDOW)!=WS_EX_TOOLWINDOW)
		&&::GetWindowLong(hwnd, GWL_HWNDPARENT)==0)								//判断窗口是否是可见、顶层 */
	{
		TCHAR str[256] = {0};
		::GetWindowText(hwnd,str,sizeof(str));                                  //获取窗口的名称
		if (0 == _wcsicmp(TargetName,str))
		{
			WndHandleVector.push_back(hwnd);
		}
	}
	return 1;
}

/**
 * .31 table EnumTopWindows()
 * 枚举所有顶级窗口，包括不可见的窗口。返回的table中,key为序号，value为句柄。
 * For example:
 local TableOfEnumRegKey = EnumTopWindows("My window name")
 for key,value in pairs(TableOfEnumRegKey) do
	print("key: " .. tostring(key) .. ", value: " .. tostring(value))
 end
 */
int LFEnumTopWindows(lua_State *l)
{
	WndHandleVector.clear();

	const char* szWndName = lua_tostring(l,1);
	if (szWndName)
	{
		std::wstring wstr = String(szWndName).toStdWString();
		EnumWindows(lpEnumFunc,(LPARAM)wstr.c_str());
	}
	/* create table. */  
	lua_newtable(l);  
	/* push (key, value). */ 
	for (int i=0; i!=WndHandleVector.size(); i++)
	{
		lua_pushinteger(l,i+1); //从1开始
		lua_pushinteger(l,(lua_Integer)WndHandleVector[i]);
		lua_settable(l,-3);
	}
	/* deal return. */
	return 1;
}

/**
 * .32 BOOLEAN PostMessage(Integer hWnd, Integer Msg, Integer wParam, Integer lParam)
 * 向一个窗口发送消息(Post)
 * For example:
 local TableOfEnumRegKey = EnumTopWindows("My window name")
 for key,value in pairs(TableOfEnumRegKey) do
	PostMessage(value,WM_CLOSE,0,0);
 end
 */
int LFPostMessage(lua_State *l)
{
	HWND hWnd = (HWND)lua_tointeger(l,1);
	UINT Msg = (UINT)lua_tointeger(l,2);
	WPARAM wParam = (WPARAM)lua_tointeger(l,3);
	LPARAM lParam = (LPARAM)lua_tointeger(l,4);
	BOOL bRet = PostMessageW(hWnd,Msg,wParam,lParam);
	lua_pushboolean(l,bRet);
	return 1;
}
/**
 * .33 BOOLEAN SendMessage(Integer hWnd, Integer Msg, Integer wParam, Integer lParam)
 * 向一个窗口发送消息(Send)
 * For example:
 local TableOfEnumRegKey = EnumTopWindows("My window name")
 for key,value in pairs(TableOfEnumRegKey) do
	SendMessage(value,WM_CLOSE,0,0);
 end
 */
int LFSendMessage(lua_State *l)
{
	HWND hWnd = (HWND)lua_tointeger(l,1);
	UINT Msg = (UINT)lua_tointeger(l,2);
	WPARAM wParam = (WPARAM)lua_tointeger(l,3);
	LPARAM lParam = (LPARAM)lua_tointeger(l,4);
	BOOL bRet = SendMessageW(hWnd,Msg,wParam,lParam);
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * 执行子程序，如果指定了等待执行完毕，则可取得子进程的标准输出。
 * 返回值为子进程的退出码，如果未等待执行完毕就返回则返回255
 */
INT ExecSubProcess(LPCWSTR szCmd, WORD ShowWnd, DWORD waitTime, OUT string& Result)
{
	if (!szCmd || !szCmd[0])
		return -1;
	//子进程启动信息设置
	WCHAR szTempPath[MAX_PATH] = { 0 };
	GetTempPath(MAX_PATH, szTempPath);
	WCHAR szTempFileName[MAX_PATH] = { 0 };
	GetTempFileName(szTempPath, L"~l", 0, szTempFileName);
	//子进程启动信息设置
	STARTUPINFOW si;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfoW(&si);
	si.wShowWindow = ShowWnd;
	si.dwFlags = STARTF_USESHOWWINDOW;
	HANDLE hOutPutFile = 0;
	if (szTempFileName[0])
	{
		si.dwFlags = si.dwFlags | STARTF_USESTDHANDLES;
		SECURITY_ATTRIBUTES psa = { sizeof(psa),NULL,TRUE };;
		psa.bInheritHandle = TRUE;
		hOutPutFile = CreateFile(szTempFileName, GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, &psa,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		si.hStdOutput = hOutPutFile;
		si.hStdError = hOutPutFile;
	}
	// 运行子进程并等待其结束
	PROCESS_INFORMATION pi;
	// 	MessageBox(NULL,FullCmdLine,NULL,MB_OK);
	// 	return 0;
	BOOL flag = CreateProcessW(NULL, (LPWSTR)szCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	if (!flag)
	{
		LOG_INFO("执行命令%s时发生了错误，错误ID为%d：", String::fromStdWString((LPCWSTR)szCmd).c_str(), GetLastError());
		return GetLastError();
	}

	DWORD dwExitCode = 255;

	if (waitTime != 0)
	{
		DWORD dwWait = WaitForSingleObject(pi.hProcess, waitTime);
		if (dwWait == WAIT_OBJECT_0)
		{
			GetExitCodeProcess(pi.hProcess, &dwExitCode);
			if (hOutPutFile)
			{
				CloseHandle(hOutPutFile);
				hOutPutFile = NULL;
			}
			//
			// 有些进程的输出的字符串比较特殊，不一定是ANSI字符串，不加转换返回原始数据，让调用者拿到数据后自行转换。
			//
			FileReader fr(String::fromStdWString(szTempFileName));
			if (fr.open())
			{
				Result = fr.read();
			}
			LOG_INFO("子进程[%s]退出返回[%d],标准输出：", String::fromStdWString(szCmd).c_str(), dwExitCode);
			LOG_INFO("%s", Result.c_str());
		}
		else if (dwWait == WAIT_TIMEOUT)
		{
			LOG_INFO("子进程[%s]执行中……", String::fromStdWString(szCmd).c_str());
		}
		else
		{
			LOG_ERROR("等待子进程[%s]时返回一个意料之外的值[%d]", String::fromStdWString(szCmd).c_str(), dwWait);
		}
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if (hOutPutFile)
	{
		CloseHandle(hOutPutFile);
		hOutPutFile = NULL;
	}

	return dwExitCode;
}


/**
 * .34 Exec(CommandLine,SW_SHOW/SW_HIDE)
 * 执行一个程序，但不等待它执行完毕就返回。
 */
int LFExec(lua_State *l)
{
	const char* CommandLine = lua_tostring(l, 1);
	WORD ShowWindow = LOWORD(lua_tointeger(l, 2));
	std::wstring wstr = String(CommandLine).toStdWString();
	string result;
	INT iExitCode = ExecSubProcess(wstr.c_str(), ShowWindow, 0, result);
	LOG_INFO("[%s],[%d]返回[%d]", CommandLine, ShowWindow, iExitCode);
	lua_pushboolean(l, iExitCode);
	return 1;
}



/**
 * .35 ExecWait(CommandLine,SW_SHOW/SW_HIDE)
 * 执行一个程序并等待它执行完毕
 */
int LFExecWait(lua_State *l)
{
	const char* CommandLine = lua_tostring(l,1);
	WORD ShowWindow = LOWORD(lua_tointeger(l,2));
	std::wstring wstr = String(CommandLine).toStdWString();
	string result;
	INT iExitCode = ExecSubProcess(wstr.c_str(), ShowWindow, INFINITE, result);
	LOG_INFO("[%s],[%d]返回[%d]", CommandLine, ShowWindow, iExitCode);
	lua_pushboolean(l, iExitCode);
	return 1;
}

int LFExecGetStdout(lua_State *l)
{
	const char* CommandLine = lua_tostring(l,1);
	WORD ShowWindow = LOWORD(lua_tointeger(l,2));
	string result;
	std::wstring wstr = String(CommandLine).toStdWString();
	INT iExitCode = ExecSubProcess(wstr.c_str(), ShowWindow, INFINITE, result);
	LOG_INFO("[%s],[%d]返回[%d]", CommandLine, ShowWindow, iExitCode);
	lua_pushboolean(l, iExitCode);
	lua_pushstring(l, result.c_str());
	return 2;
}

/**
 * .36 ShellExecute
 * 封装API ShellExecute
 *
 */
int LFShellExecute(lua_State *l)
{
	HWND hwnd = (HWND)lua_tointeger(l,1);
	LPCSTR lpOperation = lua_tostring(l,2);
	LPCSTR lpFile = lua_tostring(l,3);
	LPCSTR lpParameters = lua_tostring(l,4);
	LPCSTR lpDirectory = lua_tostring(l,5);
	INT nShowCmd = lua_tointeger(l,6);

	HINSTANCE ins = ShellExecuteA(hwnd, lpOperation, lpFile, lpParameters,lpDirectory, nShowCmd);
	lua_pushinteger(l,(lua_Integer)ins);
	return 1;
}

/**
 * \brief 替换输入字符串中的宏为它指定的目录。
 */
int LFReplacePathMacro(lua_State *l)
{
	const char* pInputString = lua_tostring(l,1);
	std::string outString = os::ReplacePathMacro(pInputString);
	lua_pushstring(l,outString.c_str());
	return 1;
}


int LFGetSpecialFolder(lua_State *l)
{
	int FolderId = lua_tointeger(l, 1);
	char szDir[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, szDir, FolderId,FALSE);
	LOG_INFO("取得ID=%d的目录%s", FolderId, szDir);
	lua_pushstring(l, szDir);
	return 1;
}

/**
 * \brief 创建一个EXE的快捷方式到指定的位置。
 */
int LFCreateFileShortcut(lua_State *l)
{
	const char* lpszLnkFileFullPath = lua_tostring(l,1);
	const char* lpszTargetExeFullPath = lua_tostring(l,2);
	if (lpszLnkFileFullPath == NULL || lpszTargetExeFullPath== NULL)
	{
		lua_pushboolean(l, FALSE);
		return 0;
	}
	BOOL bRet = os::CreateFileShortcut(String(lpszLnkFileFullPath).toStdWString().c_str(),
							String(lpszTargetExeFullPath).toStdWString().c_str());
	LOG_INFO("创建快捷方式[%s]==>[%s]", lpszTargetExeFullPath, lpszLnkFileFullPath);
	lua_pushboolean(l,bRet);
	return 1;
}

int LFKillProcessByName(lua_State *l)
{
	const char* lpszProcessNameToKill = lua_tostring(l, 1);
	LOG_INFO("杀死进程[%s]", lpszProcessNameToKill);
	BOOL bKilled = KillProcessByName(String(lpszProcessNameToKill).toStdWString().c_str());
	lua_pushboolean(l,bKilled);
	return 1;
}

/**
 * \brief 通知系统已经修改了注册表中打开文件的关联程序。（PS:可以起到刷新Explorer.exe的作用）
 */
int LFShellNotifyAssoChanged(lua_State *l)
{
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	return 0;
}

int LFStopService(lua_State* l)
{
	const char* lpszServiceName = lua_tostring(l,1);
	BOOL bRet = StopServiceByName(String(lpszServiceName).toStdWString().c_str());
	LOG_INFO("停止服务[%s]%s", lpszServiceName, bRet?"成功":"失败");
	lua_pushboolean(l, bRet);
	return 1;
}

int LFStartService(lua_State* l)
{
	const char* lpszServiceName = lua_tostring(l, 1);
	BOOL bRet = StartServiceByName(String(lpszServiceName).toStdWString().c_str());
	LOG_INFO("开始服务[%s]%s", lpszServiceName, bRet ? "成功" : "失败");
	lua_pushboolean(l, bRet);
	return 1;
}

#define OSEXT_VERSION "1.0.0.1"
#define OSEXT_LIBNAME "OsExt"
/*
** Assumes the table is on top of the stack.
*/
static void set_info(lua_State *L)
{
	lua_pushliteral(L, "Copyright (C) 2015 - 2016 Lua everywhere");
	lua_setfield(L, -2, "_COPYRIGHT");
	lua_pushliteral(L, "OsExt is a Lua library developed to encapsure common os function.");
	lua_setfield(L, -2, "_DESCRIPTION");
	lua_pushliteral(L, "OsExt " OSEXT_VERSION);
	lua_setfield(L, -2, "_VERSION");
}
/**
 * \brief OsExtLib结构体定义OsExt模块LUA接口和内部实现的映射关系。
 * 每个成员的第一个是导出接口名，第二个是内部实现函数名。
 * 有关LUA接口的使用见内部实现明的说明。点击链接可以查看。
 */
static const struct luaL_Reg OsExtLib[] = {
	{"Sleep", LFSleep},
	{"MessageBox", LFMessageBox},
	{"CreateDirRecursively", LFCreateDirRecursively},
	{"CreateFileRecursively", LFCreateFileRecursively},
	{"CreateDir", LFCreateDir},
	{"IsDirReadOnly", LFIsDirReadOnly},
	{"GetAppFullPath",LFGetAppFullPath},
	{"GetAppPath", LFGetAppPath},
	{"GetFileName", LFGetFileName},
	{"GetPath", LFGetPath},
	{"GetLastFolder", LFGetLastFolder},
	{"GetParentPath", LFGetParentPath},
	{"GetUrlFileName", LFGetFileNameFromURL},
	{"GetUrlPath", LFGetUrlPath},
	{"IsFileCanExclusiveOpen", LFIsFileCanExclusiveOpen},
	{"DeepDeleteFile", LFDeepDeleteFile},
	{"DeleteAllFiles", LFDeleteAllFiles},
	{"RemoveDirectoryRecursively", LFRemoveDirectoryRecursively},
	{"DirAddBackSlash", LFDirAddBackSlash},
	{"CopyFolder", LFCopyFolder},
	{"DeepCopyFile", LFDeepCopyFile},
	{"SetClipboardText", LFSetClipboardText},
	{"GetFileVersion", LFGetFileVersion},
	{"GetFileInfo", LFGetFileInfo},
	{"CompareVersion", LFCompareVersion},
	{"GetIEVersion", LFGetIEVersion},
	{"GetLastError", LFGetLastError},
	{"PathFileExist", LFPathFileExist},
	{"IsWow64",LFIsWow64},
	{"DisableWow64FsRedir",LFDisableWow64FsRedir},
	{"RestoreWow64FsRedir",LFRestoreWow64FsRedir},
	{"EnumTopWindows",LFEnumTopWindows},
	{"PostMessage",LFPostMessage},
	{"SendMessage",LFSendMessage},
	{"Exec",LFExec},
	{"ExecWait",LFExecWait},
	{"ExecGetStdout",LFExecGetStdout},
	{"ShellExecute",LFShellExecute},
	{"ReplacePathMacro",LFReplacePathMacro},
	{"CreateShortCut",LFCreateFileShortcut},
	{"DirRemoveBackSlash",LFDirRemoveBackSlash},
	{"GetSpecialFolder",LFGetSpecialFolder },
	{"ShellNotifyAssoChanged",LFShellNotifyAssoChanged},
	{"KillProcessByName",LFKillProcessByName},
	{"StopService", LFStopService },
	{"StartService", LFStartService},
	{NULL, NULL},
};

/**
 * \} 
 */

#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

int luaopen_OsCommon( lua_State* l )
{
	luaL_newlib (l, OsExtLib);
	lua_pushvalue(l, -1);
	lua_setglobal(l, OSEXT_LIBNAME);
	set_info (l);
	return 1;
}
