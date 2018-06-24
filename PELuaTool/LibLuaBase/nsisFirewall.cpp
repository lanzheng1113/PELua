/**
 * \file nsisFirewall.cpp 
 * \brief 此文件从 http://wiz0u.free.fr/prog/nsisFirewall 下载。移除了NSIS导出函数部分。
 * Copyright (c) 2007-2009 Olivier Marcoux
 * \author Olivier Marcoux
 */

/*
nsisFirewall -- Small NSIS plugin for simple tasks with Windows Firewall
Web site: http://wiz0u.free.fr/prog/nsisFirewall

Copyright (c) 2007-2009 Olivier Marcoux

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

#include "stdafx.h"
#include "nsisFirewall.h"
#include <tchar.h>
#include <shlwapi.h>
//#import "libid:58FBCF7C-E7A9-467C-80B3-FC65E8FCCA08"
#import "netfw.tlb"
using namespace NetFwTypeLib;

HRESULT AddAuthorizedApplication(LPCTSTR ExceptionName, LPCTSTR ProcessPath)
{
    HRESULT result = CoInitialize(NULL);
	if (FAILED(result))
        return result;
	result = REGDB_E_CLASSNOTREG;
	try
	{
		INetFwMgrPtr fwMgr(L"HNetCfg.FwMgr");
        if (fwMgr)
        {
		    INetFwAuthorizedApplicationPtr app(L"HNetCfg.FwAuthorizedApplication");
            if (app)
            {
		        app->ProcessImageFileName = ProcessPath;
		        app->Name = ExceptionName;
		        app->Scope = NET_FW_SCOPE_ALL;
		        app->IpVersion = NET_FW_IP_VERSION_ANY;
		        app->Enabled = VARIANT_TRUE;
			    fwMgr->LocalPolicy->CurrentProfile->AuthorizedApplications->Add(app);
		        result = S_OK;
            }
        }
	}
	catch (_com_error& e)
	{
        result = e.Error();
	}
    CoUninitialize();
    return result;
}

HRESULT AddAuthorizedApplication(LPCTSTR ExceptionName) // (overload) add current process
{
	TCHAR ProcessPath[MAX_PATH];
	if (!GetModuleFileName(NULL, ProcessPath, MAX_PATH))
        return HRESULT_FROM_WIN32(GetLastError());
    return AddAuthorizedApplication(ExceptionName, ProcessPath);
}

HRESULT RemoveAuthorizedApplication(LPCTSTR ProcessPath)
{
    HRESULT result = CoInitialize(NULL);
	if (FAILED(result))
        return result;
	try
	{
		INetFwMgrPtr fwMgr(L"HNetCfg.FwMgr");
        if (fwMgr)
        {

		    fwMgr->LocalPolicy->CurrentProfile->AuthorizedApplications->Remove(ProcessPath);
            result = S_OK;
        }
	}
	catch (_com_error& e)
	{
		e;
	}
    CoUninitialize();
    return result;
}
