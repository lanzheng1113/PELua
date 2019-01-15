#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "util/WinRegs.h"
#include "util/StringEx.h"
#include "util/File.h"
#include "util/Path.h"
#include <atlstr.h>
#include <set>


static bool GetSubkeys(const CString& pKeyFullPath,std::vector<std::wstring>& v)
{
	CString str1(pKeyFullPath);
	HKEY Root = (HKEY)-1;
	CString SubKey;

	if (0 == str1.Find(_T("HKEY_LOCAL_MACHINE\\")))
	{
		Root = HKEY_LOCAL_MACHINE;
		SubKey = str1.Right(str1.GetLength() - _tcslen(_T("HKEY_LOCAL_MACHINE\\")));
	}
	else
	{
		return false;
	}

	CWinRegKey reg(Root, (LPCTSTR)SubKey, FALSE);
	v = reg.EnumSubKeys();
	return true;
}

std::vector<wstring> StringVectorIntersection(const std::vector<wstring>& vA, const std::vector<wstring>& vB, bool caseInsens = true)
{
	std::vector<wstring> ret;
	//To upper case.
	std::map<wstring, wstring> sA;
	for (wstring i : vA)
	{
		std::wstring iori(i);
		if (caseInsens)
			transform(i.begin(), i.end(), i.begin(), toupper);
		sA.insert(make_pair(i,iori));
	}

	std::map<wstring, wstring> sB;
	for (wstring i : vB)
	{
		std::wstring iori(i);
		if (caseInsens)
			transform(i.begin(), i.end(), i.begin(), toupper);
		sB.insert(make_pair(i,iori));
	}

	//Z1 = A∩B = {x|x∈A,且x不∈B}
	for (auto i : sA)
	{
		if (sB.find(i.first) != sB.end())
		{
			//i was found in set B;
			ret.push_back(i.second);
		}
	}
	return ret;
}

std::vector<wstring> StringVectorComplement(const std::vector<wstring>& vA, const std::vector<wstring>& vB, bool caseInsens = true)
{
	std::vector<wstring> ret;
	//To upper case.
	std::map<wstring, wstring> mA;
	for (wstring i : vA)
	{
		wstring iori(i);
		if (caseInsens)
			transform(i.begin(), i.end(), i.begin(), toupper);
		mA.insert(make_pair(i, iori));
	}

	std::map<wstring, wstring> mB;
	for (wstring i : vB)
	{
		wstring iori(i);
		if (caseInsens)
			transform(i.begin(), i.end(), i.begin(), toupper);
		mB.insert(make_pair(i,iori));
	}

	//Z1 = A\B = {x|x∈A,且x不∈B}
	for (auto i : mA)
	{
		if (mB.find(i.first) == mB.end())
		{
			//i was NOT found in set B;
			ret.push_back(i.second);
		}
	}

	return ret;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	MessageBox(L"Get ready to compare, the report file <sb.txt> will be created at current work path.");
	CEdit e = GetDlgItem(IDC_EDIT_REGPATH1);
	ATL::CString strRegPathA;
	e.GetWindowTextW(strRegPathA);
	MessageBox(strRegPathA);

	CEdit e2 = GetDlgItem(IDC_EDIT_REGPATH2);
	ATL::CString strRegPathB;
	e2.GetWindowTextW(strRegPathB);
	MessageBox(strRegPathB);

	if (strRegPathA.IsEmpty() || strRegPathB.IsEmpty())
	{
		MessageBox(_T("CANNOT BE EMPTY"));
	}
	/*
	if (IsCompareKeyValue())
	{
		...
	}
	else
	// we only compare sub-keys
	*/
	{
		std::vector<std::wstring> vA;
		if (!GetSubkeys(strRegPathA, vA))
		{
			MessageBox(_T("Not support now 1"));
			return 0;
		}
		std::vector<std::wstring> vB;
		if (!GetSubkeys(strRegPathB, vB))
		{
			MessageBox(_T("Not support now 2"));
			return 0;
		}
	
		{
			// A\B = { x | x∈A,且x不∈B }
			std::vector<std::wstring> v = StringVectorComplement(vA, vB);
			// join string with '\n'
			std::string str;
			for (std::wstring i : v)
			{
				str += String::fromStdWString(i);
				str += "\n";
			}
			//write to file.
			std::string p = Path::getApplicationDirPath();
			p += "In_A_NOT_In_B.txt";
			FileWriter fw(p);
			if (fw.open())
			{
				fw.write(str);
				fw.close();
			}
			else
				MessageBox(_T("write data to file failed"));
		}

		{
			// B\A = { x | x∈B,且x不∈A }
			std::vector<std::wstring> v = StringVectorComplement(vB, vA);
			// join string with '\n'
			std::string str;
			for (std::wstring i : v)
			{
				str += String::fromStdWString(i);
				str += "\n";
			}
			//write to file.
			std::string p = Path::getApplicationDirPath();
			p += "In_B_NOT_In_A.txt";
			FileWriter fw(p);
			if (fw.open())
			{
				fw.write(str);
				fw.close();
			}
			else
				MessageBox(_T("write data to file failed"));
		}

		//A∩B == B∩A
		{
			std::vector<wstring> v = StringVectorIntersection(vA, vB);
			std::string str;
			for (std::wstring i : v)
			{
				str += String::fromStdWString(i);
				str += "\n";
			}
			//write to file.
			std::string p = Path::getApplicationDirPath();
			p += "In_Both_A_And_B.txt";
			FileWriter fw(p);
			if (fw.open())
			{
				fw.write(str);
				fw.close();
			}
			else
				MessageBox(_T("write data to file failed"));
		}
	}
	return 0;
}
