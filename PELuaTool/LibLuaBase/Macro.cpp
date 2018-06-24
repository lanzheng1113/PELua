#include "stdafx.h"
#include "Macros.h"
#include "util/StringEx.h"
#include <shlobj.h>
#include <string>
using std::string;
#include "util/Path.h"
#include <strsafe.h>

static string KPCmdCommonRelpace( const string& KPcmd,int pathcid,const string& tag )
{
	char szTemp[MAX_PATH] = {0};
	SHGetSpecialFolderPathA(NULL,szTemp,pathcid,FALSE);
	StringCbCatA(szTemp,sizeof(szTemp),"\\");
	String cmdEx(KPcmd);
	cmdEx.replace(tag,string(szTemp));
	return cmdEx;
}

namespace os
{
	string ReplacePathMacro( const string& KPCmd )
	{
		//SHGetSpecialFolderPath
		String cmdEx(KPCmd);
		if (cmdEx.find("${") == string::npos) 
			return cmdEx;

		if (cmdEx.find("${INSTDIR}") != string::npos)
		{
			//安装目录
			string tag = "${INSTDIR}";
			string appPath = Path::getApplicationDirPath();
			cmdEx.replace(tag,appPath);
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}

		if (cmdEx.find("${SYSTEM32}") != string::npos)
		{
			//指定SYSTEM32目录
			char szTemp[MAX_PATH] = {0};
			SHGetSpecialFolderPathA(NULL,szTemp,CSIDL_WINDOWS,FALSE);
			StringCbCatA(szTemp,sizeof(szTemp),"\\SYSTEM32\\");
			cmdEx.replace("${SYSTEM32}",string(szTemp));
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}

		if (cmdEx.find("${SYSWOW64}") != string::npos)
		{
			//SysWOW64目录
			char szTemp[MAX_PATH] = {0};
			SHGetSpecialFolderPathA(NULL,szTemp,CSIDL_WINDOWS,FALSE);
			StringCbCatA(szTemp,sizeof(szTemp),"\\SysWOW64\\");
			cmdEx.replace("${SYSWOW64}",string(szTemp));
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}

		if (cmdEx.find("${SYSTEM}") != string::npos)
		{
			//SYSTEM32或者是SysWOW64目录，视编译为64位或者是32位程序而定
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_SYSTEM,"${SYSTEM}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}

		if (cmdEx.find("${WINDOWS}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_WINDOWS,"${WINDOWS}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}

		if (cmdEx.find("${UPDATE}") != string::npos)
		{
			//软件的update目录。
			string appPath = Path::getApplicationDirPath();
			appPath += "\\update\\";
			cmdEx.replace("{UPDATE}",appPath);
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${TEMP}") != string::npos)
		{
			//临时目录下
			char szTemp[MAX_PATH] = {0};
			GetTempPathA(MAX_PATH,szTemp);
			cmdEx.replace("${HOME}",szTemp);
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${APPDATA}") != string::npos)
		{
			//C:\Documents and Settings\ username\Application Data
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_APPDATA,"${APPDATA}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_APPDATA}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Application Data
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_APPDATA,"${COMMON_APPDATA}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PROFILE}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PROFILE,"${PROFILE}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PROGRAM_FILES}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PROGRAM_FILES,"${PROGRAM_FILES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PROGRAM_FILESX86}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PROGRAM_FILESX86,"${PROGRAM_FILESX86}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PROGRAM_FILES_COMMON}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PROGRAM_FILES_COMMON,"${PROGRAM_FILES_COMMON}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PROGRAM_FILES_COMMONX86}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PROGRAM_FILES_COMMONX86,"${CSIDL_PROGRAM_FILES_COMMONX86}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PROGRAMS}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PROGRAMS,"${PROGRAMS}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_DESKTOPDIRECTORY}") != string::npos)
		{
			// C:\Documents and Settings\All Users\Desktop
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_DESKTOPDIRECTORY,"${COMMON_DESKTOPDIRECTORY}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_DOCUMENTS}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Documents
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_DOCUMENTS,"${COMMON_DOCUMENTS}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_FAVORITES}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_FAVORITES,"${COMMON_FAVORITES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_MUSIC}") != string::npos)
		{
			// C:\Documents and Settings\All Users\Documents\My Music.
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_MUSIC,"${COMMON_MUSIC}");
			if (cmdEx.find("${") == string::npos)
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_PICTURES}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Documents\My Pictures
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_PICTURES,"${COMMON_PICTURES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_PROGRAMS}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Start Menu\Programs
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_PROGRAMS,"${COMMON_PROGRAMS}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_STARTMENU}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Start Menu
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_STARTMENU,"${COMMON_STARTMENU}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_STARTUP}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Start Menu\Programs\Startup
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_STARTUP,"${COMMON_STARTUP}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_TEMPLATES}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Templates
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_TEMPLATES,"${COMMON_TEMPLATES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMMON_VIDEO}") != string::npos)
		{
			//C:\Documents and Settings\All Users\Documents\My Videos
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMMON_VIDEO,"${COMMON_VIDEO}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COMPUTERSNEARME}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COMPUTERSNEARME,"${COMPUTERSNEARME}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${COOKIES}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_COOKIES,"${COOKIES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${DESKTOP}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_DESKTOP,"${DESKTOP}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${DESKTOPDIRECTORY}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_DESKTOPDIRECTORY,"${DESKTOPDIRECTORY}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${FAVORITES}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_FAVORITES,"${FAVORITES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${FONTS}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_FONTS,"${FONTS}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${HISTORY}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_HISTORY,"${HISTORY}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${INTERNET}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_INTERNET,"${INTERNET}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${INTERNET_CACHE}") != 
			string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_INTERNET_CACHE,"${INTERNET_CACHE}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${LOCAL_APPDATA}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_LOCAL_APPDATA,"${LOCAL_APPDATA}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${MYDOCUMENTS}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_MYDOCUMENTS,"${MYDOCUMENTS}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${MYMUSIC}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_MYMUSIC,"${MYMUSIC}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${MYPICTURES}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_MYPICTURES,"${MYPICTURES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${MYVIDEO}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_MYVIDEO,"${MYVIDEO}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${NETHOOD}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_NETHOOD,"${NETHOOD}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${NETWORK}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_NETWORK,"${NETWORK}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PERSONAL}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PERSONAL,"${PERSONAL}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PRINTERS}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PRINTERS,"${PRINTERS}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${PRINTHOOD}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_PRINTHOOD,"${PRINTHOOD}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${RECENT}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_RECENT,"${RECENT}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${RESOURCES}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_RESOURCES,"${RESOURCES}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${RESOURCES_LOCALIZED}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_RESOURCES_LOCALIZED,"${RESOURCES_LOCALIZED}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${SENDTO}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_SENDTO,"${SENDTO}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${STARTMENU}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_STARTMENU,"${STARTMENU}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${STARTUP}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_STARTUP,"${STARTUP}");
			if (cmdEx.find("${") == string::npos) 
				return cmdEx;
		}
		if (cmdEx.find("${TEMPLATES}") != string::npos)
		{
			cmdEx = KPCmdCommonRelpace(cmdEx,CSIDL_TEMPLATES,"${TEMPLATES}");
			if (cmdEx.find("${") == string::npos)
				return cmdEx;
		}
		return cmdEx;
	}
}

