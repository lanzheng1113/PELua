/**
 * \file SystemCommon.h 
 * \brief 封装了一些常用的与文件操作、文件版本号、进程有关的Windows api
 * 
 */


#pragma once

#include <vector>
#include <string>

#ifdef UNICODE
#define TS_String std::wstring
#else
#define TS_String std::string
#endif

#define IE_VERSION_OLD				(long)-1
#define IE_VERSION_5				(long)0
#define IE_VERSION_6				(long)1
#define IE_VERSION_7				(long)2
#define IE_VERSION_8				(long)3
#define IE_VERSION_9				(long)4
#define IE_VERSION_10				(long)5
#define IE_VERSION_LAST				(long)1000

BOOL KillProcessByName(LPCTSTR lpProcessName);
DWORD FindProc(LPCTSTR lpProcessName);
DWORD ParentProcessID(LPCTSTR lpProcessName);
BOOL StartServiceByName(LPCTSTR ServiceName);
BOOL StopServiceByName(LPCTSTR ServiceName);

namespace os
{
	/**
	* \brief 函数功能：对指定文件在指定的目录下创建其快捷方式
	*
	* \param lpszLnkFileFullPath    快捷方式名称，为NULL表示EXE文件名。
	* \param lpszTargetExeFullPath  快捷方式名称，为NULL表示EXE文件名。
	*/
	BOOL CreateFileShortcut(LPCTSTR lpszLnkFileFullPath,LPCTSTR lpszTargetExeFullPath);
	//////////////////////////////////////////////////////////////////////////
	// 文件创建相关的
	namespace FilePath
	{
		/**
		 * \brief 强制创建目录
		 * \param pszFullPathFileName 需要创建的目录
		 * \return 成功返回TRUE,否则返回FALSE。
		 */
		BOOL ForceCreateDir(LPCTSTR pszFullPathFileName);
		
		/**
		 * \brief 强制创建文件
		 * \param pszFullPathFileName 需要创建的文件全路径
		 * \return 成功返回TRUE,否则返回FALSE。
		 */
		BOOL ForceCreateFile(LPCTSTR pszFullPathFileName);

		/**
		 * \brief 创建目录,存在和创建成功返回true
		 * \param pszDir 需要创建的目录全路径
		 * \return 成功返回TRUE,否则返回FALSE。存在和创建成功返回TRUE
		 */
		BOOL CreateMyDir(LPCTSTR pszDir);

		/**
		 * \brief 判断一个目录下是否可以创建文件
		 * \param lpszDir 目录的全路径
		 * \return 成功返回TRUE,否则返回FALSE。存在和创建成功返回TRUE
		 */
		BOOL IsDirCanCreateFile(LPCTSTR lpszDir);

		/**
		 * \brief 获得当前实例的路径
		 * \return 当前实例的路径
		 */
		TS_String GetAppPath();

		// 打开文件选择对话框
		//BOOL OpenSelectFile(PTSTR pstrFileName, PTSTR pstrTitleName, HWND hWnd, LPCTSTR lpszFilter, LPCTSTR lpszDefExt);

		// 打开保存文件对话框
		//BOOL OpenSaveFile(HINSTANCE hInst, HWND hWnd, LPTSTR pTitle, LPTSTR pFilter, LPTSTR pFileName, LPTSTR lpstrDefExt, DWORD dFlags,DWORD * pdwDefIndex, LPCTSTR lpszInitDir = NULL);

       // UINT CALLBACK MyOFNHookProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

		/**
		 * \brief 从路径中分离文件名
		 * \param pathname 路径
		 * \return 文件名
		 */
		TS_String GetFileName(const TS_String &pathname);
		
		/**
		 * \brief 从路径中分离路径名（去除文件名）
		 * \param pathname 路径
		 * \return 去除文件名后的路径
		 */
		TS_String GetPath(const TS_String &pathname);

		/**
		 * \brief 从路径中分离最后一个文件夹名（去除文件名）
		 * \param pathname 路径
		 * \return 最后一个文件夹名
		 */
		TS_String GetLastFolderName(const TS_String &pathname);

		/**
		 * \brief 取一个路径的上一级目录
		 * \param pathname 路径
		 * \return 上一级目录
		 */
		TS_String GetParentPath(const TS_String &pathname);
		
		/**
		 * \brief 从Url路径中分离文件名
		 * \param pathname URL
		 * \return 文件名
		 */
		TS_String GetUrlFileName(const TS_String &pathname);
		
		/**
		 * \brief 从Url路径中分离路径名（去除文件名）：
		 * \param pathname URL
		 * \return URL路径
		 */
		TS_String GetUrlPath(const TS_String &pathname);
		
		//判断一个文件是否可以用独占方式打开
		BOOL IsAllowDenyOpen(const TS_String &strPath);
		
		//删除文件
		BOOL DeepDeleteFile( const TS_String& strFile );
		
		//删除该文件夹下的所有文件
		BOOL DeleteAllFiles(const TS_String &strPath);   
		
		BOOL RemoveDirectoryRecursively(LPCTSTR lpszDir);
		
		//在文件夹末尾加反斜线表示文件夹的结束
		BOOL DirAddBackSlash(TS_String& strPath);

		//移除文件夹末尾的反斜杠
		BOOL DirRemoveBackSlash(TS_String& strPath);

		//拷贝该文件夹及下边的所有文件
		BOOL CopyFolder(const TS_String& strSrc, const TS_String& strDst, BOOL bForce = FALSE );

		//深拷贝文件
		BOOL DeepCopyFile( const TS_String& strSrc, const TS_String& strDst, TS_String& strTemp );

		//release下的带参数日志输出
		//void ReleaseTrace( const TCHAR* pszInfo, ... );

		////////////////////////////////////////////////////////////////////
		// SetText
		// - Places text on the clipboard
		////////////////////////////////////////////////////////////////////
		//
		// Parameters:
		//	lpszBuffer - pointer to a string where the text is to be put
		//	hWnd       - window handle to be used by clipboard
		//
		// Return Values:
		//	TRUE       - Text was successfully copied from clipboard
		//	FALSE      - No text on the clipboard
		//
		////////////////////////////////////////////////////////////////////
		
		int SetClipboardText (LPCTSTR lpszBuffer, HWND hWnd);

	}

	//////////////////////////////////////////////////////////////////////////
	// 文件版本号相关的
	namespace FileVersion
	{
		// 获得文件的版本号
		BOOL GetFileVersion( LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen );
		BOOL GetFileVersion( LPCTSTR strFile, VS_FIXEDFILEINFO** pInfo );
		TS_String GetFileVersion( LPCTSTR strFile );
        TS_String GetFileVersion(HMODULE hModule);
		
		// 获得文件的信息
		BOOL GetFileInfo( LPCTSTR strFile, LPCTSTR lpszInfoName, LPTSTR pszCompanyName, UINT uCompanyName );

		/*      returns -1 if src <  dst
		*       returns  0 if src == dst
		*       returns +1 if src >  dst
		**/
		int CompareVersion( LPCTSTR pszSrc, LPCTSTR pszDst );

		// 获得IE的版本号
		long GetIEVersion();
	}

	
	//////////////////////////////////////////////////////////////////////////
	// 加密，解密相关的
	namespace Descode
	{
		// 加密，解密
		LPTSTR DecryptPassword(LPCTSTR lpszSrc, LPTSTR lpszDes, int nMax);
		LPTSTR EncryptPassword(LPCTSTR lpszSrc, LPTSTR lpszDes, int nMax);
	}

    //////////////////////////////////////////////////////////////////////////
    // 启动进程相关
    namespace Process
    {
        BOOL AysnCreateProcess(LPCTSTR lpszCmdLine, WORD dwShowWindow = SW_SHOW, DWORD dwTimeout = 0);
        BOOL AysnShellExecute(LPCTSTR lpszOperation, LPCTSTR lpszFile, LPCTSTR lpszParam = NULL, LPCTSTR lpszDirectory = NULL, int nShowCmd = SW_SHOW);
        DWORD WINAPI CreateProcessProc(LPVOID lpParam);
        DWORD WINAPI ShellExecuteProc(LPVOID lpParam);
		void SafeTerminateThread(HANDLE hThread, DWORD dwExitCode = -1);
		//启动启应用程序
		void StartApp( LPCTSTR szAppCaption, LPCTSTR szParam = NULL, bool bIsFullPath = false );

		//获取GUID
		TS_String GetGuid();

        typedef struct 
        {
            // 分享插件启动的时候，参数较长
            TCHAR szCmdLine[MAX_PATH * 3 + 1];
            WORD dwShowWindow;
            DWORD dwTimeout;
        } CREATEPROCESS_PARAM;

        typedef struct  
        {
            TCHAR szOperation[MAX_PATH];
            TCHAR szFilePath[MAX_PATH];
            TCHAR szParam[MAX_PATH];
            TCHAR szDirectory[MAX_PATH];
            int nShowCmd;
        } SHELLEXECUTE_PARAM;
        
    }
	
	    //////////////////////////////////////////////////////////////////////////
    // window相关
    namespace Window
    {
        HWND FindWindowByProcess(LPCTSTR lpszClassName, LPCTSTR lpszWindowName = NULL, DWORD nProcessID = 0/*Current Process*/);
    }

	namespace ValidSystem
	{
		// 统一化路径
		std::wstring GetNormalPath(LPCTSTR lpszPath);

		// 判断该文件是否是统一化的路径
		bool IsNormalPath(LPCTSTR lpszPath);

		// 判断文件名是否有效
		bool IsFileNameValid(LPCTSTR lpszFileName);

		// 判断该路径是否是根路径的子路径
		bool ValidPathIsSub(LPCTSTR lpszRoot, LPCTSTR lpszSub);

		// 判断改路径是否是dll文件 lpszExt：.dll|.ext|.bat|,必须加.
		bool IsFileSpecifiedExt(LPCTSTR lpszFile, LPCTSTR lpszExt);

		// 判断该文件是否是pe文件
		bool IsPEFile(LPCTSTR lpszFilePath);

		// 判断url是否属于一个子域名
		bool IsUrlIsSubUrl(LPCTSTR lpszUrl, LPCTSTR lpszSub);

		bool IsValidDownloadUrl(LPCTSTR lpszDownUrl);
	}

	//调试相关
    namespace DebugHelper
    {
        void RegisterInvalidParamHandler();
    }
};


//////////////////////////////////////////////////////////////////////////
// 网络相关的
namespace NetHelper
{
	// 获得域名
	BOOL GetDomain( LPCTSTR url, LPTSTR buf, int cbBuf);
};

//////////////////////////////////////////////////////////////////////////
// 字符串相关的
namespace StringHelper
{
	// 把字符串按某种分隔符分解成字符串
	int SplitString(LPCTSTR lpszSource, LPCTSTR lpszStep, std::vector<TS_String> &arrStr);

	// 把字符串按某种分隔符分解成字符串,返回值为分隔符个数加一
	int SplitStringEx(LPCTSTR lpszStr, std::vector<TS_String> &strlist, TCHAR chSeparator = '\n', size_t iMaxCount = 0x7fffffff);
	int SplitStringExA(LPCSTR lpszStr, std::vector<std::string> &strlist, CHAR chSeparator = '\n', size_t iMaxCount = 0x7fffffff);

	// 保证字符串以某结尾
	void EnsureStringEnd(LPTSTR lpszDes, LPCTSTR lpszEnd, DWORD dwSize);

	TS_String& Trim(TS_String &s);

	TS_String& ReplaceAll(TS_String& str,const TS_String& old_value,const TS_String& new_value);

	TS_String& MakeLower(TS_String &s);

	BOOL StrICompare(TS_String sA, TS_String sB);
	int StrIFind(TS_String sA, TS_String sB);

	//字符串格式化函数
	TS_String Format(const TCHAR *fmt, ...);

	std::string Wstring2String( const wchar_t* pwc );
	std::wstring String2WString(const char* pc);

	std::string TransWStringToUTF8( const wchar_t* apFrom);
	std::wstring TransUTF8ToWString( const char * apFrom);
}