#include "StdAfx.h"
#include <intrin.h>
#include <assert.h>
#include "SystemCommon.h"
#include <atlbase.h>
#include <atlconv.h>
#include <WinInet.h>
#include <Shlwapi.h>
#include <CommDlg.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>
#include <new.h>
#include <TlHelp32.h>

#pragma warning (disable:4786)
#pragma comment(lib, "Version")
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "Wininet")

//////////////////////////////////////////////////////////////////////////
// 创建文件相关的
BOOL os::FilePath::CreateMyDir( LPCTSTR pszDir )
{
	assert(pszDir);

	if (!pszDir || !pszDir[0])
		return FALSE;

    BOOL bRet;
	bRet = CreateDirectory( pszDir, NULL );
	if( FALSE == bRet )
	{
		if( ERROR_ALREADY_EXISTS == GetLastError() )
			return TRUE;
	}
	return bRet;
}

BOOL os::FilePath::ForceCreateDir( LPCTSTR pszFullPathFileName )
{
	if (!pszFullPathFileName || !pszFullPathFileName[0])
		return FALSE;

	if ( PathIsRoot( pszFullPathFileName ) )
		return TRUE;
	
	TCHAR Dir[MAX_PATH+1]={};
	int	 nNeed;
	LPCTSTR p, pLast;
	BOOL  Result;
	
	Result=FALSE;
	assert(pszFullPathFileName);
	pLast=pszFullPathFileName;
	if(_tcslen(pLast)>_MAX_PATH) return FALSE;
	while(NULL!=*pLast)
	{
		p=_tcsstr(pLast,_T("\\"));
		if(NULL==p)
		{
			p=_tcsstr(pLast,_T("/"));
			
			if(NULL==p)
				return Result;
		}
		nNeed=p-pszFullPathFileName;
		if(nNeed>0)
		{
			memset(Dir,0,sizeof(Dir));
			_tcsncpy_s(Dir, pszFullPathFileName, p - pszFullPathFileName);
			Result=CreateMyDir(Dir);
		}
		p++;
		pLast=p;
	}
	return Result;
}

// 强制创建文件
BOOL os::FilePath::ForceCreateFile( LPCTSTR pszFullPathFileName )
{
	if (!pszFullPathFileName || !pszFullPathFileName[0])
		return FALSE
		;
	ForceCreateDir( GetPath(pszFullPathFileName).c_str() );
	HANDLE hFile = CreateFile( pszFullPathFileName,	GENERIC_WRITE, 0, NULL,	CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle( hFile );
	return TRUE;
}
BOOL os::FilePath::IsDirCanCreateFile(LPCTSTR lpszDir)
{
	if (!lpszDir || !lpszDir[0])
		return FALSE;

	BOOL bRead = FALSE;

	TCHAR szTempFile[MAX_PATH + 1] = {};
	StringCbPrintf(szTempFile,sizeof(szTempFile),_T("%s%d.txt"), lpszDir, GetTickCount());
	//_sntprintf_s(szTempFile, MAX_PATH, _T("%s%d.txt"), lpszDir, GetTickCount());
	HANDLE hFile = CreateFile(szTempFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		bRead = TRUE;
		CloseHandle(hFile);
		DeleteFile(szTempFile);
	}

	return bRead;
}

TS_String os::FilePath::GetAppPath()
{
	TCHAR wszPath[MAX_PATH]={};
	::GetModuleFileName( GetModuleHandle(NULL), wszPath, MAX_PATH );
	PathRemoveFileSpec( wszPath );
	PathAddBackslash( wszPath );
	return wszPath;
}

//BOOL os::FilePath::OpenSelectFile(PTSTR pstrFileName, PTSTR pstrTitleName, HWND hWnd, LPCTSTR lpszFilter, LPCTSTR lpszDefExt)
//{
//	OPENFILENAME ofn = {};
//	ofn.lStructSize = sizeof(OPENFILENAME) ;
//	ofn.hInstance = NULL;
//	ofn.lpstrFilter = lpszFilter;
//	ofn.lpstrCustomFilter = NULL;
//	ofn.nMaxCustFilter = 0;
//	ofn.nFilterIndex = 0;
//	ofn.lpstrFile = NULL;
//	ofn.nMaxFile = MAX_PATH;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = MAX_PATH;
//	ofn.lpstrInitialDir = NULL;
//	ofn.lpstrTitle = NULL;
//	ofn.Flags = 0;
//	ofn.nFileOffset = 0;
//	ofn.nFileExtension = 0;
//	ofn.lpstrDefExt = lpszDefExt;
//	ofn.lCustData = 0L;
//	ofn.lpfnHook = NULL;
//	ofn.lpTemplateName = NULL;
//
//	ofn.hwndOwner = hWnd;
//	ofn.lpstrFile = pstrFileName;
//	ofn.lpstrFileTitle = pstrTitleName;
//	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT|OFN_NODEREFERENCELINKS;
//	return GetOpenFileName(&ofn);
//}
//
//UINT CALLBACK os::FilePath::MyOFNHookProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch (uMsg)
//	{
//	case WM_WINDOWPOSCHANGED:
//		{
//			HWND hwndParentDlg;
//			RECT rect;
//			
//			hwndParentDlg = GetParent(hwndDlg);
//			GetWindowRect(hwndParentDlg, &rect);
//			SetWindowPos(hwndParentDlg, 0,
//				(GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left) / 2,
//				(GetSystemMetrics(SM_CYSCREEN) - rect.bottom + rect.top) / 2,
//				0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
//			break;
//		}
//	}
//	
//	return 0;
//	UNREFERENCED_PARAMETER(wParam);
//	UNREFERENCED_PARAMETER(lParam);
//}

//从路径中分离文件名
TS_String os::FilePath::GetFileName(const TS_String &pathname) 
{ 
	// vc9 build error
	int i = 0;
	for(i = pathname.size()-1; i>=0; i-- ) 
	{ 
		if( pathname[i]=='\\' || pathname[i]=='/') 
			break; 
	} 
	return pathname.substr( i+1 ); 
}

//从路径中分离路径名（去除文件名）：
TS_String os::FilePath::GetPath(const TS_String &pathname) 
{
	int i = 0;
	for(i = pathname.size()-1; i>=0; i-- ) 
	{ 
		if( pathname[i]=='\\' || pathname[i]=='/') 
			break; 
	} 
	return pathname.substr( 0, i+1 ); 
}

//从路径中分离最后一个文件夹名（去除文件名）：
TS_String os::FilePath::GetLastFolderName(const TS_String &pathname) 
{
	int i = 0;
	int iLastPos = pathname.size();
	bool bHaveFile = FALSE;
	bool bHaveSlash = TRUE;
	for(i = pathname.size()-1; i>=0; i-- ) 
	{ 
		if( bHaveSlash )
		{
			if (pathname[i]=='\\' || pathname[i]=='/')
			{
				iLastPos = i;//滤掉尾部的斜杠
			}
			else
			{
				bHaveSlash = FALSE;
			}
		}
		
		if (pathname[i]=='.')
		{
			bHaveFile = TRUE;
		}

		if( pathname[i]=='\\' || pathname[i]=='/' ) 
		{
			if ( bHaveFile )
			{
				iLastPos = i;
				bHaveFile = FALSE;//不停止，接着往下找
			}
			else
			{
				if(!bHaveSlash)
				{
					break;//跳出循环
				}
			}
		}
	} 
	
	int nLen = iLastPos - i - 1;
	return pathname.substr( i+1, nLen ); 
}

//取一个路径的上一级目录：
TS_String os::FilePath::GetParentPath(const TS_String &pathname) 
{ 
	TS_String strReturn(L"");

	if (!pathname.empty())
	{
		TCHAR wszPath[1024]={};
		_tcsncpy_s(wszPath, pathname.c_str(), _TRUNCATE);
		if ( ('\\' == pathname[pathname.size()-1]) || ('/' == pathname[pathname.size()-1]) )
		{
			PathRemoveFileSpec( wszPath );
		}
		PathRemoveFileSpec( wszPath );
		strReturn = wszPath;
		strReturn += L"\\";
	}
	return strReturn;
}


//从Url路径中分离文件名
TS_String os::FilePath::GetUrlFileName(const TS_String &pathname) 
{ 
	// vc9 build error
	int i = 0;
	for(i = pathname.size()-1; i>=0; i-- ) 
	{ 
		if( pathname[i]=='/' ) 
			break; 
	} 
	return pathname.substr( i+1 ); 
}

//从Url路径中分离路径名（去除文件名）：
TS_String os::FilePath::GetUrlPath(const TS_String &pathname) 
{
	// vc9 build error
	int i = 0;
	for(i = pathname.size()-1; i>=0; i-- ) 
	{ 
		if( pathname[i]=='/' ) 
			break; 
	} 
	return pathname.substr( 0, i ); 
}

//判断一个文件是否可以用独占方式打开
BOOL os::FilePath::IsAllowDenyOpen(const TS_String &strPath)   
{  
	int nR = _taccess( strPath.c_str(), 0 );//Existence only

	if ( nR != 0 )//不存在
	{
		return TRUE;
	}
	
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile( strPath.c_str(), GENERIC_ALL,
		0,	NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile!= INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return TRUE;
	}
	return FALSE;
}

//删除文件
BOOL os::FilePath::DeepDeleteFile( const TS_String& strFile )
{
	int nR = _taccess( strFile.c_str(), 0 );//Existence only
	
	if ( nR != 0 || PathIsDirectory(strFile.c_str()))//不存在
	{
		return TRUE;
	}
    SetFileAttributes(strFile.c_str(), FILE_ATTRIBUTE_NORMAL);//去掉只读属性
	
	BOOL bRet = ::DeleteFile( strFile.c_str() );
	if (!bRet)
	{
		TS_String strDstTmp = strFile + L".tmp";
		MoveFileEx( strFile.c_str(), strDstTmp.c_str(), MOVEFILE_REPLACE_EXISTING );
		if (!DeleteFile(strDstTmp.c_str()))
		{
			MoveFileEx( strDstTmp.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT );
		}
	}
	return bRet;
}
//删除该文件夹及下边的所有文件
BOOL os::FilePath::DeleteAllFiles(const TS_String &strPath)   
{ 
	if (strPath.empty())
	{
		return TRUE;
	}
	DeleteFile(strPath.c_str());

	SHFILEOPSTRUCT   fo;
	TS_String strFrom = strPath;

	if (strPath[strPath.size()-1] != _T('\\'))
	{
		strFrom = strFrom + _T("\\");
	}
	strFrom = strFrom + _T("*.*") + _T('\0');
	memset(&fo, 0, sizeof(fo));   
	
	//fo.hwnd = ::GetActiveWindow();
	fo.hwnd = NULL;
	fo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;   
	fo.wFunc = FO_DELETE;   
	fo.pFrom = strFrom.c_str();   
	
	SHFileOperation(&fo);
	RemoveDirectory(strPath.c_str());
    
	return   TRUE;   
}   
BOOL os::FilePath::RemoveDirectoryRecursively(LPCTSTR lpszDir)
{
	BOOL bRet = TRUE;
	HANDLE hFindFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA struFindFileData={0};

	TCHAR strToFind[ MAX_PATH ] = _T( "" );
	StringCbPrintf(strToFind, sizeof(strToFind),_T( "%s\\%s" ), lpszDir, _T( "*.*" ));
	//_sntprintf_s( strToFind, sizeof(strToFind), MAX_PATH - 1, _T( "%s\\%s" ), lpszDir, _T( "*.*" ));
	hFindFile=FindFirstFile( strToFind, &struFindFileData );
	if ( hFindFile == INVALID_HANDLE_VALUE )
		return FALSE;
	do
	{
		if ( !_tcscmp( struFindFileData.cFileName, _T( "." ) ) )
			continue;
		if ( !_tcscmp( struFindFileData.cFileName, _T( ".." ) ) )
			continue;
		if ( struFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			TCHAR strNewSource[ MAX_PATH ] = _T( "" );
			StringCbPrintf(strNewSource, sizeof(strNewSource),_T( "%s\\%s" ), lpszDir, struFindFileData.cFileName );
			//_sntprintf_s( strNewSource, sizeof(strNewSource), MAX_PATH - 1, _T( "%s\\%s" ), lpszDir, struFindFileData.cFileName );
			bRet = RemoveDirectoryRecursively( strNewSource);
		}
		else
		{
			TCHAR strFile[ MAX_PATH ] = _T( "" );
			StringCbPrintf(strFile, sizeof(strFile), _T( "%s\\%s" ), lpszDir, struFindFileData.cFileName );
			//_sntprintf_s( strFile, sizeof(strFile), MAX_PATH - 1, _T( "%s\\%s" ), lpszDir, struFindFileData.cFileName );
			bRet = DeepDeleteFile( strFile );
		}
	}
	while ( FindNextFile( hFindFile, &struFindFileData ) );
	FindClose(hFindFile);
	hFindFile = INVALID_HANDLE_VALUE;
	SetFileAttributes(lpszDir, FILE_ATTRIBUTE_ARCHIVE);
	bRet = RemoveDirectory( lpszDir );
	return bRet;
}

BOOL os::FilePath::DirRemoveBackSlash( TS_String& strPath )
{
	TCHAR szPath[MAX_PATH] = {0};
	_tcsncpy_s(szPath, strPath.c_str(), _TRUNCATE);
	int len = _tcslen(szPath);
	if ( len >= 1 && (_T('\\') == szPath[len - 1] || (_T('/') == szPath[len - 1])))
	{
		PathRemoveBackslash(szPath);
	}
	strPath = szPath;
	return TRUE;
}

BOOL os::FilePath::DirAddBackSlash(TS_String& strPath)
{	

	TCHAR szPath[MAX_PATH] = {0};
	//int len = strPath.length(); 当len大于MAX_PATH时将导致数组越界访问
	_tcsncpy_s(szPath, strPath.c_str(), _TRUNCATE);
	int len = _tcslen(szPath);

	if ( len >= 1 && (_T('\\') != szPath[len - 1] && (_T('/') != szPath[len - 1])))
	{
		PathAddBackslash(szPath);
	}
	strPath = szPath;
	return TRUE;
}

//拷贝该文件夹及下边的所有文件
BOOL os::FilePath::CopyFolder( const TS_String& strSrc, const TS_String& strDst, BOOL bForce)
{        
    TS_String tmpFrom(strSrc);
    TS_String tmpTo(strDst);  
	BOOL bRet = FALSE;

    if (!PathFileExists(tmpTo.c_str()))
    {
        ForceCreateDir(tmpTo.c_str());
    }
    if (PathFileExists(tmpFrom.c_str()))
    {            
        WIN32_FIND_DATA FindFileData;
        TS_String tmpFind(tmpFrom);
        tmpFind += _T("\\*.*");
		
        HANDLE hFind=::FindFirstFile(tmpFind.c_str(),&FindFileData);
        if(INVALID_HANDLE_VALUE == hFind)
            return bRet;
		
        while(TRUE)
        {
            TS_String tmpFile(tmpTo);
            tmpFile += _T("\\");
            tmpFile += FindFileData.cFileName;
			
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //目录
            {
                if(FindFileData.cFileName[0]!='.')
                {
                    if (!PathFileExists(tmpFile.c_str()))        //目录不存在
                    {
						DirAddBackSlash(tmpFile);
                        ForceCreateDir(tmpFile.c_str());    
                    }
					
                    bRet = CopyFolder( tmpFrom + _T("\\") + FindFileData.cFileName, tmpFile, bForce);                
                }                
            }
            else                    //文件
            {                
                if (bForce) // 强制覆盖
                {
					TS_String xstr;
                    bRet = DeepCopyFile((tmpFrom + _T("\\") + FindFileData.cFileName).c_str(), tmpFile.c_str(),xstr);
                }else if (!PathFileExists(tmpFile.c_str()))    //文件不存在
                {
                    bRet = ::CopyFile( (tmpFrom + _T("\\") + FindFileData.cFileName).c_str(),
						      tmpFile.c_str(),
							  TRUE);    //拷贝文件
                }
            }
			
			
            if(!FindNextFile(hFind,&FindFileData))    
                break;
        }
        FindClose(hFind);
	}
	return bRet;
}

//深拷贝文件
BOOL os::FilePath::DeepCopyFile( const TS_String& strSrc, const TS_String& strDst, TS_String& strTemp )
{
	BOOL bRet = FALSE;
	if (PathFileExists(strSrc.c_str()))
	{
		if(!StringHelper::StrICompare(strSrc, strDst))
		{
			if (PathFileExists(strDst.c_str()))
			{
				SetFileAttributes(strDst.c_str(), FILE_ATTRIBUTE_NORMAL);//去掉只读属性
			}
			else
			{
				TS_String strPath = GetPath( strDst );
				ForceCreateDir( strPath.c_str() );
			}

// 			if (!DeleteFile(strDst.c_str()))
// 			{
			if (PathFileExists(strDst.c_str()))
			{
				TS_String strDstTmp = strDst + L".tmp";
				if (MoveFileEx( strDst.c_str(), strDstTmp.c_str(), MOVEFILE_REPLACE_EXISTING ))
				{
					strTemp = strDstTmp;
				}else{
					return FALSE;
				}
			}
// 				if (!DeleteFile(strDstTmp.c_str()))
// 				{
// 					MoveFileEx( strDstTmp.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT );
// 				}
//			}
			bRet = ::CopyFile( strSrc.c_str(), strDst.c_str(), FALSE );
//			bRet = TRUE;
		}		
	}
	return bRet;
}

void os::Process::StartApp( LPCTSTR szAppCaption, LPCTSTR szParam/*=NULL*/, bool bIsFullPath /*= false */ )
{
	TS_String strAppPath(szAppCaption);
	
	if (!bIsFullPath)
	{
		strAppPath = os::FilePath::GetAppPath() + szAppCaption;
	}	
	//WinExec( wszPath, SW_SHOWNORMAL );//同步
	ShellExecute( NULL, NULL, strAppPath.c_str(), szParam, NULL, SW_SHOWNORMAL );//异步
}


int os::FilePath::SetClipboardText( LPCTSTR lpszBuffer, HWND hWnd )
{
	if (!lpszBuffer || !lpszBuffer[0])
		return TRUE;

	HGLOBAL hGlobal;		// Global memory handle
	LPTSTR lpszData;			// Pointer to clipboard data
	unsigned long nSize;	// Size of clipboard data
	
	if (_tcslen(lpszBuffer)<1)
	{
		return TRUE;
	}
	
	//try
	{
		// First, open the clipboard. OpenClipboard() takes one
		// parameter, the handle of the window that will temporarily
		// be it's owner. If NULL is passed, the current process
		// is assumed. After opening, empty the clipboard so we
		// can put our text on it.
		OpenClipboard(hWnd);
		EmptyClipboard();
		
		// Get the size of the string in the buffer that was
		// passed into the function, so we know how much global
		// memory to allocate for the string.
		nSize = lstrlen(lpszBuffer);
		
		// Allocate the memory for the string.
		hGlobal = GlobalAlloc(GMEM_ZEROINIT, (nSize+1)*sizeof(TCHAR));
		
		// If we got any error during the memory allocation,
		// we have been returned a NULL handle.
		if (hGlobal == NULL) return FALSE;
		
		// Now we have a global memory handle to the text
		// stored on the clipboard. We have to lock this global
		// handle so that we have access to it.
		lpszData = (LPTSTR)GlobalLock(hGlobal);
		
		// Now, copy the text from the buffer into the allocated
		// global memory pointer.
		for (UINT i = 0; i < nSize + 1; ++i)
			*(lpszData + i) = *(lpszBuffer + i);
	}
// 	catch (...)
// 	{
// 		
// 	}
	
	// Now, simply unlock the global memory pointer,
	// set the clipboard data type and pointer,
	// and close the clipboard.
	GlobalUnlock(hGlobal);
#ifndef _UNICODE
	SetClipboardData(CF_TEXT, hGlobal);
#else
	SetClipboardData(CF_UNICODETEXT, hGlobal);
#endif 
	CloseClipboard();
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// 文件版本号相关的
BOOL os::FileVersion::GetFileVersion( LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen )
{
	if (!strFile || !strFile[0] || !pszVersion || nVersionLen<=0 )
		return FALSE;

	TCHAR  szVersionBuffer[8192] = {};
	DWORD dwVerSize;
	DWORD dwHandle;
	
	dwVerSize = GetFileVersionInfoSize( strFile, &dwHandle );
	if( dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer)-1) )
		return FALSE;
	
	if( GetFileVersionInfo( strFile,0,dwVerSize,szVersionBuffer) )
	{
		VS_FIXEDFILEINFO * pInfo;
		unsigned int nInfoLen;
		
		if( VerQueryValue( szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen ) )
		{
			_sntprintf_s( pszVersion, nVersionLen, _TRUNCATE, _T("%d.%d.%d.%d"), 
				HIWORD( pInfo->dwFileVersionMS ), LOWORD( pInfo->dwFileVersionMS ), 
				HIWORD( pInfo->dwFileVersionLS ), LOWORD( pInfo->dwFileVersionLS ) );
			pszVersion[nVersionLen-1] = _T('\0');
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL os::FileVersion::GetFileVersion( LPCTSTR strFile, VS_FIXEDFILEINFO** pInfo )
{
	if (!strFile || !strFile[0])
		return FALSE;

	TCHAR  szVersionBuffer[8192] = {};
	DWORD dwVerSize;
	DWORD dwHandle;
	
	dwVerSize = GetFileVersionInfoSize( strFile, &dwHandle );
	if( dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer)-1) )
		return FALSE;
	
	if( GetFileVersionInfo( strFile,0,dwVerSize,szVersionBuffer) )
	{
		unsigned int nInfoLen;
		if( VerQueryValue( szVersionBuffer, _T("\\"), (void**)pInfo, &nInfoLen ) )
			return TRUE;
	}
	
	return FALSE;
}

TS_String os::FileVersion::GetFileVersion( LPCTSTR strFile )
{
	if (!strFile || !strFile[0])
		return _T("");
#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(A) (sizeof(A)/sizeof((A)[0]))
#endif
	TCHAR  szVersionBuffer[8192] = {};
	if( GetFileVersion(strFile, szVersionBuffer, SIZEOF_ARRAY(szVersionBuffer)) )
	{
		return szVersionBuffer;
	}
	return _T("");
}

TS_String os::FileVersion::GetFileVersion(HMODULE hModule)
{
    if (!hModule) return L"";
    
    TCHAR szFilePath[MAX_PATH] = {};
    GetModuleFileName(hModule, szFilePath, MAX_PATH);
    return GetFileVersion(szFilePath);
}

BOOL os::FileVersion::GetFileInfo( LPCTSTR strFile, LPCTSTR lpszInfoName, LPTSTR pszCompanyName, UINT uCompanyName )
{
	if (!strFile || !strFile[0] || !lpszInfoName || !lpszInfoName[0] || !pszCompanyName || uCompanyName<=0)
		return FALSE;

	TCHAR  szVersionBuffer[8192] = {};
	DWORD dwVerSize;
	DWORD dwHandle;
	
	dwVerSize = GetFileVersionInfoSize( strFile, &dwHandle );
	if( dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer)-1) )
		return FALSE;
	
	if( GetFileVersionInfo( strFile,0,dwVerSize,szVersionBuffer) )
	{
		struct LANGANDCODEPAGE {WORD wLanguage;WORD wCodePage;} *lpTranslate;
		UINT cbTranslate = sizeof(LANGANDCODEPAGE);
		VerQueryValue( szVersionBuffer, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);
		
		int nLanCount = cbTranslate/sizeof(struct LANGANDCODEPAGE);
		assert( nLanCount );
		if( nLanCount > 0 )
		{
			TCHAR szSubBlock[MAX_PATH] = {};
#ifndef SIZEOF_ARRAY
			#define SIZEOF_ARRAY(A) (sizeof(A)/sizeof((A)[0]))
#endif
			_sntprintf_s( szSubBlock, SIZEOF_ARRAY(szSubBlock)-1, _T("\\StringFileInfo\\%04x%04x\\%s"), 
				lpTranslate[0].wLanguage, lpTranslate[0].wCodePage, lpszInfoName);
			
			TCHAR* pszName = NULL;
			VerQueryValue( szVersionBuffer, szSubBlock, (void**)&pszName, &uCompanyName );
			if( pszName )
			{
				_sntprintf_s( pszCompanyName, uCompanyName-1, _TRUNCATE, _T("%s"), pszName );
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

int os::FileVersion::CompareVersion( LPCTSTR pszSrc, LPCTSTR pszDst )
{
	std::vector<TS_String> arrStrSrc;
	std::vector<TS_String> arrStrDst;

	UINT nSrcSize = StringHelper::SplitStringEx( pszSrc, arrStrSrc, _T('.') );
	UINT nDstSize = StringHelper::SplitStringEx( pszDst, arrStrDst, _T('.'));

	for (unsigned int i=0; i< min( nSrcSize, nDstSize ); i++ )
	{
		int iSrc = _ttoi(arrStrSrc[i].c_str());
		int iDst = _ttoi(arrStrDst[i].c_str());
		if ( iSrc > iDst ) 
		{
			return 1;
		}
		else if ( iSrc < iDst )
		{
			return -1;
		}
	}

	if( nSrcSize > nDstSize )
	{
		return 1;
	}
	else if ( nSrcSize < nDstSize )
	{
		return -1;
	}

	return 0;
}

static bool GetIEExploereVersion(LPTSTR pszVersion, DWORD dwSize)
{
	if (!pszVersion || dwSize<=0)
		return false;

	DWORD dwType = REG_DWORD;
	if ( ERROR_SUCCESS == SHGetValue(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Internet Explorer"), _T("Version"), &dwType, pszVersion, &dwSize) && _T('\0') != pszVersion[0]) return true;
	else {
		TCHAR szIEPath[MAX_PATH] = {};
		if( _T('\0') == szIEPath[0] ) {
			SHGetSpecialFolderPath(NULL, szIEPath, CSIDL_PROGRAM_FILES, FALSE);
			PathAppend(szIEPath, _T("internet explorer\\iexplore.exe"));
            return !!os::FileVersion::GetFileVersion(szIEPath, pszVersion, dwSize);
		}
	}
	
	return false;
}

long os::FileVersion::GetIEVersion()
{
	static long lIeVer = IE_VERSION_LAST;
	if( lIeVer == IE_VERSION_LAST ) {
		TCHAR szIeVersion[MAX_PATH + 1] = {};
		GetIEExploereVersion(szIeVersion, MAX_PATH);
		if( _T('6') == szIeVersion[0] ) lIeVer = IE_VERSION_6;
		else if( _T('7') == szIeVersion[0] ) lIeVer = IE_VERSION_7;
		else if( _T('8') == szIeVersion[0] ) lIeVer = IE_VERSION_8;
		else if( _T('9') == szIeVersion[0] ) lIeVer = IE_VERSION_9;
	}
	
	return lIeVer;
}

//////////////////////////////////////////////////////////////////////////
// 网络 [houzhigang]此函数会导致wininet卡死，不要使用
/*BOOL SystemCommon::NetHelper::HaveInternetConnection()
{
	return TRUE;
	DWORD dwFlag = 0;
	
	//	如果函数返回FALSE,则肯定没有连接
	if ( !InternetGetConnectedState( &dwFlag, 0 ) )
		return FALSE;
	
	//	如果OFFLINE被置位，则可能是没有连接，也可能是实际上有连接存在
	//	为了尽可能不干扰用户的正常使用，也认为是没有连接
	if ( dwFlag & INTERNET_CONNECTION_OFFLINE )
		return FALSE;
	
	//	必须有下列任意一种连接，才认为是有连接
	//	因为INTERNET_CONNECTION_CONFIGURED被置位也可能是没有连接
	if ( ( dwFlag & INTERNET_CONNECTION_LAN ) ||
		( dwFlag & INTERNET_CONNECTION_MODEM ) ||
		( dwFlag & INTERNET_CONNECTION_PROXY ) )
		return TRUE;
	
	return FALSE;
}*/


BOOL NetHelper::GetDomain( LPCTSTR url, LPTSTR buf, int cchBuf)
{
	if (!url || !url[0] || !buf || cchBuf<=0)
		return FALSE;

	URL_COMPONENTS uc = {};
	uc.dwStructSize = sizeof(uc);
	uc.lpszHostName = buf;
	uc.dwHostNameLength = cchBuf;
	
	memset( buf,0, cchBuf * sizeof(TCHAR));
	__try
	{
		if( !InternetCrackUrl( url, lstrlen(url), (DWORD)ICU_DECODE, &uc ) )
		{
			// 返回为FALSE,
			// 当url = www.sohu.com/dasfa.php?sdfa=3324234 时,如果不返回False,
			// 那么会得到一个错误的Domain:www.sohu.com/dasfa.php?sdfa=3324234,导致后面的逻辑错误
			return FALSE;
		}
		else
		{
			LPTSTR lpstr = StrStr(buf,_T("\\"));
			if( lpstr )
				*lpstr = 0;
		}
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
	}
	
	if( _tcslen(buf) == 0 )
		return FALSE;
	return TRUE;
}

int StringHelper::SplitString(LPCTSTR lpszSource, LPCTSTR lpszStep, std::vector<TS_String> &arrStr)
{
	arrStr.clear();
	if (lpszSource && lpszSource[0] && lpszStep && lpszStep[0])
	{
		int nLength = _tcslen(lpszSource);
		TCHAR * lpszSliptSource = new TCHAR[nLength + 1];
		if (lpszSliptSource)
		{
			memset(lpszSliptSource, 0, sizeof(TCHAR) * (nLength + 1));
			_sntprintf_s(lpszSliptSource, nLength + 1, _TRUNCATE, _T("%s"), lpszSource);
#if _MSC_VER < 1500
			TCHAR * lpszResult = _tcstok(lpszSliptSource, lpszStep);
#else
			TCHAR * lpNextToken = NULL;
			TCHAR * lpszResult = _tcstok_s(lpszSliptSource, lpszStep, &lpNextToken);
#endif
			while(lpszResult != NULL)
			{
				arrStr.push_back(lpszResult);
#if _MSC_VER < 1500
				lpszResult = _tcstok(NULL, lpszStep);
#else
				lpszResult = _tcstok_s(NULL, lpszStep, &lpNextToken);
#endif
			}
            delete [] lpszSliptSource;
		}
	}
	return arrStr.size();
}

int StringHelper::SplitStringEx(LPCTSTR lpszStr, std::vector<TS_String> &strlist, TCHAR chSeparator/* = '\n'*/, size_t iMaxCount/* = 0x7fffffff*/)
{
    strlist.clear();
    
    int iLast = 0;
    for (int i = 0; lpszStr[i]; i++)
    {
        if (lpszStr[i] == chSeparator)
        {
            strlist.push_back(TS_String(lpszStr + iLast, i - iLast));
            if (iMaxCount == strlist.size())
                break;
            iLast = i + 1;
        }
    }
    if (iMaxCount > strlist.size())
        strlist.push_back(lpszStr + iLast);
    
    return strlist.size();
}

int StringHelper::SplitStringExA(LPCSTR lpszStr, std::vector<std::string> &strlist, CHAR chSeparator/* = '\n'*/, size_t iMaxCount/* = 0x7fffffff*/)
{
    strlist.clear();
    
    int iLast = 0;
    for (int i = 0; lpszStr[i]; i++)
    {
        if (lpszStr[i] == chSeparator)
        {
            strlist.push_back(std::string(lpszStr + iLast, i - iLast));
            if (iMaxCount == strlist.size())
                break;
            iLast = i + 1;
        }
    }
    if (iMaxCount > strlist.size())
        strlist.push_back(lpszStr + iLast);
    
    return strlist.size();
}

void StringHelper::EnsureStringEnd(LPTSTR lpszDes, LPCTSTR lpszEnd, DWORD dwSize)
{
	if (lpszDes && lpszEnd)
	{
		BOOL bConnect = TRUE;
		size_t nDesLength = _tcslen(lpszDes);
		size_t nEndLength = _tcslen(lpszEnd);
		if (nDesLength >= _tcslen(lpszEnd))
		{
			TCHAR * pszFind = lpszDes + (nDesLength - nEndLength);
			if (pszFind)
			{
				if (StrCmpI(pszFind, lpszEnd) == 0)
					bConnect =FALSE;
			}
		}

		if (bConnect)
			_tcsncat_s(lpszDes, nDesLength + nEndLength, lpszEnd, nEndLength);
	}
}

TS_String& StringHelper::Trim(TS_String &s)   
{  
	if(s.empty())   
	{  
		return s;  
	}  
	
	s.erase(0,s.find_first_not_of(_T(" ")));  
	s.erase(s.find_last_not_of(_T(" ")) + 1);  
	return s;  
}  
TS_String& StringHelper::ReplaceAll(TS_String& str,const TS_String& old_value,const TS_String& new_value)  
{  
	for(TS_String::size_type pos(0); pos!=TS_String::npos; pos+=new_value.length())
	{  
		if((pos=str.find(old_value,pos))!=TS_String::npos )  
			str.replace(pos,old_value.length(),new_value);  
		else break;  
	}  
	return str;  
} 
TS_String& StringHelper::MakeLower(TS_String &s)   
{  
	if(s.empty())   
	{  
		return s;  
	}  
	
	TCHAR* pBuf = new TCHAR[s.length()+1];
	_tcsncpy_s(pBuf, s.length()+1, s.c_str(), _TRUNCATE);
#if _MSC_VER < 1500
	_tcslwr(pBuf);
#else
	_tcslwr_s(pBuf, s.length() + 1);
#endif
	s = pBuf;
	delete [] pBuf;
	
	return s;  
}  

BOOL StringHelper::StrICompare(TS_String sA, TS_String sB)   
{  
	MakeLower( sA );
	MakeLower( sB );
	if( 0 == sA.compare(sB))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}  

int StringHelper::StrIFind(TS_String sA, TS_String sB)   
{  
	MakeLower( sA );
	MakeLower( sB );
	return sA.find(sB);
}  

#define MAX_FORMATE_BUF 16*1024
static TS_String StringFormat_X(const TCHAR *fmt, ...) 
{ 
	if (!fmt || !fmt[0])
		return TS_String(fmt);

	TS_String sReturn = fmt;
	try
	{
		va_list arglist;
		va_start(arglist, fmt);

		int nLen = _vsctprintf( fmt, arglist ) +1; // _vscprintf doesn't count; terminating '\0'
		TCHAR* pszBuf = new TCHAR[nLen];

#if _MSC_VER < 1500
		_vstprintf( pszBuf, fmt, arglist );
#else
		_vstprintf_s( pszBuf, nLen, fmt, arglist );
#endif
		va_end(arglist);
		
		sReturn = pszBuf;

		delete [] pszBuf;
	}
	catch (...)
	{
		ATLASSERT(FALSE);
	}
	return sReturn;
}

std::string StringHelper::Wstring2String( const wchar_t* pwc )
{
	if (!pwc || !pwc[0])
		return "";

	std::string result;
	
	int nLen=WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pwc, -1, NULL, 0, NULL, NULL );
	if(nLen<=0)
	{
		return std::string( "" );
	}
	char *presult=new char[nLen];
	if ( NULL == presult ) 
	{
		return std::string("");
	}
	WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pwc, -1, presult, nLen, NULL, NULL );
	presult[nLen-1]=0;
	result = presult;
	delete [] presult;

    return result;
}

std::wstring StringHelper::String2WString( const char* pc )
{
	if (!pc || !pc[0])
		return L"";

	int nLen = strlen( pc );
	int nSize = MultiByteToWideChar( CP_ACP, 0, (LPCSTR)pc, nLen, 0, 0 );
	if ( nSize<=0 ) 
	{
		return std::wstring( L"" );
	}
	WCHAR *pDst=new WCHAR[nSize+1];
	if ( NULL == pDst )
	{
		return NULL;
	}
	MultiByteToWideChar( CP_ACP, 0, (LPCSTR)pc, nLen, pDst, nSize );
	pDst[nSize]=0;
	if ( 0xFEFF == pDst[0] )
	{
		for (int i=0;i<nSize;i++)
		{
			pDst[i]=pDst[i+1];
		}
	}
	std::wstring wcstr(pDst);
	delete [] pDst;
	return wcstr;
}

std::string StringHelper::TransWStringToUTF8(const wchar_t* apFrom)
{
	if (!apFrom || !apFrom[0])
		return "";

    int liOutNeedLen = WideCharToMultiByte(CP_UTF8, 0, apFrom, -1, NULL, NULL, NULL, NULL);
    if (liOutNeedLen > 0)
    {
        std::vector<char> lvcWideChar(liOutNeedLen);
        char * lpBuff = &lvcWideChar.front();
		
        liOutNeedLen = WideCharToMultiByte(CP_UTF8, 0, apFrom, -1, lpBuff, liOutNeedLen, NULL, NULL);
        if (liOutNeedLen > 0)
        {
            return std::string(lpBuff);
        }
    }
    return "";
}

std::wstring StringHelper::TransUTF8ToWString(const char * apFrom)
{
	if (!apFrom || !apFrom[0])
		return L"";

    int liOutNeedLen = MultiByteToWideChar(CP_UTF8, 0, apFrom, -1, NULL, 0);
    if (liOutNeedLen > 0)
    {
        std::vector<wchar_t> lvcWideChar(liOutNeedLen);
        wchar_t * lpBuff = &lvcWideChar.front();
		
        liOutNeedLen = MultiByteToWideChar(CP_UTF8, 0, apFrom, -1, lpBuff, liOutNeedLen);
        if (liOutNeedLen > 0)
        {
            return std::wstring(lpBuff);
        }
    }
    return L"";
}






//////////////////////////////////////////////////////////////////////////
// 加密，解密的函数
namespace os
{
	namespace Descode
	{
		#define BASE_XOR_VALUE	(0x75127602)
		#define BASE_PLUS_VALUE	(0x00990011)

		// sizeof(pOutput) >= dwInputLen * 2
		void SimpleFormEncode( char* pInput, DWORD dwInputLen, char* pOutput )
		{
			char cHigh, cLow;

			for( DWORD i = 0; i < dwInputLen; i++ )
			{
				cHigh = (BYTE)pInput[i] >> 4;
				if( cHigh >= 10 )
					*pOutput++ = ( cHigh - 10 ) + 'A';
				else
					*pOutput++ = cHigh + '0';

				cLow = pInput[i] & 0x0F;
				if( cLow >= 10 )
					*pOutput++ = ( cLow - 10 ) + 'A';
				else
					*pOutput++ = cLow + '0';
			}
		}

		// pInput 和 pOutput 可以相同
		void SimpelFormDecode( char* pInput, DWORD dwInputLen, char* pOutput )
		{
			char cHigh, cLow;
			
			assert( dwInputLen % 2 == 0 );

			for( DWORD i = 0; i < dwInputLen; i += 2 )
			{
				// cHigh
				if( pInput[i] >= 'A' )
					cHigh = pInput[i] - 'A' + 10;
				else
					cHigh = pInput[i] - '0';

				// cLow
				if( pInput[i + 1] >= 'A' )
					cLow = pInput[i + 1] - 'A' + 10;
				else
					cLow = pInput[i + 1] - '0';

				*pOutput++ = cLow | ( (BYTE)cHigh << 4 );
			}
		}

		//	加密字符串。
		//	strInput为输入字符串，dwKey为一个密钥
		//	转换成功返回一个字符串指针，该字符串必须由调用者free() !!!!!!!!
		//	转换失败返回NULL
		//	算法对加密后的串做BASE64编码
		char* EncryptString(char* strInput, DWORD dwKey)
		{
			char* strDup;
			int nLen;
			nLen=strlen(strInput);

			strDup=(char*)malloc((nLen+1)*sizeof(char));
			if(strDup==NULL)
				return NULL;

			int i;
			srand(dwKey);
			for(i=0;i<nLen;i++)
				strDup[i] = (char)(strInput[i] ^ ((char)(rand()+BASE_PLUS_VALUE)) ^ BASE_XOR_VALUE);

			int nFormLen = nLen * 2 + 1;
			char* pForm = (char*)malloc( nFormLen );
			if( NULL != pForm )
			{
				SimpleFormEncode( strDup, nLen, pForm );
				pForm[ nLen * 2 ] = '\0';
			}

			free(strDup);

			return pForm;
		}

		//	对strEncrypt的密文进行解码，该密文必须由EncryptString加密的
		//	如果失败，返回NULL
		//	成功，返回一块指向原文字符串的缓冲区，由调用者free() !!!!!!!
		void DecryptString(char* strEncrypt, DWORD dwKey)
		{
			int nDecodeLen;

			nDecodeLen = strlen(strEncrypt);
			SimpelFormDecode( strEncrypt, nDecodeLen, strEncrypt );

			nDecodeLen /= 2;
			srand(dwKey);
			for( int i = 0; i < nDecodeLen; i++ )
				strEncrypt[i] ^= ((char)(rand()+BASE_PLUS_VALUE)) ^ BASE_XOR_VALUE;
			strEncrypt[nDecodeLen] = '\0';
		}

		DWORD GetVolumeNumber()
		{
			TCHAR tzVolumeNameBuffer[MAX_PATH+2]={};
			TCHAR tzFileSystemNameBuffer[MAX_PATH+2]={};
			DWORD dwVolumeNameSize = MAX_PATH+1;
			DWORD dwFileSystemNameSize = MAX_PATH+1;

			DWORD dwMaximumComponentLength=0;
			DWORD dwFileSystemFlags=0;

			DWORD dwVolumeSerialNumber = 0;
			if(!GetVolumeInformation(_T("C:\\"),tzVolumeNameBuffer,dwVolumeNameSize,
				&dwVolumeSerialNumber,&dwMaximumComponentLength,&dwFileSystemFlags,
				tzFileSystemNameBuffer,dwFileSystemNameSize) || !dwVolumeSerialNumber)
				dwVolumeSerialNumber = 1985;
			return dwVolumeSerialNumber;
		}

		LPSTR EncryptPassword(LPCTSTR lpszSrc)
		{
			LPSTR lpstrPassword = NULL;
			static DWORD dwKey = GetVolumeNumber();
			if(lpszSrc)
			{
				USES_CONVERSION;
				CHAR*	psData = T2A(lpszSrc);
				lpstrPassword = EncryptString(psData, dwKey);
			}
			
			return lpstrPassword;
		}

		LPSTR DecryptPassword(LPCTSTR lpszSrc)
		{
			LPSTR lpstrPassword = NULL;
			static DWORD dwKey = GetVolumeNumber();
			if(lpszSrc && _tcslen(lpszSrc)%2 == 0)
			{
				USES_CONVERSION;
				CHAR* psData = T2A(lpszSrc);
				DecryptString(psData,dwKey);
				if(psData)
				{
					int nLength = strlen(psData);
					if(nLength>0 && nLength<100)
					{
						lpstrPassword = (CHAR*)malloc((nLength+1)*sizeof(CHAR));
						if(lpstrPassword)
						{
							memset(lpstrPassword , 0 , (nLength+1)*sizeof(CHAR));
							_snprintf_s(lpstrPassword , nLength, _TRUNCATE, "%s" , psData);
						}
					}
				}
			}	
			return lpstrPassword;
		}

		LPTSTR DecryptPassword( LPCTSTR lpszSrc, LPTSTR lpszDes, int nMax )
		{
			if ( lpszDes )
			{
				LPSTR pszPass = DecryptPassword( lpszSrc );
				if ( pszPass )
				{
					USES_CONVERSION;
					_sntprintf_s( lpszDes, nMax-1, _TRUNCATE, _T("%s"), A2T( pszPass ) );
					free( pszPass );
				}
			}
			return lpszDes;
		}

		LPTSTR EncryptPassword( LPCTSTR lpszSrc, LPTSTR lpszDes, int nMax )
		{
			if ( lpszDes )
			{
				LPSTR pszPass = EncryptPassword( lpszSrc );
				if ( pszPass )
				{
					USES_CONVERSION;
					_sntprintf_s( lpszDes, nMax-1, _TRUNCATE, _T("%s"), A2T(pszPass) );
					free( pszPass );
				}
			}
			return lpszDes;
		}
	}
    namespace Process
    {
        BOOL AysnCreateProcess(LPCTSTR lpszCmdLine, WORD dwShowWindow, DWORD dwTimeout)
        {
            BOOL bRet = TRUE;
            CREATEPROCESS_PARAM* pParam = new CREATEPROCESS_PARAM;
            memset(pParam, 0, sizeof(CREATEPROCESS_PARAM));
            if (lpszCmdLine)
                _sntprintf_s(pParam->szCmdLine, _countof(pParam->szCmdLine), L"%s", lpszCmdLine);
            pParam->dwShowWindow = dwShowWindow;
            pParam->dwTimeout = dwTimeout;

            HANDLE hThread = CreateThread(NULL, 0, CreateProcessProc, (LPVOID)pParam, 0, NULL);
            if (dwTimeout > 0)
            {
                if (WAIT_TIMEOUT == WaitForSingleObject(hThread, dwTimeout))
                {
					SuspendThread(hThread);
                    TerminateThread(hThread, -1);
                    bRet = FALSE;
                }
            }
            CloseHandle(hThread);
            return bRet;
        }

        BOOL AysnShellExecute(LPCTSTR lpszOperation, LPCTSTR lpszFilePath, LPCTSTR lpszParam, LPCTSTR lpszDirectory, int nShowCmd)
        {
            SHELLEXECUTE_PARAM* pParam = new SHELLEXECUTE_PARAM;
            memset(pParam, 0, sizeof(SHELLEXECUTE_PARAM));
            if (lpszOperation)
                _sntprintf_s(pParam->szOperation, _countof(pParam->szOperation), lpszOperation);
            if (lpszFilePath)
                _sntprintf_s(pParam->szFilePath, _countof(pParam->szFilePath), lpszFilePath);
            if (lpszParam)
                _sntprintf_s(pParam->szParam, _countof(pParam->szParam), lpszParam);
            if (lpszDirectory)
                _sntprintf_s(pParam->szDirectory, _countof(pParam->szDirectory), lpszDirectory);
            pParam->nShowCmd = nShowCmd;

            HANDLE hThread = CreateThread(NULL, 0, ShellExecuteProc, (LPVOID)pParam, 0, NULL);
            CloseHandle(hThread);
            return TRUE;
        }

        DWORD WINAPI CreateProcessProc(LPVOID lpParam)
        {
            if (lpParam)
            {
                CREATEPROCESS_PARAM* pParam = (CREATEPROCESS_PARAM*)lpParam;

                STARTUPINFO si = {};
                PROCESS_INFORMATION pi = {};
                si.cb = sizeof(si);
                si.dwFlags = STARTF_FORCEOFFFEEDBACK | STARTF_USESHOWWINDOW;
                si.wShowWindow = pParam->dwShowWindow;
                
                if (::CreateProcess(NULL, pParam->szCmdLine, NULL, NULL, TRUE, 0, 0, 0, &si, &pi))
                {   
                    // Close process and thread handles. 
                    if (pParam->dwTimeout > 0)
                    {
                        DWORD dwRet = WaitForSingleObject(pi.hProcess, pParam->dwTimeout);
                    }
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                } 
                delete pParam;
            }
            return 0;
        }

        DWORD WINAPI ShellExecuteProc(LPVOID lpParam)
        {
            if (lpParam)
            {
                SHELLEXECUTE_PARAM* pParam = (SHELLEXECUTE_PARAM*)lpParam;
                ShellExecute(NULL, pParam->szOperation, pParam->szFilePath, pParam->szParam, pParam->szDirectory, pParam->nShowCmd);
                delete pParam;
            }
            return 0;
        }


		void SafeTerminateThread(HANDLE hThread, DWORD dwExitCode/* = -1*/)
		{
			::SuspendThread(hThread);
			::TerminateThread(hThread, dwExitCode);
			::CloseHandle(hThread);
		}

		TS_String GetGuid()
		{
			TS_String strGuid;
			GUID guid;
			CoInitialize(NULL);
			if (S_OK == ::CoCreateGuid(&guid))
			{
				strGuid = StringFormat_X(_T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
					guid.Data1,
					guid.Data2,
					guid.Data3,
					guid.Data4[0], guid.Data4[1],
					guid.Data4[2], guid.Data4[3],
					guid.Data4[4], guid.Data4[5],
					guid.Data4[6], guid.Data4[7]);
			}
			CoUninitialize();
			return strGuid;
		}

    }
	
    namespace Window
    {
        HWND FindWindowByProcess(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD nProcessID)
        {
            HWND hWnd = NULL;
            if (nProcessID == 0) nProcessID = GetCurrentProcessId();
            DWORD dwWindowProcessID = 0;
            do 
            {
                hWnd = FindWindowEx(NULL, hWnd, lpszClassName, lpszWindowName);
                if (hWnd != NULL)
                {
                    GetWindowThreadProcessId(hWnd, &dwWindowProcessID);
                    if (dwWindowProcessID == nProcessID)
                        break;
                }
            } while (hWnd != NULL);
            
            return hWnd;
        }
    }

	namespace ValidSystem
	{
		// 统一化路径
		std::wstring GetNormalPath(LPCTSTR lpszPath)
		{
			std::wstring strNormal = L"";
			if(lpszPath && lpszPath[0])
			{
				TCHAR szDes[MAX_PATH + 1] = {0};
				int i = 0;
				while (lpszPath[i] && i < MAX_PATH)
				{
					if(lpszPath[i] == _T('/'))
						szDes[i] = _T('\\');
					else
						szDes[i] = lpszPath[i];

					i++;
				}

				TCHAR szBuffer[MAX_PATH] = {0}; 
				TCHAR *lpStrDes;
				lpStrDes = szBuffer;
				if(PathCanonicalize(lpStrDes, szDes))
					strNormal = szBuffer;
			}
			return strNormal;
		}

		// 判断该文件是否是统一化的路径
		bool IsNormalPath(LPCTSTR lpszPath)
		{
			bool bRet = false;
			if (lpszPath && lpszPath[0])
			{
				TCHAR szPathTemp[MAX_PATH + 1] = {0};
				_sntprintf_s(szPathTemp, MAX_PATH, L"%s", lpszPath);
				PathRemoveExtension(szPathTemp);
				if (!(StrStrI(szPathTemp, L"/") ||
					StrStrI(szPathTemp, L"?") || StrStrI(szPathTemp, L"\"") || StrStrI(szPathTemp, L"*") ||
					StrStrI(szPathTemp, L"<") || StrStrI(szPathTemp, L">") || StrStrI(szPathTemp, L"|") || StrStrI(szPathTemp, L"%")
					|| StrStrI(szPathTemp, L".")))
					bRet = true;
			}
			return bRet;
		}

		// 判断文件名是否有效
		bool IsFileNameValid(LPCTSTR lpszFileName)
		{
			// : ? " < > | \ / % *
			bool bRet = false;
			if (lpszFileName && lpszFileName[0])
			{
				if (!(StrStrI(lpszFileName, L".") || StrStrI(lpszFileName, L"\\") || StrStrI(lpszFileName, L"/") ||
					StrStrI(lpszFileName, L":") || StrStrI(lpszFileName, L"?") || StrStrI(lpszFileName, L"\"") || StrStrI(lpszFileName, L"*") ||
					StrStrI(lpszFileName, L"<") || StrStrI(lpszFileName, L">") || StrStrI(lpszFileName, L"|") || StrStrI(lpszFileName, L"%")))
					bRet = true;
			}
			return bRet;
		}

		BOOL GetDomain(LPCTSTR url, LPTSTR buf, int cbBuf)
		{
			URL_COMPONENTS uc;
			ZeroMemory(&uc, sizeof(uc));

			uc.dwStructSize = sizeof(uc);
			uc.lpszHostName = buf;
			uc.dwHostNameLength = cbBuf;

			memset(buf,0,cbBuf*sizeof(TCHAR));

			if(!InternetCrackUrl(url, _tcslen(url), (DWORD)ICU_DECODE, &uc))
				return FALSE;

			if( NULL == buf || buf[0] == 0 )
				return FALSE;

			return TRUE;
		}

		// 判断该路径是否是根路径的子路径
		bool ValidPathIsSub(LPCTSTR lpszRoot, LPCTSTR lpszSub)
		{
			bool bRet = false;
			if(lpszRoot && lpszRoot[0] && lpszSub && lpszSub[0])
			{
				std::wstring strRoot = GetNormalPath(lpszRoot);
				std::wstring strSub = GetNormalPath(lpszSub);
				if(IsNormalPath(strRoot.c_str()) && IsNormalPath(strSub.c_str()))
				{
					if(StrStrI(strSub.c_str(), strRoot.c_str()) == strSub.c_str())
						return true;
				}
			}
			return bRet;
		}

		// 判断该文件是否是指定扩展名的文件
		bool IsFileSpecifiedExt(LPCTSTR lpszFile, LPCTSTR lpszExt)
		{
			bool bRet = false;
			if(lpszFile == NULL || !lpszFile[0])
				return bRet;

			if(lpszExt == NULL || !lpszExt[0])
				return bRet;

			std::wstring strNormal = GetNormalPath(lpszFile);
			if(strNormal.length() > 0 && PathFileExists(strNormal.c_str()))
			{
				LPTSTR  pszSub = PathFindExtension(strNormal.c_str());
				if(pszSub)
				{
					if(StrCmpI(lpszExt, pszSub) == 0)
						bRet = true;
					else
					{
						TCHAR szExt[MAX_PATH + 1] = {0};
						_sntprintf_s(szExt, MAX_PATH, L"%s|", pszSub);
						if(StrStrI(lpszExt, pszSub))
							bRet = true;
					}
				}
			}

			return bRet;
		}

		// 判断url是否属于一个子域名
		bool IsUrlIsSubUrl(LPCTSTR lpszUrl, LPCTSTR lpszDomain)
		{
			bool bRet = false;
			if(lpszUrl && lpszUrl[0] && lpszDomain && lpszDomain[0])
			{
				// 必须是http://或者https://
				bool bHttp = false;
				//LPTSTR lpszHttp = StrStrI(lpszUrl, L"http://");
				if(_tcsncmp(lpszUrl, L"http://", 7) == 0)
					bHttp = true;
				else if(_tcsncmp(lpszUrl, L"https://", 8) == 0)
					bHttp = true;

				if(bHttp)
				{
					TCHAR szDoMain[MAX_PATH + 1] = {0};
					if(GetDomain(lpszUrl, szDoMain, MAX_PATH))
					{
						if(PathMatchSpec(szDoMain, lpszDomain))
							bRet = true;
					}
				}
			}
			return bRet;
		}

		bool IsValidDownloadUrl(LPCTSTR lpszDownUrl)
		{
			// 有种情况会传入""，不能返回FALSE
			bool bRet = true;
			return bRet;
		}

		bool IsPEFile(LPCTSTR lpszFilePath)
		{
			bool bRet = false;
			if(lpszFilePath == NULL || !lpszFilePath[0])
				return bRet;

			std::wstring strNormal = GetNormalPath(lpszFilePath);
			if (PathFileExists(strNormal.c_str()))
			{
				if(IsFileSpecifiedExt(strNormal.c_str(), L".exe|.scr|.bat|.dll|.com|.cmd|"))
					bRet = true;
				else
				{
					HANDLE hFile = ::CreateFile(strNormal.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile != INVALID_HANDLE_VALUE)
					{
						// 定义PE文件中的DOS头和NT头
						IMAGE_DOS_HEADER dosHeader;
						DWORD dwRead = 0;

						// 读取DOS头
						::ReadFile(hFile, &dosHeader, sizeof(dosHeader), &dwRead, NULL);
						if (dwRead == sizeof(dosHeader))
						{
							if (dosHeader.e_magic == IMAGE_DOS_SIGNATURE) // 是不是有效的DOS头
								bRet = true;
						}

						CloseHandle(hFile);
					}
				}
			}
			return bRet;
		}
	}

    //调试相关
	namespace DebugHelper
	{
#pragma optimize("", off)
		void InvalidParameter(const wchar_t* expression, const wchar_t* function,
			const wchar_t* file, unsigned int line,
			uintptr_t reserved) 
		{
			__debugbreak();
			_exit(1);
		}

		void PureCall() 
		{
			__debugbreak();
			_exit(1);
		}

#pragma warning(push)
#pragma warning(disable : 4748)
		void OnNoMemory() {
			__debugbreak();
			_exit(1);
		}
#pragma warning(pop)
#pragma optimize("", on)


		void RegisterInvalidParamHandler()
		{
			_set_invalid_parameter_handler(InvalidParameter);
			_set_purecall_handler(PureCall);
			std::set_new_handler(&OnNoMemory);
			_set_new_mode(1);
		}
	}//namespace DebugHelper    

	BOOL CreateFileShortcut(LPCTSTR lpszLnkFileFullPath,LPCTSTR lpszTargetExeFullPath)
	{
		if (lpszLnkFileFullPath == NULL ||
			0 == _tcscmp(lpszLnkFileFullPath,_T("")))
		{
			return FALSE;
		}

		if (lpszTargetExeFullPath == NULL ||
			0 == _tcscmp(lpszLnkFileFullPath,_T("")))
		{
			return FALSE;
		}

		HRESULT hr;
		IShellLink     *pLink;  //IShellLink对象指针
		IPersistFile   *ppf; //IPersisFil对象指针

		//创建IShellLink对象
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
		if (FAILED(hr))
			return FALSE;

		//从IShellLink对象中获取IPersistFile接口
		hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (FAILED(hr))
		{
			pLink->Release();
			return FALSE;
		}

		//目标
		pLink->SetPath(lpszTargetExeFullPath);
		//快捷键
		//pLink->SetHotkey(wHotkey);
		//备注
		//pLink->SetDescription(lpszDescription);
		//显示方式
		pLink->SetShowCmd(SW_SHOWNORMAL);
		pLink->SetWorkingDirectory(os::FilePath::GetPath(lpszTargetExeFullPath).c_str());

		//保存快捷方式到指定目录下
		hr = ppf->Save(lpszLnkFileFullPath, TRUE);

		ppf->Release();
		pLink->Release();
		return SUCCEEDED(hr);
	}

	BOOL CreateFileShortcutEx(
		const std::wstring& wstrLnkFileFullPath, 
		const std::wstring& wstrTargetExeFullPath,
		const std::wstring& wstrParam,
		const std::wstring& wstrIcon,
		ptrdiff_t IconIndex, 
		const std::wstring& wstrWorkDir)
	{
		HRESULT hr;
		IShellLink     *pLink;  //IShellLink对象指针
		IPersistFile   *ppf; //IPersisFil对象指针

							 //创建IShellLink对象
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
		if (FAILED(hr))
			return FALSE;

		//从IShellLink对象中获取IPersistFile接口
		hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (FAILED(hr))
		{
			pLink->Release();
			return FALSE;
		}

		//目标
		pLink->SetPath(wstrTargetExeFullPath.c_str());
		//快捷键
		//pLink->SetHotkey(wHotkey);
		//备注
		//pLink->SetDescription(lpszDescription);
		//显示方式
		pLink->SetShowCmd(SW_SHOWNORMAL);

		if (!wstrParam.empty())
		{
			pLink->SetArguments(wstrParam.c_str());
		}
		if (!wstrIcon.empty())
		{
			pLink->SetIconLocation(wstrIcon.c_str(), IconIndex);
		}
		if (!wstrWorkDir.empty())
		{
			pLink->SetWorkingDirectory(os::FilePath::GetPath(wstrWorkDir).c_str());
		}
		else
		{
			pLink->SetWorkingDirectory(os::FilePath::GetPath(wstrTargetExeFullPath).c_str());
		}
		//保存快捷方式到指定目录下
		hr = ppf->Save(wstrLnkFileFullPath.c_str(), TRUE);

		ppf->Release();
		pLink->Release();
		return SUCCEEDED(hr);
	}






// /**
//  * \brief 函数功能：对指定文件在指定的目录下创建其快捷方式
//  * 
//  * \param lpszFileName    指定文件，为NULL表示当前进程的EXE文件。
//  * \param lpszLnkFileDir  指定目录，不能为NULL。
//  * \param lpszLnkFileName 快捷方式名称，为NULL表示EXE文件名。
//  * \param wHotkey         为0表示不设置快捷键
//  * \param pszDescription  备注
//  * \param iShowCmd        运行方式，默认为常规窗口
// */
// BOOL CreateFileShortcut(LPCTSTR lpszFileName, 
// 						LPCTSTR lpszLnkFileDir, 
// 						LPCTSTR lpszLnkFileName,       
// 						LPCTSTR lpszWorkDir, 
// 						WORD wHotkey, 
// 						LPCTSTR lpszDescription, 
// 						int iShowCmd /*= SW_SHOWNORMAL*/)
// {
// 	if (lpszLnkFileDir == NULL)
// 		return FALSE;
// 
// 	HRESULT hr;
// 	IShellLink     *pLink;  //IShellLink对象指针
// 	IPersistFile   *ppf; //IPersisFil对象指针
// 
// 	//创建IShellLink对象
// 	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
// 	if (FAILED(hr))
// 		return FALSE;
// 
// 	//从IShellLink对象中获取IPersistFile接口
// 	hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
// 	if (FAILED(hr))
// 	{
// 		pLink->Release();
// 		return FALSE;
// 	}
// 	WCHAR MyPath[1024] = {0};
// 	GetModuleFileName(NULL,MyPath,sizeof(MyPath));
// 	//目标
// 	if (lpszFileName == NULL)
// 		pLink->SetPath(MyPath);
// 	else
// 		pLink->SetPath(lpszFileName);
// 
// 	//工作目录
// 	if (lpszWorkDir != NULL)
// 		pLink->SetPath(lpszWorkDir);
// 
// 	//快捷键
// 	if (wHotkey != 0)
// 		pLink->SetHotkey(wHotkey);
// 
// 	//备注
// 	if (lpszDescription != NULL)
// 		pLink->SetDescription(lpszDescription);
// 
// 	//显示方式
// 	pLink->SetShowCmd(iShowCmd);
// 
// 
// 	//快捷方式的路径 + 名称
// 	wchar_t szBuffer[MAX_PATH];
// 	if (lpszLnkFileName != NULL) //指定了快捷方式的名称
// 		wsprintf(szBuffer, L"%s\\%s", lpszLnkFileDir, lpszLnkFileName);
// 	else  
// 	{
// 		//没有指定名称，就从取指定文件的文件名作为快捷方式名称。
// 		wchar_t *pstr;
// 		if (lpszFileName != NULL)
// 			pstr = wcsrchr(lpszFileName, L'\\');
// 		else
// 			pstr = wcsrchr(MyPath, L'\\');
// 
// 		if (pstr == NULL)
// 		{    
// 			ppf->Release();
// 			pLink->Release();
// 			return FALSE;
// 		}
// 		//注意后缀名要从.exe改为.lnk
// 		wsprintf(szBuffer, L"%s\\%s", lpszLnkFileDir, pstr);
// 		int nLen = wcslen(szBuffer);
// 		szBuffer[nLen - 3] = L'l';
// 		szBuffer[nLen - 2] = L'n';
// 		szBuffer[nLen - 1] = L'k';
// 	}
// 	//保存快捷方式到指定目录下
// 	hr = ppf->Save(szBuffer, TRUE);
// 
// 	ppf->Release();
// 	pLink->Release();
// 	return SUCCEEDED(hr);
// }

}

//
// 取父进程ID
//
DWORD ParentProcessID(LPCTSTR lpProcessName)
{
	DWORD Result = 0;
	DWORD CurrentProcessId = GetCurrentProcessId();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		for (BOOL b = Process32First(hSnap, &pe); b; b = Process32Next(hSnap, &pe))
		{
			if (pe.th32ProcessID == CurrentProcessId)
			{
				Result = pe.th32ParentProcessID;
				break;
			}
		}
		CloseHandle(hSnap);
	}
	return Result;
}

// Find process
// 找指定进程名的进程ID
//
DWORD FindProc(LPCTSTR lpProcessName)
{
	DWORD Result = 0;
	DWORD CurrentProcessId = GetCurrentProcessId();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		for (BOOL b = Process32First(hSnap, &pe); b; b = Process32Next(hSnap, &pe))
		{
			if (lpProcessName) //取进程ID
			{
				if (_tcsicmp(pe.szExeFile, lpProcessName) == 0)
				{
					Result = pe.th32ProcessID;
					break;
				}
			}
		}
		CloseHandle(hSnap);
	}
	return Result;
}

BOOL KillProcessByName(LPCTSTR lpProcessName)
{
	if (!lpProcessName || !lpProcessName[0])
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	DWORD ProcessID = 0;
	do
	{
		ProcessID = FindProc(lpProcessName);
		if (ProcessID)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
			if (hProcess)
			{
				if (TerminateProcess(hProcess, 0))
				{
					bResult = TRUE;
				}
				CloseHandle(hProcess);
			}
		}
	} while (ProcessID > 0);

	return bResult;
}

//
// Start service
//
BOOL StartServiceByName(LPCTSTR ServiceName)
{
	BOOL bResult = FALSE;
	SC_HANDLE hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hManager)
	{
		SC_HANDLE hService = OpenService(hManager, ServiceName, SERVICE_START | SERVICE_STOP);
		if (hService)
		{
			bResult = StartService(hService, 0, NULL);
			CloseServiceHandle(hService);
		}
		CloseServiceHandle(hManager);
	}
	return bResult;
}

//
// Stop a service.
//
BOOL StopServiceByName(LPCTSTR ServiceName)
{
	BOOL bResult = FALSE;
	SC_HANDLE hManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hManager)
	{
		SC_HANDLE hService = OpenService(hManager, ServiceName, SERVICE_START | SERVICE_STOP);
		if (hService)
		{
			SERVICE_STATUS sStatus;
			bResult = ControlService(hService, SERVICE_CONTROL_STOP, &sStatus);
			CloseServiceHandle(hService);
		}
		CloseServiceHandle(hManager);
	}
	return bResult;
}