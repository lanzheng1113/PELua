/**
 * \file OsCommon.cpp 
 * \brief ���õ�ϵͳ���ܷ�װ
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
 * \defgroup LUA_OSEXT LUA�ű�:OsExtģ��
 * \{
 */


/**
 * .1 Lua_sleep
 * ��װSleep����
 * 
 * For example:
 *	Sleep(100) --�ȴ�100����
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
 * ��װMessageBox����Windows API�Ĳ���һһ��Ӧ��
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
 *		iRet = MessageBox(0,"���Ե������ѡ","����",MB_YESNO)
 *		print(iRet)
 *		if iRet == IDYES then
 *			print("ѡ������")
 *		elseif iRet == IDNO then
 *			print("ѡ���˷�")
 *		elseif iRet == IDCANCEL then
 *			print("ѡ����ȡ��")
 *		else
 *			print("δ֪��ѡ��")
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
 * ����һ��Ŀ¼�������Ŀ¼���ϼ�Ŀ¼������Ҳ��һ�鴴����
 * @param 1 : 
 *		[�ַ���]·��
 * @return :
 *		���Ŀ¼�Ѿ����ڻ��ߴ����ɹ�����TRUE������ʧ�ܷ���FALSE��ʹ��GetLastError���Ի�ȡ�����롣
 * For example:
 *		BooleanVal = ForceCreateDir("D:\\Folder1\\Folder2\\Folder3")
 *		if BooleanVal == true then
 *			print "����Ŀ¼D:\\Folder1\\Folder2\\Folder3�ɹ���"
 *		else
 *			print "����Ŀ¼D:\\Folder1\\Folder2\\Folder3ʧ����"
 *		end
 *		BooleanVal = ForceCreateDir("D:\\Folder1\\Folder2\\Folder3\\Folder4")
 *		if BooleanVal == true then
 *			print "����Ŀ¼D:\\Folder1\\Folder2\\Folder3�ɹ���"
 *		else
 *			print "����Ŀ¼D:\\Folder1\\Folder2\\Folder3ʧ����"
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
 * ����һ���ļ��������Ŀ¼��������ô���𼶴���
 * @param 1 : 
 *	[�ַ���]�ļ�ȫ·��
 * @return :
 *  �ļ������ɹ�����TRUE,���򷵻�FALSE��ʹ��GetLastError���Ի�ȡ�����롣
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
 * ����Ŀ¼,���ںʹ����ɹ�����true
 * @param 1 :
 *		[�ַ���]Ŀ¼��·��,��Ŀ¼������ڲ�֧���𼶴�����
 * @return :
 *		�����ɹ�����TRUE,���򷵻�FALSE��ʹ��GetLastError���Ի�ȡ�����롣
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
	LOG_INFO("����Ŀ¼%s,%d", strTargetDir.c_str(), bRet);
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .6 IsDirCanCreateFile
 * �ж�һ��Ŀ¼�Ƿ�Ϊֻ��
 * @param 1 :
 *		[�ַ���]Ŀ¼·��
 * @return : 
 *		ֻ������TRUE,���򷵻�FALSE��Ŀ¼������Ҳ����FALSE��
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
 * ��ȡ��ǰʵ����ȫ·���������ļ���
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
 * ��õ�ǰʵ����·���������ֵһ���Է�б�ܽ�β��
 * @return :
 *		���ص�ǰ�����·����
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
 * ��·���з����ļ���
 * @param 1 :
 *		[�ַ���]�ļ�ȫ·��
 * @return :
 *		�ļ���
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
 * ��·���з���·������ȥ���ļ�����
 * @param 1 :
 *		[�ַ���]�ļ�ȫ·��
 * @return :
 *		ȥ���ļ���֮���·����
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
 * ��·���з������һ���ļ�������ȥ���ļ�������
 * @param 1 :
 *		�ļ�ȫ·��
 * @return :
 *		·���е����һ���ļ���
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
 * ȡһ��·������һ��Ŀ¼
 * @param 1 :
 *		ȫ·��
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
 * ��Url·���з����ļ���
 * @param 1 :
 *		ȫ·��
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
 * ��Url·���з���·������ȥ���ļ�����
 * @param 1 :
 *		URLȫ·��
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
 * �ж�һ���ļ��Ƿ�����ö�ռ��ʽ�򿪡�ʹ�ô˷�����ǰ�����ļ����ڣ�������ܵ��´���Ľ����
 * @param 1 :
 *		�ļ�ȫ·��
 * @return :
 *		TRUE ���ԡ�FALSE ���ܡ�
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
 * ɾ���ļ�,����ļ���ռ���������������趨Ϊ������ɾ����ע�⣬���������ļ������ڣ�ͬ������TRUE��
 * @param 1 :
 *		�ļ�ȫ·��
 * @return :
 *		�ɹ�ɾ�������������趨������ɾ��ʱ����TRUE�����򷵻�FALSE
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
 * ɾ�����ļ����µ������ļ�
 * @param 1:
 *		Ŀ���ļ���·��
 * @return :
 *		�޷���ֵ
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
 * ɾ�����ļ��������е����ļ��к������ļ�������ļ���ռ������ܻ�ʧ�ܡ�
 * ����1 �ļ���·��
 * ����ֵ:
 *		�޷���ֵ��
 *	For example:
 *		TargetFolder = "D:\\��ʱĿ¼"
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
 * ���ļ���·��ĩβ�ӷ�б�߱�ʾ�ļ��еĽ��������û�У�
 * ����1���ļ���·��
 * ����ֵ1 BOOLֵ��ʾ�ɹ�����ʧ��
 * ����ֵ2 �ַ���ֵ��ʾ���յ��ַ�����
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
 * ���ļ���·��ĩβ�Ƴ���б��
 * ����1���ļ���·��
 * ����ֵ1 BOOLֵ��ʾ�ɹ�����ʧ��
 * ����ֵ2 �ַ���ֵ��ʾ���յ��ַ�����
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
 * �������ļ��м��±ߵ������ļ�
 * ����1 ����ԴĿ¼
 * ����2 Ŀ��Ŀ¼
 * ����3 ǿ�Ƹ���
 * ���� �ɹ�����TRUE,ʧ�ܷ���FALSE
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
 * ����ļ�
 * ����1 Դ�ļ�·��
 * ����2 Ŀ���ļ�·��
 * ����ֵ1 �ɹ�����TRUE��ʧ�ܷ���FALSE��
 * ����ֵ2 ���Ŀ���Ѿ����ڣ���������������ļ�
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
 * ����ָ�����ڼ�������ı�����
 * ����1 �ı�����[�ַ���]
 * ����2 ���ھ��[int]
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
 * ����ļ��İ汾��
 * ����1 �ļ�·��[�ַ���]
 * ����ֵ1 �ɹ�����ʧ��[����]
 * ����ֵ2 �汾�ַ���
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
 * ����ļ�����Ϣ
 * ����1���ļ�·��[�ַ���]
 * ����2����Ҫ��ѯ���ļ���Ϣ[�ַ���]
 *			Comments	InternalName	ProductName
 *			CompanyName	LegalCopyright	ProductVersion
 *			FileDescription	LegalTrademarks	PrivateBuild
 *			FileVersion	OriginalFilename	SpecialBuild
 * ����ֵ1����ѯ���TRUE OR FALSE.
 * ����ֵ2����ѯ�ļ���Ϣ���ַ��������
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
 * �Ƚ������汾
 * ����1 �汾��1 [�ַ���]
 * ����2 �汾��2 [�ַ���]
 * ����ֵ �� �汾��1>�汾��2ʱ����1���汾��1<�汾��2ʱ����-1���汾1=�汾2��ʱ����0
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
 * ��ȡIE�汾
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
 * ��ȡGetLastError
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
 * �ж�·�������ļ��Ƿ����
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
		LOG_INFO("%s������%d", pInputStr, bRet);
//		bRet = PathFileExistsA(pInputStr);
	}
	lua_pushboolean(l,bRet);
	return 1;
}

/**
 * .28 Boolean IsWow64(VOID)
 * �ж��Ƿ���WOW64���̣�����32λ������˵��������������ж��Ƿ�������64λϵͳ�ϡ�
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
 * ����WOW64���̵��ض��򣬽�������WOW64����
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
 * ����WOW64���̵��ض��򣬽�������WOW64����
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
		&&::GetWindowLong(hwnd, GWL_HWNDPARENT)==0)								//�жϴ����Ƿ��ǿɼ������� */
	{
		TCHAR str[256] = {0};
		::GetWindowText(hwnd,str,sizeof(str));                                  //��ȡ���ڵ�����
		if (0 == _wcsicmp(TargetName,str))
		{
			WndHandleVector.push_back(hwnd);
		}
	}
	return 1;
}

/**
 * .31 table EnumTopWindows()
 * ö�����ж������ڣ��������ɼ��Ĵ��ڡ����ص�table��,keyΪ��ţ�valueΪ�����
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
		lua_pushinteger(l,i+1); //��1��ʼ
		lua_pushinteger(l,(lua_Integer)WndHandleVector[i]);
		lua_settable(l,-3);
	}
	/* deal return. */
	return 1;
}

/**
 * .32 BOOLEAN PostMessage(Integer hWnd, Integer Msg, Integer wParam, Integer lParam)
 * ��һ�����ڷ�����Ϣ(Post)
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
 * ��һ�����ڷ�����Ϣ(Send)
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
 * ִ���ӳ������ָ���˵ȴ�ִ����ϣ����ȡ���ӽ��̵ı�׼�����
 * ����ֵΪ�ӽ��̵��˳��룬���δ�ȴ�ִ����Ͼͷ����򷵻�255
 */
INT ExecSubProcess(LPCWSTR szCmd, WORD ShowWnd, DWORD waitTime, OUT string& Result)
{
	if (!szCmd || !szCmd[0])
		return -1;
	//�ӽ���������Ϣ����
	WCHAR szTempPath[MAX_PATH] = { 0 };
	GetTempPath(MAX_PATH, szTempPath);
	WCHAR szTempFileName[MAX_PATH] = { 0 };
	GetTempFileName(szTempPath, L"~l", 0, szTempFileName);
	//�ӽ���������Ϣ����
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
	// �����ӽ��̲��ȴ������
	PROCESS_INFORMATION pi;
	// 	MessageBox(NULL,FullCmdLine,NULL,MB_OK);
	// 	return 0;
	BOOL flag = CreateProcessW(NULL, (LPWSTR)szCmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
	if (!flag)
	{
		LOG_INFO("ִ������%sʱ�����˴��󣬴���IDΪ%d��", String::fromStdWString((LPCWSTR)szCmd).c_str(), GetLastError());
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
			// ��Щ���̵�������ַ����Ƚ����⣬��һ����ANSI�ַ���������ת������ԭʼ���ݣ��õ������õ����ݺ�����ת����
			//
			FileReader fr(String::fromStdWString(szTempFileName));
			if (fr.open())
			{
				Result = fr.read();
			}
			LOG_INFO("�ӽ���[%s]�˳�����[%d],��׼�����", String::fromStdWString(szCmd).c_str(), dwExitCode);
			LOG_INFO("%s", Result.c_str());
		}
		else if (dwWait == WAIT_TIMEOUT)
		{
			LOG_INFO("�ӽ���[%s]ִ���С���", String::fromStdWString(szCmd).c_str());
		}
		else
		{
			LOG_ERROR("�ȴ��ӽ���[%s]ʱ����һ������֮���ֵ[%d]", String::fromStdWString(szCmd).c_str(), dwWait);
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
 * ִ��һ�����򣬵����ȴ���ִ����Ͼͷ��ء�
 */
int LFExec(lua_State *l)
{
	const char* CommandLine = lua_tostring(l, 1);
	WORD ShowWindow = LOWORD(lua_tointeger(l, 2));
	std::wstring wstr = String(CommandLine).toStdWString();
	string result;
	INT iExitCode = ExecSubProcess(wstr.c_str(), ShowWindow, 0, result);
	LOG_INFO("[%s],[%d]����[%d]", CommandLine, ShowWindow, iExitCode);
	lua_pushboolean(l, iExitCode);
	return 1;
}



/**
 * .35 ExecWait(CommandLine,SW_SHOW/SW_HIDE)
 * ִ��һ�����򲢵ȴ���ִ�����
 */
int LFExecWait(lua_State *l)
{
	const char* CommandLine = lua_tostring(l,1);
	WORD ShowWindow = LOWORD(lua_tointeger(l,2));
	std::wstring wstr = String(CommandLine).toStdWString();
	string result;
	INT iExitCode = ExecSubProcess(wstr.c_str(), ShowWindow, INFINITE, result);
	LOG_INFO("[%s],[%d]����[%d]", CommandLine, ShowWindow, iExitCode);
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
	LOG_INFO("[%s],[%d]����[%d]", CommandLine, ShowWindow, iExitCode);
	lua_pushboolean(l, iExitCode);
	lua_pushstring(l, result.c_str());
	return 2;
}

/**
 * .36 ShellExecute
 * ��װAPI ShellExecute
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
 * \brief �滻�����ַ����еĺ�Ϊ��ָ����Ŀ¼��
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
	LOG_INFO("ȡ��ID=%d��Ŀ¼%s", FolderId, szDir);
	lua_pushstring(l, szDir);
	return 1;
}

/**
 * \brief ����һ��EXE�Ŀ�ݷ�ʽ��ָ����λ�á�
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
	LOG_INFO("������ݷ�ʽ[%s]==>[%s]", lpszTargetExeFullPath, lpszLnkFileFullPath);
	lua_pushboolean(l,bRet);
	return 1;
}

int LFKillProcessByName(lua_State *l)
{
	const char* lpszProcessNameToKill = lua_tostring(l, 1);
	LOG_INFO("ɱ������[%s]", lpszProcessNameToKill);
	BOOL bKilled = KillProcessByName(String(lpszProcessNameToKill).toStdWString().c_str());
	lua_pushboolean(l,bKilled);
	return 1;
}

/**
 * \brief ֪ͨϵͳ�Ѿ��޸���ע����д��ļ��Ĺ������򡣣�PS:������ˢ��Explorer.exe�����ã�
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
	LOG_INFO("ֹͣ����[%s]%s", lpszServiceName, bRet?"�ɹ�":"ʧ��");
	lua_pushboolean(l, bRet);
	return 1;
}

int LFStartService(lua_State* l)
{
	const char* lpszServiceName = lua_tostring(l, 1);
	BOOL bRet = StartServiceByName(String(lpszServiceName).toStdWString().c_str());
	LOG_INFO("��ʼ����[%s]%s", lpszServiceName, bRet ? "�ɹ�" : "ʧ��");
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
 * \brief OsExtLib�ṹ�嶨��OsExtģ��LUA�ӿں��ڲ�ʵ�ֵ�ӳ���ϵ��
 * ÿ����Ա�ĵ�һ���ǵ����ӿ������ڶ������ڲ�ʵ�ֺ�������
 * �й�LUA�ӿڵ�ʹ�ü��ڲ�ʵ������˵����������ӿ��Բ鿴��
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
