-- 创建soft2的快捷方式 --
注册表删除键 = RegIni.DeleteRegKey
注册表删除值 = RegIni.DeleteRegValue
注册表枚举键 = RegIni.EnumRegKey
注册表枚举值 = RegIni.EnumRegValue
-- 往注册表中写入一个DWORD。If the registry key doesn't exist it will be created.
-- 四个RegIni.WriteRegDWORD的参数依次如下：
-- 根键(string)，有效的根键"HKLM"->HKEY_LOCAL_MACHINE,"HKCU"->HKEY_CURRENT_USER ...
-- 子健路径(string)
-- 键名(string)
-- DWORD值(integer)
-- 返回值的类型为boolean成功返回true失败返回false.
-- WriteRegDWORD("HKLM","Software\\My Company\\My Software","DWORD Value",0xDEADBEEF)
注册表读整数 = RegIni.ReadRegDWORD
注册表读字符串 = RegIni.ReadRegStr
注册表设置WOW64 = RegIni.SetRegView
注册表写二进制 = RegIni.WriteRegBin
注册表写整数 = RegIni.WriteRegDWORD
注册表写字符串 = RegIni.WriteRegStr
注册表写扩展字符串 = RegIni.WriteRegExpandStr
读环境变量 = RegIni.ReadEnvStr

function 获取桌面目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_DESKTOPDIRECTORY)
end

function 获取开始菜单目录()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_STARTMENU)
end

function 注册Soft2()
	注册表写整数("HKCU","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer","HideSCAVolume",1)
	注册表删除键("HKLM","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\HideSCAVolume")
	注册表写整数("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\TabbedBrowsing","NTPFirstRun",1)
	注册表写整数("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\TabbedBrowsing","NewTabpageShow",0)
	注册表写整数("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\TabbedBrowsing","ThumbnailBehavior",0)
	注册表写整数("HKCU","Software\\StartIsBack","StartMetroApps",0)
	注册表写整数("HKCU","Software\StartIsBack","Start_NotifyNewApps",0)
	注册表写字符串("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\New Windows","PopupMgr","yes")
	注册表写整数("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\New Windows","PlaySound",0)
	注册表写整数("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\New Windows","UseSecBand",0)
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\Main","DoNotTrack",1)
	注册表写整数("HKLM","Software\\Microsoft\\Internet Explorer\\Main","RunOnceHasShown",1)
	注册表写整数("HKLM","Software\\Microsoft\\Internet Explorer\\Main","RunOnceComplete",1)
	注册表写整数("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\Main","DisableFirstRunCustomize",1)
	注册表写整数("HKLM","Software\\Microsoft\\Internet Explorer\\TabbedBrowsing","PopupsUseNewWindow",2)
	注册表写字符串("HKLM","Software\\Microsoft\\Internet Explorer\\Main","Enable AutoImageResize","yes")
	注册表写字符串("HKLM","Software\\Microsoft\\Internet Explorer\\Main","UseClearType","yes")
	注册表写整数("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\FlipAhead","FPEnabled",0)
	注册表写整数("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\Suggested Sites","DataStreamEnabledState",0)
	注册表写整数("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\Main","NoProtectedModeBanner",1)
	注册表写整数("HKCU","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\3","2500",3)
	注册表写整数("HKCU","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\4","2500",3)
	注册表写整数("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\PhishingFilter","EnabledV9",0)
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\Main","RunOnceHasShown",1)
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\Main","RunOnceComplete",1)
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\Main","DisableFirstRunCustomize",1)
	注册表写整数("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\Main","DisableFirstRunCustomize",1)
	注册表写整数("HKLM","SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\Infodelivery\Restrictions","NoUpdateCheck",1)
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\TabbedBrowsing","WarnOnClose",0)
	注册表写字符串("HKCU","Software\\Microsoft\\Internet Explorer\\Main","Use FormSuggest","yes")
	注册表写整数("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\MAIN","DEPOff",1)
	注册表写字符串("HKCU","Software\\Microsoft\\Internet Explorer\\Main","Enable AutoImageResize","yes")
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\Suggested Sites","Enabled",0)
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\Main","NoUpdateCheck",1)
	注册表写字符串("HKCU","Software\\Microsoft\\Internet Explorer\\Toolbar","LinksFolderName"," ")
	注册表写整数("HKCU","Software\\Microsoft\\Internet Explorer\\Toolbar","Locked",1)
	注册表写字符串("HKCU","Software\\Microsoft\\Internet Explorer\\Toolbar","ShowDiscussionButton","Yes")
	注册表写整数("HKLM","SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\SQM","DisableCustomerImprovementProgram",1)
	注册表写字符串("HKLM","SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\Main","Isolation","PMIL")
	
	-- 输入法,不等它执行完毕就返回。
	执行子进程并等待它完成 = OsExt.ExecWait
	执行子进程 = OsExt.Exec
	执行子进程并等待它完成("X:\\PEPrograms\\X86Soft2\\FREEIME\\REGISTRY.EXE /s",SW_HIDE)
	
	-- 软碟通右键注册
	存在路径 = OsExt.PathFileExist
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\ULTRAISO\\ULTRAISO.EXE") then
		执行子进程并等待它完成("cmd.exe /c X:\\PEPrograms\\X86Soft2\\磁盘光盘\\ULTRAISO\\ULTRAISO.CMD",NULL,SW_HIDE)
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\文件工具\\IMAGINE\\IMAGINE.EXE") then
		OsExt.ExecWait("X:\\PEPrograms\\X86Soft2\\文件工具\\IMAGINE\IMAGINE.EXE /assocext")
	end
	
	-- 向日葵注册表处理，新版本号要增加
	注册表写整数("HKCU","SOFTWARE\\Oray\\SunLogin\\SunloginClient","9.0.2.48501_IsRunSeted",1)
	注册表写整数("HKCU","SOFTWARE\\Oray\\SunLogin\\SunloginClient","9.1.0.53088_IsRunSeted",1)
	注册表写整数("HKCU","SOFTWARE\\Oray\\SunLogin\\SunloginClient","9.6.0.56908_IsRunSeted",1)
	
	-- XP注册表
	if 存在路径("X:\\WXPE") then
		注册表写字符串("HKLM","SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName","ComputerName","WINPE")
		注册表写字符串("HKLM","SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName","ComputerName","WINPE")
		注册表写字符串("HKLM","SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters","Hostname","WINPE")
		注册表写字符串("HKLM","SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters","NV Hostname","WINPE")
		注册表写字符串("HKLM","SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment","ComputerName","WINPE")
		注册表写字符串("HKLM","SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion","RegisteredOwner","User")
		注册表写字符串("HKLM","SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion","RegisteredOrganization","Microsoft")
	end
	-- 桌面快捷方式
	桌面目录 = 获取桌面目录()
	创建快捷方式 = OsExt.CreateShortCut
	if 存在路径("X:\WXPE") then
		创建快捷方式(桌面目录 .. "\\远程控制AnyDesk.lnk","X:\\PEPrograms\\X86Soft2\\实用工具\\AnyDesk.exe")
	end
	创建快捷方式(桌面目录 .. "\\远程控制向日葵.lnk","X:\\PEPrograms\\X86Soft2\\实用工具\\Sunlogin.exe")
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\DiskGenius_Mono.exe") then
		创建快捷方式(桌面目录 .. "\\分区大师.lnk","X:\\PEPrograms\\X86Soft2\\磁盘光盘\\DiskGenius_Mono.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\备份还原\\GHOST.EXE") then
		创建快捷方式(桌面目录 .. "手动Ghost.lnk","X:\\PEPrograms\\X86Soft2\\备份还原\\GHOST.EXE")
	end
	
	--开始菜单
	开始菜单目录 = 获取开始菜单目录()
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\DiskGenius_Mono.exe") then
		创建快捷方式(开始菜单目录 .. "\\分区大师.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\DiskGenius_Mono.exe")
	end
	创建快捷方式(开始菜单目录 .. "\\下载工具.lnk", "X:\\PEPrograms\\X86Soft2\\实用工具\\MiniGet\\MiniGet.exe")
	if 存在路径("X:\\PEPrograms\\X86Soft2\\系统维护\\NTPWEDIT\\NTPWEDIT.EXE") then
		创建快捷方式(开始菜单目录 .. "\\系统密码破解.lnk", "X:\\PEPrograms\\X86Soft2\\系统维护\\NTPWEDIT\\NTPWEDIT.EXE")
	end
	
	--程序
	if 存在路径("X:\\PEPrograms\\X86Soft2\\备份还原\\CGI标准版32位.exe") then
		创建快捷方式(开始菜单目录 .. "\\备份还原\\CGI.lnk", "X:\\PEPrograms\\X86Soft2\\备份还原\\CGI标准版32位.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\引导修复\\UEFIX\\UEFIX.EXE") then
		创建快捷方式(开始菜单目录 .. "\\引导修复\\UEFI引导修复.lnk", "X:\\PEPrograms\\X86Soft2\\引导修复\\UEFIX\\UEFIX.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\引导修复\\NTBOOTAUTOFIX\\NTBOOTAUTOFIX.EXE") then
		创建快捷方式(开始菜单目录 .. "\\引导修复\\Widnows引导修复.lnk", "X:\\PEPrograms\\X86Soft2\\引导修复\\NTBOOTAUTOFIX\\NTBOOTAUTOFIX.EXE")
	end
	if 存在路径("X:\\WXPE") then
		创建快捷方式(开始菜单目录 .. "\\引导修复\\Dism++.lnk", "X:\\PEPrograms\\X86Soft2\\引导修复\\Dism++\\Dism++x86.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\DiskGenius_Mono.exe") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\分区大师.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\DiskGenius_Mono.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\傲梅分区助手\\分区助手PE.exe") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\分区助手.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\傲梅分区助手\\分区助手PE.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\磁盘坏道分析修复VICTORIA.EXE") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\磁盘坏道分析修复.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\磁盘坏道分析修复VICTORIA.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\磁盘错误修复NDD2006.EXE") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\磁盘错误修复.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\磁盘错误修复NDD2006.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\Defraggler.exe") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\硬盘碎片整理.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\Defraggler.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\BOOTICEx86.exe") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\BOOTICE引导管理.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\BOOTICEx86.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\ULTRAISO\\ULTRAISO.EXE") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\ULTRAISO.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\ULTRAISO\\ULTRAISO.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\芯片无忧.exe") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\芯片无忧.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\芯片无忧.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\磁盘光盘\\FbinstTool.exe") then
		创建快捷方式(开始菜单目录 .. "\\磁盘光盘\\FbinstTool.lnk", "X:\\PEPrograms\\X86Soft2\\磁盘光盘\\FbinstTool.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\文件工具\\IMAGINE\IMAGINE.EXE") then
		创建快捷方式(开始菜单目录 .. "\\文件工具\\Imageine.lnk", "X:\\PEPrograms\\X86Soft2\\文件工具\\IMAGINE\\IMAGINE.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\文件工具\\DEFORMITYDIR\\DEFORMITYDIR.EXE") then
		创建快捷方式(开始菜单目录 .. "\\文件工具\\畸形目录处理.lnk", "X:\\PEPrograms\\X86Soft2\\文件工具\\DEFORMITYDIR\\DEFORMITYDIR.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\系统维护\\WinNTSetup3\\WinNTSetup_v3.8.8.6_Final_Plus.exe") then
		创建快捷方式(开始菜单目录 .. "\\系统维护\\NT6安装器.lnk", "X:\\PEPrograms\\X86Soft2\\系统维护\\WinNTSetup3\\WinNTSetup_v3.8.8.6_Final_Plus.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\系统维护\\NTPWEDIT\\NTPWEDIT.EXE") then
		创建快捷方式(开始菜单目录 .. "\\系统维护\\系统密码破解.lnk", "X:\\PEPrograms\\X86Soft2\\系统维护\\NTPWEDIT\\NTPWEDIT.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\硬件检测\\HDTune.exe") then
		创建快捷方式(开始菜单目录 .. "\\硬件检测\\磁盘性能检测.lnk", "X:\\PEPrograms\\X86Soft2\\硬件检测\\HDTune.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\硬件检测\\MEMTEST.EXE") then
		创建快捷方式(开始菜单目录 .. "\\硬件检测\\内存条测试.lnk", "X:\\PEPrograms\\X86Soft2\\硬件检测\\MEMTEST.EXE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\硬件检测\\AIDA64.exe") then
		创建快捷方式(开始菜单目录 .. "\\硬件检测\\硬件及系统检测AIDA64.lnk", "X:\\PEPrograms\\X86Soft2\\硬件检测\\AIDA64.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\硬件检测\\DisplayX.exe") then
		创建快捷方式(开始菜单目录 .. "\\硬件检测\\显示器测试.lnk", "X:\\PEPrograms\\X86Soft2\\硬件检测\\DisplayX.exe")
	end
	if 存在路径("X:\\WXPE") then
		创建快捷方式(开始菜单目录 .. "\\实用工具\\远程控制AnyDesk.lnk", "X:\\PEPrograms\\X86Soft2\\实用工具\\AnyDesk.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\实用工具\\Sunlogin.exe") then
		创建快捷方式(开始菜单目录 .. "\\实用工具\\远程控制向日葵.lnk", "X:\\PEPrograms\\X86Soft2\\实用工具\\Sunlogin.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\实用工具\\ARTICLE") then
		创建快捷方式(开始菜单目录 .. "\\实用工具\\资料.lnk", "X:\\PEPrograms\\X86Soft2\\实用工具\\ARTICLE")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\实用工具\\Hash_1.0.4_XiaZaiBa.exe") then
		创建快捷方式(开始菜单目录 .. "\\实用工具\MD5校验工具.lnk", "X:\\PEPrograms\\X86Soft2\\实用工具\\Hash_1.0.4_XiaZaiBa.exe")
	end
	if 存在路径("X:\\PEPrograms\\X86Soft2\\实用工具\\MiniGet\\MiniGet.exe") then
		创建快捷方式(开始菜单目录 .. "\\实用工具\\下载工具.lnk", "X:\\PEPrograms\\X86Soft2\\实用工具\\MiniGet\MiniGet.exe")
	end
end

注册Soft2()