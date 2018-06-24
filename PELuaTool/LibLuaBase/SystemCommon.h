/**
 * \file SystemCommon.h 
 * \brief ��װ��һЩ���õ����ļ��������ļ��汾�š������йص�Windows api
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
	* \brief �������ܣ���ָ���ļ���ָ����Ŀ¼�´������ݷ�ʽ
	*
	* \param lpszLnkFileFullPath    ��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����
	* \param lpszTargetExeFullPath  ��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����
	*/
	BOOL CreateFileShortcut(LPCTSTR lpszLnkFileFullPath,LPCTSTR lpszTargetExeFullPath);
	//////////////////////////////////////////////////////////////////////////
	// �ļ�������ص�
	namespace FilePath
	{
		/**
		 * \brief ǿ�ƴ���Ŀ¼
		 * \param pszFullPathFileName ��Ҫ������Ŀ¼
		 * \return �ɹ�����TRUE,���򷵻�FALSE��
		 */
		BOOL ForceCreateDir(LPCTSTR pszFullPathFileName);
		
		/**
		 * \brief ǿ�ƴ����ļ�
		 * \param pszFullPathFileName ��Ҫ�������ļ�ȫ·��
		 * \return �ɹ�����TRUE,���򷵻�FALSE��
		 */
		BOOL ForceCreateFile(LPCTSTR pszFullPathFileName);

		/**
		 * \brief ����Ŀ¼,���ںʹ����ɹ�����true
		 * \param pszDir ��Ҫ������Ŀ¼ȫ·��
		 * \return �ɹ�����TRUE,���򷵻�FALSE�����ںʹ����ɹ�����TRUE
		 */
		BOOL CreateMyDir(LPCTSTR pszDir);

		/**
		 * \brief �ж�һ��Ŀ¼���Ƿ���Դ����ļ�
		 * \param lpszDir Ŀ¼��ȫ·��
		 * \return �ɹ�����TRUE,���򷵻�FALSE�����ںʹ����ɹ�����TRUE
		 */
		BOOL IsDirCanCreateFile(LPCTSTR lpszDir);

		/**
		 * \brief ��õ�ǰʵ����·��
		 * \return ��ǰʵ����·��
		 */
		TS_String GetAppPath();

		// ���ļ�ѡ��Ի���
		//BOOL OpenSelectFile(PTSTR pstrFileName, PTSTR pstrTitleName, HWND hWnd, LPCTSTR lpszFilter, LPCTSTR lpszDefExt);

		// �򿪱����ļ��Ի���
		//BOOL OpenSaveFile(HINSTANCE hInst, HWND hWnd, LPTSTR pTitle, LPTSTR pFilter, LPTSTR pFileName, LPTSTR lpstrDefExt, DWORD dFlags,DWORD * pdwDefIndex, LPCTSTR lpszInitDir = NULL);

       // UINT CALLBACK MyOFNHookProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

		/**
		 * \brief ��·���з����ļ���
		 * \param pathname ·��
		 * \return �ļ���
		 */
		TS_String GetFileName(const TS_String &pathname);
		
		/**
		 * \brief ��·���з���·������ȥ���ļ�����
		 * \param pathname ·��
		 * \return ȥ���ļ������·��
		 */
		TS_String GetPath(const TS_String &pathname);

		/**
		 * \brief ��·���з������һ���ļ�������ȥ���ļ�����
		 * \param pathname ·��
		 * \return ���һ���ļ�����
		 */
		TS_String GetLastFolderName(const TS_String &pathname);

		/**
		 * \brief ȡһ��·������һ��Ŀ¼
		 * \param pathname ·��
		 * \return ��һ��Ŀ¼
		 */
		TS_String GetParentPath(const TS_String &pathname);
		
		/**
		 * \brief ��Url·���з����ļ���
		 * \param pathname URL
		 * \return �ļ���
		 */
		TS_String GetUrlFileName(const TS_String &pathname);
		
		/**
		 * \brief ��Url·���з���·������ȥ���ļ�������
		 * \param pathname URL
		 * \return URL·��
		 */
		TS_String GetUrlPath(const TS_String &pathname);
		
		//�ж�һ���ļ��Ƿ�����ö�ռ��ʽ��
		BOOL IsAllowDenyOpen(const TS_String &strPath);
		
		//ɾ���ļ�
		BOOL DeepDeleteFile( const TS_String& strFile );
		
		//ɾ�����ļ����µ������ļ�
		BOOL DeleteAllFiles(const TS_String &strPath);   
		
		BOOL RemoveDirectoryRecursively(LPCTSTR lpszDir);
		
		//���ļ���ĩβ�ӷ�б�߱�ʾ�ļ��еĽ���
		BOOL DirAddBackSlash(TS_String& strPath);

		//�Ƴ��ļ���ĩβ�ķ�б��
		BOOL DirRemoveBackSlash(TS_String& strPath);

		//�������ļ��м��±ߵ������ļ�
		BOOL CopyFolder(const TS_String& strSrc, const TS_String& strDst, BOOL bForce = FALSE );

		//����ļ�
		BOOL DeepCopyFile( const TS_String& strSrc, const TS_String& strDst, TS_String& strTemp );

		//release�µĴ�������־���
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
	// �ļ��汾����ص�
	namespace FileVersion
	{
		// ����ļ��İ汾��
		BOOL GetFileVersion( LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen );
		BOOL GetFileVersion( LPCTSTR strFile, VS_FIXEDFILEINFO** pInfo );
		TS_String GetFileVersion( LPCTSTR strFile );
        TS_String GetFileVersion(HMODULE hModule);
		
		// ����ļ�����Ϣ
		BOOL GetFileInfo( LPCTSTR strFile, LPCTSTR lpszInfoName, LPTSTR pszCompanyName, UINT uCompanyName );

		/*      returns -1 if src <  dst
		*       returns  0 if src == dst
		*       returns +1 if src >  dst
		**/
		int CompareVersion( LPCTSTR pszSrc, LPCTSTR pszDst );

		// ���IE�İ汾��
		long GetIEVersion();
	}

	
	//////////////////////////////////////////////////////////////////////////
	// ���ܣ�������ص�
	namespace Descode
	{
		// ���ܣ�����
		LPTSTR DecryptPassword(LPCTSTR lpszSrc, LPTSTR lpszDes, int nMax);
		LPTSTR EncryptPassword(LPCTSTR lpszSrc, LPTSTR lpszDes, int nMax);
	}

    //////////////////////////////////////////////////////////////////////////
    // �����������
    namespace Process
    {
        BOOL AysnCreateProcess(LPCTSTR lpszCmdLine, WORD dwShowWindow = SW_SHOW, DWORD dwTimeout = 0);
        BOOL AysnShellExecute(LPCTSTR lpszOperation, LPCTSTR lpszFile, LPCTSTR lpszParam = NULL, LPCTSTR lpszDirectory = NULL, int nShowCmd = SW_SHOW);
        DWORD WINAPI CreateProcessProc(LPVOID lpParam);
        DWORD WINAPI ShellExecuteProc(LPVOID lpParam);
		void SafeTerminateThread(HANDLE hThread, DWORD dwExitCode = -1);
		//������Ӧ�ó���
		void StartApp( LPCTSTR szAppCaption, LPCTSTR szParam = NULL, bool bIsFullPath = false );

		//��ȡGUID
		TS_String GetGuid();

        typedef struct 
        {
            // ������������ʱ�򣬲����ϳ�
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
    // window���
    namespace Window
    {
        HWND FindWindowByProcess(LPCTSTR lpszClassName, LPCTSTR lpszWindowName = NULL, DWORD nProcessID = 0/*Current Process*/);
    }

	namespace ValidSystem
	{
		// ͳһ��·��
		std::wstring GetNormalPath(LPCTSTR lpszPath);

		// �жϸ��ļ��Ƿ���ͳһ����·��
		bool IsNormalPath(LPCTSTR lpszPath);

		// �ж��ļ����Ƿ���Ч
		bool IsFileNameValid(LPCTSTR lpszFileName);

		// �жϸ�·���Ƿ��Ǹ�·������·��
		bool ValidPathIsSub(LPCTSTR lpszRoot, LPCTSTR lpszSub);

		// �жϸ�·���Ƿ���dll�ļ� lpszExt��.dll|.ext|.bat|,�����.
		bool IsFileSpecifiedExt(LPCTSTR lpszFile, LPCTSTR lpszExt);

		// �жϸ��ļ��Ƿ���pe�ļ�
		bool IsPEFile(LPCTSTR lpszFilePath);

		// �ж�url�Ƿ�����һ��������
		bool IsUrlIsSubUrl(LPCTSTR lpszUrl, LPCTSTR lpszSub);

		bool IsValidDownloadUrl(LPCTSTR lpszDownUrl);
	}

	//�������
    namespace DebugHelper
    {
        void RegisterInvalidParamHandler();
    }
};


//////////////////////////////////////////////////////////////////////////
// ������ص�
namespace NetHelper
{
	// �������
	BOOL GetDomain( LPCTSTR url, LPTSTR buf, int cbBuf);
};

//////////////////////////////////////////////////////////////////////////
// �ַ�����ص�
namespace StringHelper
{
	// ���ַ�����ĳ�ַָ����ֽ���ַ���
	int SplitString(LPCTSTR lpszSource, LPCTSTR lpszStep, std::vector<TS_String> &arrStr);

	// ���ַ�����ĳ�ַָ����ֽ���ַ���,����ֵΪ�ָ���������һ
	int SplitStringEx(LPCTSTR lpszStr, std::vector<TS_String> &strlist, TCHAR chSeparator = '\n', size_t iMaxCount = 0x7fffffff);
	int SplitStringExA(LPCSTR lpszStr, std::vector<std::string> &strlist, CHAR chSeparator = '\n', size_t iMaxCount = 0x7fffffff);

	// ��֤�ַ�����ĳ��β
	void EnsureStringEnd(LPTSTR lpszDes, LPCTSTR lpszEnd, DWORD dwSize);

	TS_String& Trim(TS_String &s);

	TS_String& ReplaceAll(TS_String& str,const TS_String& old_value,const TS_String& new_value);

	TS_String& MakeLower(TS_String &s);

	BOOL StrICompare(TS_String sA, TS_String sB);
	int StrIFind(TS_String sA, TS_String sB);

	//�ַ�����ʽ������
	TS_String Format(const TCHAR *fmt, ...);

	std::string Wstring2String( const wchar_t* pwc );
	std::wstring String2WString(const char* pc);

	std::string TransWStringToUTF8( const wchar_t* apFrom);
	std::wstring TransUTF8ToWString( const char * apFrom);
}