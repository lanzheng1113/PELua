-- ����soft2�Ŀ�ݷ�ʽ --
ע���ɾ���� = RegIni.DeleteRegKey
ע���ɾ��ֵ = RegIni.DeleteRegValue
ע���ö�ټ� = RegIni.EnumRegKey
ע���ö��ֵ = RegIni.EnumRegValue
-- ��ע�����д��һ��DWORD��If the registry key doesn't exist it will be created.
-- �ĸ�RegIni.WriteRegDWORD�Ĳ����������£�
-- ����(string)����Ч�ĸ���"HKLM"->HKEY_LOCAL_MACHINE,"HKCU"->HKEY_CURRENT_USER ...
-- �ӽ�·��(string)
-- ����(string)
-- DWORDֵ(integer)
-- ����ֵ������Ϊboolean�ɹ�����trueʧ�ܷ���false.
-- WriteRegDWORD("HKLM","Software\\My Company\\My Software","DWORD Value",0xDEADBEEF)
ע�������� = RegIni.ReadRegDWORD
ע�����ַ��� = RegIni.ReadRegStr
ע�������WOW64 = RegIni.SetRegView
ע���д������ = RegIni.WriteRegBin
ע���д���� = RegIni.WriteRegDWORD
ע���д�ַ��� = RegIni.WriteRegStr
ע���д��չ�ַ��� = RegIni.WriteRegExpandStr
���������� = RegIni.ReadEnvStr

function ��ȡ����Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_DESKTOPDIRECTORY)
end

function ��ȡ��ʼ�˵�Ŀ¼()
	return OsExt.GetSpecialFolder(CSIDL_COMMON_STARTMENU)
end

function ע��Soft2()
	ע���д����("HKCU","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer","HideSCAVolume",1)
	ע���ɾ����("HKLM","SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\HideSCAVolume")
	ע���д����("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\TabbedBrowsing","NTPFirstRun",1)
	ע���д����("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\TabbedBrowsing","NewTabpageShow",0)
	ע���д����("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\TabbedBrowsing","ThumbnailBehavior",0)
	ע���д����("HKCU","Software\\StartIsBack","StartMetroApps",0)
	ע���д����("HKCU","Software\StartIsBack","Start_NotifyNewApps",0)
	ע���д�ַ���("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\New Windows","PopupMgr","yes")
	ע���д����("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\New Windows","PlaySound",0)
	ע���д����("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\New Windows","UseSecBand",0)
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\Main","DoNotTrack",1)
	ע���д����("HKLM","Software\\Microsoft\\Internet Explorer\\Main","RunOnceHasShown",1)
	ע���д����("HKLM","Software\\Microsoft\\Internet Explorer\\Main","RunOnceComplete",1)
	ע���д����("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\Main","DisableFirstRunCustomize",1)
	ע���д����("HKLM","Software\\Microsoft\\Internet Explorer\\TabbedBrowsing","PopupsUseNewWindow",2)
	ע���д�ַ���("HKLM","Software\\Microsoft\\Internet Explorer\\Main","Enable AutoImageResize","yes")
	ע���д�ַ���("HKLM","Software\\Microsoft\\Internet Explorer\\Main","UseClearType","yes")
	ע���д����("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\FlipAhead","FPEnabled",0)
	ע���д����("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\Suggested Sites","DataStreamEnabledState",0)
	ע���д����("HKCU","SOFTWARE\\Microsoft\\Internet Explorer\\Main","NoProtectedModeBanner",1)
	ע���д����("HKCU","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\3","2500",3)
	ע���д����("HKCU","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\4","2500",3)
	ע���д����("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\PhishingFilter","EnabledV9",0)
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\Main","RunOnceHasShown",1)
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\Main","RunOnceComplete",1)
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\Main","DisableFirstRunCustomize",1)
	ע���д����("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\\Main","DisableFirstRunCustomize",1)
	ע���д����("HKLM","SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\Infodelivery\Restrictions","NoUpdateCheck",1)
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\TabbedBrowsing","WarnOnClose",0)
	ע���д�ַ���("HKCU","Software\\Microsoft\\Internet Explorer\\Main","Use FormSuggest","yes")
	ע���д����("HKLM","SOFTWARE\\Microsoft\\Internet Explorer\MAIN","DEPOff",1)
	ע���д�ַ���("HKCU","Software\\Microsoft\\Internet Explorer\\Main","Enable AutoImageResize","yes")
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\Suggested Sites","Enabled",0)
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\Main","NoUpdateCheck",1)
	ע���д�ַ���("HKCU","Software\\Microsoft\\Internet Explorer\\Toolbar","LinksFolderName"," ")
	ע���д����("HKCU","Software\\Microsoft\\Internet Explorer\\Toolbar","Locked",1)
	ע���д�ַ���("HKCU","Software\\Microsoft\\Internet Explorer\\Toolbar","ShowDiscussionButton","Yes")
	ע���д����("HKLM","SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\SQM","DisableCustomerImprovementProgram",1)
	ע���д�ַ���("HKLM","SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\Main","Isolation","PMIL")
	
	-- ���뷨,������ִ����Ͼͷ��ء�
	ִ���ӽ��̲��ȴ������ = OsExt.ExecWait
	ִ���ӽ��� = OsExt.Exec
	ִ���ӽ��̲��ȴ������("X:\\PEPrograms\\X86Soft2\\FREEIME\\REGISTRY.EXE /s",SW_HIDE)
	
	-- ���ͨ�Ҽ�ע��
	����·�� = OsExt.PathFileExist
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\ULTRAISO\\ULTRAISO.EXE") then
		ִ���ӽ��̲��ȴ������("cmd.exe /c X:\\PEPrograms\\X86Soft2\\���̹���\\ULTRAISO\\ULTRAISO.CMD",NULL,SW_HIDE)
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\�ļ�����\\IMAGINE\\IMAGINE.EXE") then
		OsExt.ExecWait("X:\\PEPrograms\\X86Soft2\\�ļ�����\\IMAGINE\IMAGINE.EXE /assocext")
	end
	
	-- ���տ�ע������°汾��Ҫ����
	ע���д����("HKCU","SOFTWARE\\Oray\\SunLogin\\SunloginClient","9.0.2.48501_IsRunSeted",1)
	ע���д����("HKCU","SOFTWARE\\Oray\\SunLogin\\SunloginClient","9.1.0.53088_IsRunSeted",1)
	ע���д����("HKCU","SOFTWARE\\Oray\\SunLogin\\SunloginClient","9.6.0.56908_IsRunSeted",1)
	
	-- XPע���
	if ����·��("X:\\WXPE") then
		ע���д�ַ���("HKLM","SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName","ComputerName","WINPE")
		ע���д�ַ���("HKLM","SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName","ComputerName","WINPE")
		ע���д�ַ���("HKLM","SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters","Hostname","WINPE")
		ע���д�ַ���("HKLM","SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters","NV Hostname","WINPE")
		ע���д�ַ���("HKLM","SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment","ComputerName","WINPE")
		ע���д�ַ���("HKLM","SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion","RegisteredOwner","User")
		ע���д�ַ���("HKLM","SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion","RegisteredOrganization","Microsoft")
	end
	-- �����ݷ�ʽ
	����Ŀ¼ = ��ȡ����Ŀ¼()
	������ݷ�ʽ = OsExt.CreateShortCut
	if ����·��("X:\WXPE") then
		������ݷ�ʽ(����Ŀ¼ .. "\\Զ�̿���AnyDesk.lnk","X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\AnyDesk.exe")
	end
	������ݷ�ʽ(����Ŀ¼ .. "\\Զ�̿������տ�.lnk","X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\Sunlogin.exe")
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\DiskGenius_Mono.exe") then
		������ݷ�ʽ(����Ŀ¼ .. "\\������ʦ.lnk","X:\\PEPrograms\\X86Soft2\\���̹���\\DiskGenius_Mono.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���ݻ�ԭ\\GHOST.EXE") then
		������ݷ�ʽ(����Ŀ¼ .. "�ֶ�Ghost.lnk","X:\\PEPrograms\\X86Soft2\\���ݻ�ԭ\\GHOST.EXE")
	end
	
	--��ʼ�˵�
	��ʼ�˵�Ŀ¼ = ��ȡ��ʼ�˵�Ŀ¼()
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\DiskGenius_Mono.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\������ʦ.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\DiskGenius_Mono.exe")
	end
	������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���ع���.lnk", "X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\MiniGet\\MiniGet.exe")
	if ����·��("X:\\PEPrograms\\X86Soft2\\ϵͳά��\\NTPWEDIT\\NTPWEDIT.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ϵͳ�����ƽ�.lnk", "X:\\PEPrograms\\X86Soft2\\ϵͳά��\\NTPWEDIT\\NTPWEDIT.EXE")
	end
	
	--����
	if ����·��("X:\\PEPrograms\\X86Soft2\\���ݻ�ԭ\\CGI��׼��32λ.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���ݻ�ԭ\\CGI.lnk", "X:\\PEPrograms\\X86Soft2\\���ݻ�ԭ\\CGI��׼��32λ.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\�����޸�\\UEFIX\\UEFIX.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\�����޸�\\UEFI�����޸�.lnk", "X:\\PEPrograms\\X86Soft2\\�����޸�\\UEFIX\\UEFIX.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\�����޸�\\NTBOOTAUTOFIX\\NTBOOTAUTOFIX.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\�����޸�\\Widnows�����޸�.lnk", "X:\\PEPrograms\\X86Soft2\\�����޸�\\NTBOOTAUTOFIX\\NTBOOTAUTOFIX.EXE")
	end
	if ����·��("X:\\WXPE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\�����޸�\\Dism++.lnk", "X:\\PEPrograms\\X86Soft2\\�����޸�\\Dism++\\Dism++x86.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\DiskGenius_Mono.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\������ʦ.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\DiskGenius_Mono.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\��÷��������\\��������PE.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\��������.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\��÷��������\\��������PE.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\���̻��������޸�VICTORIA.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\���̻��������޸�.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\���̻��������޸�VICTORIA.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\���̴����޸�NDD2006.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\���̴����޸�.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\���̴����޸�NDD2006.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\Defraggler.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\Ӳ����Ƭ����.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\Defraggler.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\BOOTICEx86.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\BOOTICE��������.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\BOOTICEx86.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\ULTRAISO\\ULTRAISO.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\ULTRAISO.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\ULTRAISO\\ULTRAISO.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\оƬ����.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\оƬ����.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\оƬ����.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\���̹���\\FbinstTool.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\���̹���\\FbinstTool.lnk", "X:\\PEPrograms\\X86Soft2\\���̹���\\FbinstTool.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\�ļ�����\\IMAGINE\IMAGINE.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\�ļ�����\\Imageine.lnk", "X:\\PEPrograms\\X86Soft2\\�ļ�����\\IMAGINE\\IMAGINE.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\�ļ�����\\DEFORMITYDIR\\DEFORMITYDIR.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\�ļ�����\\����Ŀ¼����.lnk", "X:\\PEPrograms\\X86Soft2\\�ļ�����\\DEFORMITYDIR\\DEFORMITYDIR.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\ϵͳά��\\WinNTSetup3\\WinNTSetup_v3.8.8.6_Final_Plus.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ϵͳά��\\NT6��װ��.lnk", "X:\\PEPrograms\\X86Soft2\\ϵͳά��\\WinNTSetup3\\WinNTSetup_v3.8.8.6_Final_Plus.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\ϵͳά��\\NTPWEDIT\\NTPWEDIT.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ϵͳά��\\ϵͳ�����ƽ�.lnk", "X:\\PEPrograms\\X86Soft2\\ϵͳά��\\NTPWEDIT\\NTPWEDIT.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\Ӳ�����\\HDTune.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\Ӳ�����\\�������ܼ��.lnk", "X:\\PEPrograms\\X86Soft2\\Ӳ�����\\HDTune.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\Ӳ�����\\MEMTEST.EXE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\Ӳ�����\\�ڴ�������.lnk", "X:\\PEPrograms\\X86Soft2\\Ӳ�����\\MEMTEST.EXE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\Ӳ�����\\AIDA64.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\Ӳ�����\\Ӳ����ϵͳ���AIDA64.lnk", "X:\\PEPrograms\\X86Soft2\\Ӳ�����\\AIDA64.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\Ӳ�����\\DisplayX.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\Ӳ�����\\��ʾ������.lnk", "X:\\PEPrograms\\X86Soft2\\Ӳ�����\\DisplayX.exe")
	end
	if ����·��("X:\\WXPE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ʵ�ù���\\Զ�̿���AnyDesk.lnk", "X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\AnyDesk.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\Sunlogin.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ʵ�ù���\\Զ�̿������տ�.lnk", "X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\Sunlogin.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\ARTICLE") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ʵ�ù���\\����.lnk", "X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\ARTICLE")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\Hash_1.0.4_XiaZaiBa.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ʵ�ù���\MD5У�鹤��.lnk", "X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\Hash_1.0.4_XiaZaiBa.exe")
	end
	if ����·��("X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\MiniGet\\MiniGet.exe") then
		������ݷ�ʽ(��ʼ�˵�Ŀ¼ .. "\\ʵ�ù���\\���ع���.lnk", "X:\\PEPrograms\\X86Soft2\\ʵ�ù���\\MiniGet\MiniGet.exe")
	end
end

ע��Soft2()